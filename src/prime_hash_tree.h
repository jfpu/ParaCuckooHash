#pragma once

#include <vector>
#include <utility>
#include <functional>  // std::hash
#include <unordered_map>
#include <type_traits>
#include <math.h>
#include <assert.h>

using namespace::std;

/**
 * alignas() requires GCC >= 4.9, so we stick with the alignment attribute for
 * GCC.
 */
#ifdef __GNUC__
#define ALIGNAS(x) __attribute__((aligned(x)))
#else
#define ALIGNAS(x) alignas(x)
#endif

// c++11 for std::is_same_v
template< class T, class U >
inline constexpr bool is_same_v = is_same<T, U>::value;

#define EPSILON 0.0001f

// https://zhuanlan.zhihu.com/p/290832847
#if 0
vector<int> PRIME_BASE = {256, 255, 253, 251, 247, 241, 239, 233, 229, 227, 223, 217, 211, 199,
	197, 193, 191, 181, 179, 173, 167, 163, 157, 151, 149, 139, 137, 131, 127, 113, 109, 107, 103,
  101, 97, 89, 83, 79, 73, 71, 67, 61, 59, 53, 47, 43, 41, 37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};
#endif
vector<int> PRIME_BASE = {257, 251, 247, 241, 239, 233, 229, 227, 223, 217, 211, 199,
	197, 193, 191, 181, 179, 173, 167, 163, 157, 151, 149, 139, 137, 131, 127, 113, 109, 107, 103,
  101, 97, 89, 83, 79, 73, 71, 67, 61, 59, 53, 47, 43, 41, 37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};

int get_residue(uint32_t key, int prime) {
  return key % prime;
}

int get_residue(uint64_t key, int prime) {
  return key % prime;
}

int get_residue(std::string& key, int prime) {
  int residue = 0;
  //Each char is divided by prime.
  for (int i = 0; i < key.size();++i) {
    residue = (key[i] + (residue << 16)) % prime;
  }
  return residue;
}

int get_residue(char* datas,int prime) {
  int residue = 0;
  //Each byte is divided by prime.
  for (int i = 0; i < strlen(datas); ++i) {
      residue = (datas[i] + (residue << 8)) % prime;
  }
  return residue;
}

template <typename Key, typename Value, typename KeyHash = std::hash<Key>, class KeyEqual = std::equal_to<Key> >
class PrimeHashTree {
 public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = std::pair<key_type, mapped_type>;
  using size_type = std::size_t;

  // using bucket_t = std::vector<value_type>;
  using key_hash_t = KeyHash;
  using key_equal_t = KeyEqual;

  PrimeHashTree(uint64_t num_buckets = 256, float load_factor = 1.0f)
    : set_nums_(num_buckets),
      set_load_factor_(load_factor),
      nums_(0),
      load_factor_(0.0f) {
    if ((std::fabs(set_load_factor_ - 0.0f) <= EPSILON) || (set_load_factor_ > 1.0f)) {
      set_load_factor_ = 1.0f;
    }
    uint64_t approximate_nums = std::ceil(set_nums_ / set_load_factor_);
    if (approximate_nums == 0 || approximate_nums < 2) {
      approximate_nums = 2;
    }

    // double pointer to find best prime parameters: [)
    int left = 0;
    while (approximate_nums < PRIME_BASE[left]) {
      ++left;
    }
    assert(left < PRIME_BASE.size());
    int right = left + 1;
    uint64_t product = PRIME_BASE[left];
    while (right < PRIME_BASE.size()) {
      if (product >= approximate_nums) {
        break;
      }
      product *= PRIME_BASE[right++];
    }
    assert(left < right);
    assert(right <= PRIME_BASE.size());
    assert(product >= approximate_nums);

    // unable fail, do not check
    uint64_t next_product = product;
    while (right < PRIME_BASE.size()) {
      next_product *= PRIME_BASE[right];
      next_product /= PRIME_BASE[left];
      if (next_product < approximate_nums) {
        break;
      }
      product = next_product;
      ++left;
      ++right;
    }
    assert(left < right);
    assert(right <= PRIME_BASE.size());
    assert(product >= approximate_nums);
    assert(next_product <= approximate_nums);

    // init buckets
    buckets_.resize(product);
    expand_rate_ = (float) product / approximate_nums;
    // expand_rate_ = (float)product / set_nums_;

    // set real prime offsets
    primes_.resize(right - left);
    for (int i = 0; left < right; ) {
      primes_[i++] = PRIME_BASE[left++];
    }
    uint64_t offset_product = 1;
    offsets_.resize(primes_.size());
    for (int j = (primes_.size() - 1); j >= 0; --j) {
      offsets_[j] = offset_product;
      offset_product *= primes_[j];
    }

    // check
    uint64_t check_product = 1;
    for (int i = 0; i < primes_.size(); ++i) {
      check_product *= primes_[i];
    }
    assert(check_product == product);
  }

  ~PrimeHashTree() {}

  mapped_type get(key_type key) {
   vector<uint32_t> mods(primes_.size());
    for (int i = 0; i < primes_.size(); ++i) {
      mods[i] = get_residue(key, primes_[i]);
    }

    int loc = 0;
    for (int i = 0; i < mods.size(); ++i) {
      loc += (mods[i] * offsets_[i]);
    }
    assert(loc < buckets_.size());

    value_type& kv = buckets_[loc];
    if (kv.first == key) {
      return kv.second;
    }

    auto it = overflows_.find(key);
    if (overflows_.end() == it) {
      return mapped_type();
    }
    return it->second;
  }

  void put(key_type key, mapped_type val) {
    vector<uint32_t> mods(primes_.size());
    for (int i = 0; i < primes_.size(); ++i) {
      mods[i] = get_residue(key, primes_[i]);
    }

    int loc = 0;
    for (int i = 0; i < mods.size(); ++i) {
      loc += (mods[i] * offsets_[i]);
    }
    assert(loc < buckets_.size());

    bool overflow = false;
    value_type& kv = buckets_[loc];

    // update
    if (kv.first == key) {
      kv.second = val;
      return;
    }

    if constexpr (std::is_same<key_type, std::string>::value) {
      overflow = !kv.first.empty();
    } else {
      overflow = (0 != kv.first);
    }

    // insert kv
    if (!overflow) {
      kv.first = key;
      kv.second = val;
    } else {
      auto it = overflows_.find(key);
      if (overflows_.end() != it) {
        --nums_;
      }
      overflows_[key] = val;
    }

    ++nums_;
  }

  mapped_type find(key_type key) {
    return get(key);
  }

  void insert(key_type key, mapped_type val) {
    put(key, val);
  }

  uint32_t get_tree_length() const {
    assert(primes_.size() == offsets_.size());
    return primes_.size();
  }

  float get_set_load_factor() const {
    return set_load_factor_;
  }

  float load_factor() {
    size_t on = (overflows_.load_factor() == 0) ? overflows_.size() : (overflows_.size() / overflows_.load_factor());
    // load_factor_ = (float) (nums_) / (buckets_.size() + on);
    load_factor_ = (float) (nums_) / (buckets_.size());
    return load_factor_;
  }

  float overflow_rate() const {
    if (0 == nums_) return 0;
    return (float)overflows_.size() / nums_;
  }

  size_t overflow_nums() const {
    return overflows_.size();
  }

  size_t length() const {
    return nums_;
  }

  size_t size() const {
    return buckets_.size();
  }

  void print() {
    std::cout << "\n\tBuckets: " << buckets_.size() << ", SetBucketNums: " << set_nums_
              << ", SetLoadFactor: " << set_load_factor_ << ", ExpandRate: " << expand_rate_
              << "\n\tCurrent Nums: " << nums_ << ", CurLoadFactor: " << load_factor()
              << "\n\tOverflow Nums: " << overflows_.size() << ", Overflow Rate: " << overflow_rate();
    std::cout << "\n\tPrimes: " << primes_.size();
    for (auto& p: primes_) {
      cout << " : " << p;
    }
    std::cout << "\n\tOffsets: " << offsets_.size();
    for (auto& s: offsets_) {
      cout << " : " << s;
    }
    cout << "\n";
  }

 private:
  bool available(value_type& bkt) {

  }

  size_t set_nums_;
  float set_load_factor_;
  // uint32_t set_nums_;
  float expand_rate_;

  // data statistics
  size_t nums_;
  float load_factor_;

  std::vector<value_type> buckets_;
  vector<uint32_t> primes_;
  vector<uint64_t> offsets_;

  // store overflows
  unordered_map<key_type, mapped_type> overflows_;
};
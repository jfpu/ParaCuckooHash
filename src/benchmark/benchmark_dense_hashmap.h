#ifndef BENCHMARK_DENSEHASHMAP_CUCKOO
#define BENCHMARK_DENSEHASHMAP_CUCKOO

#include "../../util/sparsehash-c11/sparsehash/dense_hash_map"

template <typename T>
class BenchmarkDenseHashMap {

	public:
		const int NUM_READERS = 24;
		const int NUM_WRITERS = 8;

		BenchmarkDenseHashMap(
			int num_ops=2*1000*1000,
			float space_efficiency=0.9f);
		~BenchmarkDenseHashMap();

		void benchmark_random_interleaved_read_write();
		void benchmark_read_only();
		void benchmark_write_only();
		void benchmark_read_only_single_bucket();
		void benchmark_space_efficiency();
		void run_all();

	private:
		int m_num_ops;
		float m_space_efficiency;
		int m_num_buckets;
		std::string* m_random_keys;

		double m_benchmark_reads_helper(google::dense_hash_map<T, T>* my_map, float read_percentage=1.0f, float opts_percentage=1.0f);

};

#include "benchmark_dense_hashmap.cpp"

#endif

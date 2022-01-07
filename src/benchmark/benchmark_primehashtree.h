#ifndef BENCHMARK_PRIME_HASHTREE
#define BENCHMARK_PRIME_HASHTREE

#include "../prime_hash_tree.h"

template <typename T>
class BenchmarkPrimeHashTree {
	public:
		const int NUM_READERS = 24;
		const int NUM_WRITERS = 8;

		BenchmarkPrimeHashTree (
			int num_ops=2*1000*1000,
			float space_efficiency=0.9f);
		~BenchmarkPrimeHashTree();

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

		double m_benchmark_reads_helper(PrimeHashTree<T, T>* my_map, float read_percentage=1.0f, float opts_percentage=1.0f);

};

#include "benchmark_primehashtree.cpp"

#endif

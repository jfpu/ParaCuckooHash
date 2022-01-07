#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <random>

#include "../common/CycleTimer.h"
#include "../common/errors.h"
// #include "thread_send_requests.h"

template <typename T>
BenchmarkPrimeHashTree<T>::BenchmarkPrimeHashTree(
		int num_ops, float space_efficiency) {

	m_num_ops = num_ops;
	m_space_efficiency = space_efficiency;
	m_num_buckets = (1.0f/m_space_efficiency) * m_num_ops;

	m_random_keys = new std::string[m_num_ops];
	for (int i=0; i<m_num_ops; i++) {
		m_random_keys[i] = std::to_string(rand());
	}
}

template <typename T>
BenchmarkPrimeHashTree<T>::~BenchmarkPrimeHashTree() {

	delete[] m_random_keys;
}

template <typename T>
void BenchmarkPrimeHashTree<T>::benchmark_random_interleaved_read_write() {

  //std::cout << "TODO: Interleaved Reads / Writes" << std::endl;

  /*
  	for (float space_efficiency = 0.15f; space_efficiency <= 0.9f; space_efficiency += 0.15f) {
		int num_buckets = (1.0f/space_efficiency) * m_num_ops;

	    for (float read_percentage = 0.0f ; read_percentage <= 1.0f; read_percentage += 0.10f){
			    PrimeHashTree<T, T> my_map(num_buckets);
					// PrimeHashTree<T, T> my_map(1023487);
          my_map.reserve(num_buckets);
          double best_time = m_benchmark_reads_helper(&my_map, read_percentage);
          std::cout << "\t Interleaved case: " << 100*space_efficiency << "% Space Efficiency (" << NUM_READERS
                    << " Reader Threads), Read Percentage " << read_percentage*100 << "%: "
                    << m_num_ops / best_time / (1000 * 1000) << std::endl;
        }
	}
  */

  for (float space_efficiency = 0.15f; space_efficiency <= 0.9f; space_efficiency += 0.15f) {
    int num_buckets = (1.0f/space_efficiency) * m_num_ops;

    for (float read_percentage = 0.80f ; read_percentage <= 1.0f; read_percentage += 0.025f){
			PrimeHashTree<T, T> my_map(num_buckets);
			// PrimeHashTree<T, T> my_map(1023487);

      for (float prepopulate_percentage = 0.85f; prepopulate_percentage <= 0.95f ; prepopulate_percentage += 0.05f){


        if (prepopulate_percentage == 1.0f){

          double best_time = m_benchmark_reads_helper(&my_map, read_percentage);

          std::cout << "\t Interleaved case: " << 100*space_efficiency << "% Space Efficiency (" << NUM_READERS
                    << " Reader Threads), Read Percentage " << read_percentage*100 << "%, Prepopulate percentage: " << prepopulate_percentage * 100 << "% : "
                    << m_num_ops / best_time / (1000 * 1000) << std::endl;
					std::cout << "\t OverflowRate: " << my_map.overflow_rate() << ", (" << my_map.overflow_nums() << ", " << my_map.size() << ")" << std::endl;
        }
        else{
          // Prepopulate hash map
          m_benchmark_reads_helper(&my_map, 0, prepopulate_percentage);

          double best_time = m_benchmark_reads_helper(&my_map, read_percentage, 1-prepopulate_percentage);

          std::cout << "\t Interleaved case: " << 100*space_efficiency << "% Space Efficiency (" << NUM_READERS
                    << " Reader Threads), Read Percentage " << read_percentage*100 << "%, Prepopulate percentage: " << prepopulate_percentage * 100 << "% : "
                    << m_num_ops * (1.0f-prepopulate_percentage) / best_time / (1000 * 1000) << std::endl;
					std::cout << "\t OverflowRate: " << my_map.overflow_rate() << ", (" << my_map.overflow_nums() << ", " << my_map.size() << ")" << std::endl;
        }
      }
    }
  }


}

template <typename T>
void BenchmarkPrimeHashTree<T>::benchmark_read_only() {
	PrimeHashTree<T, T> my_map(m_num_buckets);
	// PrimeHashTree<T, T> my_map(1023487);

    double best_time = m_benchmark_reads_helper(&my_map);

    std::cout << "\t" << "Read-Only (" << NUM_READERS << " Reader Threads): "
              << m_num_ops / best_time / (1000 * 1000) << std::endl;
		std::cout << "\t OverflowRate: " << my_map.overflow_rate() << ", (" << my_map.overflow_nums() << ", " << my_map.size() << ")" << std::endl;
		// my_map.print();
}

template <typename T>
void BenchmarkPrimeHashTree<T>::benchmark_write_only() {

	double start_time, end_time, best_time;
	int num_buckets = (1.0f/0.25f) * m_num_ops;

    best_time = 1e30;
    for (int i = 0; i < 10; i++) {
	      PrimeHashTree<T, T> my_map(num_buckets);
				// PrimeHashTree<T, T> my_map(1023487);


        start_time = CycleTimer::currentSeconds();
        for (int j = 0; j < m_num_ops; j++) {
            my_map.insert(m_random_keys[j], m_random_keys[j]);
        }
        end_time = CycleTimer::currentSeconds();
        best_time = std::min(best_time, end_time-start_time);
				std::cout << "\t OverflowRate: " << my_map.overflow_rate() << ", (" << my_map.overflow_nums() << ", " << my_map.size() << ")" << std::endl;
    }
    std::cout << "\t" << "Write-Only: " << m_num_ops / best_time / (1000 * 1000) << std::endl;
}

template <typename T>
void BenchmarkPrimeHashTree<T>::benchmark_read_only_single_bucket() {

	double start_time, end_time, best_time;

	// Warm up the hashmap with sequential insertions.
	PrimeHashTree<T, T> my_map(m_num_buckets);
	// PrimeHashTree<T, T> my_map(1023487);

	for (int i = 0; i < m_num_ops; i++) {
		my_map.insert(m_random_keys[i], m_random_keys[i]);
	}

	// Concurrently read the same key.
	std::string* identical_keys = new std::string[m_num_ops];
	for (int i = 0; i < m_num_ops; i++) {
		identical_keys[i] = m_random_keys[0];
	}

    for (int num_readers = 1; num_readers <= NUM_READERS; num_readers += 4) {

		// Setup thread args.
	    pthread_t workers[num_readers];
	    WorkerArgs args[num_readers];

	    for (int i = 0; i < num_readers; i++) {
	        args[i].my_map = (void*)&my_map;
	        args[i].thread_id = (long int)i;
	        args[i].num_threads = num_readers;
	        args[i].num_ops = m_num_ops;
	        args[i].percent_reads = 1.0f;
	        args[i].keys = identical_keys;
	    }

	    best_time = 1e30;
	    for (int i = 0; i < 10; i++) {
	        start_time = CycleTimer::currentSeconds();
		    for (int j = 0; j < num_readers; j++) {
		        pthread_create(&workers[j], NULL,
		        	thread_send_requests_for_insert_find<PrimeHashTree<T, T>>, &args[j]);
		    }
		    for (int j = 0; j < num_readers; j++) {
		        pthread_join(workers[j], NULL);
		    }
	        end_time = CycleTimer::currentSeconds();
	        best_time = std::min(best_time, (end_time-start_time)/num_readers);
	    }
	    std::cout << "\t" << "Read-Only Single Bucket: (" << num_readers << " Reader Threads): " << m_num_ops / best_time / (1000 * 1000) << std::endl;
			std::cout << "\t OverflowRate: " << my_map.overflow_rate() << ", (" << my_map.overflow_nums() << ", " << my_map.size() << ")" << std::endl;
	}
	delete[] identical_keys;
}

template <typename T>
void BenchmarkPrimeHashTree<T>::benchmark_space_efficiency() {

	for (float space_efficiency = 0.15f; space_efficiency <= 0.9f; space_efficiency += 0.15f) {
		int num_buckets = (1.0f/space_efficiency) * m_num_ops;
	  PrimeHashTree<T, T> my_map(num_buckets);
		// PrimeHashTree<T, T> my_map(1023487);

		double best_time = m_benchmark_reads_helper(&my_map);

	    std::cout << "\t" << 100*space_efficiency << "% Space Efficiency: (" << NUM_READERS << " Reader Threads): " << m_num_ops / best_time / (1000 * 1000) << std::endl;
	}
}

template <typename T>
double BenchmarkPrimeHashTree<T>::m_benchmark_reads_helper(
       PrimeHashTree<T, T>* my_map, float read_percentage, float ops_percentage) {

	// Warm up the hashmap.
	for (int i = 0; i < m_num_ops; i++) {
		my_map->insert(m_random_keys[i], m_random_keys[i]);
	}

	// Setup thread args.
    pthread_t workers[NUM_READERS];
    WorkerArgs args[NUM_READERS];

    for (int i = 0; i < NUM_READERS; i++) {
        args[i].my_map = (void*)my_map;
        args[i].thread_id = (long int)i;
        args[i].num_threads = NUM_READERS;
        args[i].num_ops = m_num_ops;
        args[i].percent_reads = read_percentage;
        args[i].keys = m_random_keys;
        args[i].ops_percentage = ops_percentage;
    }

	double start_time, end_time, best_time;

    best_time = 1e30;
    for (int i = 0; i < 5; i++) {
        start_time = CycleTimer::currentSeconds();
	    for (int j = 0; j < NUM_READERS; j++) {
	        pthread_create(&workers[j], NULL,
	        	thread_send_requests_for_insert_find<PrimeHashTree<T, T>>, &args[j]);
	    }
	    for (int j = 0; j < NUM_READERS; j++) {
	        pthread_join(workers[j], NULL);
	    }
        end_time = CycleTimer::currentSeconds();
        best_time = std::min(best_time, end_time-start_time);
        // std::cout << "\t" << end_time-start_time << std::endl;
    }

	return best_time;
}

template <typename T>
void BenchmarkPrimeHashTree<T>::run_all() {

	std::cout << "Benchmarking PrimeHashTree, " << m_num_ops << " Operations..." << std::endl;

/*
  {
	  PrimeHashTree<T, T> my_map(1);
	  my_map.print();
	}
  {
	  PrimeHashTree<T, T> my_map(m_num_buckets, 0.9f);
	  my_map.print();
	}
  {
	  PrimeHashTree<T, T> my_map(102378, 0.9f);
	  my_map.print();
	}
  {
		const uint32_t nums = 19800000;
	  PrimeHashTree<uint32_t, int> my_map(nums);
	  my_map.print();
		for (int n = 1; n <= nums; ++n) {
		  my_map.put(n, n+1);
		  assert ((n+1) == my_map.get(n));
		}
	  my_map.print();
	}
*/
  {
		std::random_device                 rd;
    std::mt19937                       mt(rd());
    std::uniform_int_distribution<uint64_t> rng;

		const uint32_t nums = 19800000;
	  PrimeHashTree<uint64_t, uint64_t> my_map(nums);
	  my_map.print();
		const size_t check_nums = my_map.size() * 0.9f;
		for (size_t n = 1; n <= check_nums; ++n) {
			uint64_t key = rng(mt);
		  if ((key+1) == my_map.get(key)) continue;
		  my_map.put(key, key+1);
		  assert ((key+1) == my_map.get(key));
		}
	  my_map.print();
	}

	benchmark_read_only();

	benchmark_read_only();
	benchmark_write_only();
	benchmark_read_only_single_bucket();
	benchmark_space_efficiency();
/*
*/

	// benchmark_random_interleaved_read_write();

    std::cout << std::endl;
}

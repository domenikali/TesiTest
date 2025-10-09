#include "benchmark.hpp"
#include "bttd.hpp"
#include "matrix_multisec.hpp"
#include "matrix.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <numeric>
#include <functional>

// Helper to generate a random integer array for sectors/layers
void generate_random_indices(int* arr, int count, int max_val) {
    std::vector<int> all_indices(max_val);
    std::iota(all_indices.begin(), all_indices.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all_indices.begin(), all_indices.end(), g);

    for (int i = 0; i < count; ++i) {
        arr[i] = all_indices[i];
    }
    arr[count] = -1; // Terminator
}

// Helper to generate a fixed integer array for sectors/layers
void generate_fixed_indices(int* arr, int count) {
    for (int i = 0; i < count; ++i) {
        arr[i] = i;
    }
    arr[count] = -1; // Terminator
}

void run_benchmark(
    const std::string& case_name,
    const std::vector<std::function<void(pcm_size_t*, input_size_t*, volatile int*, int*, int64_t*)>>& algos,
    const std::vector<std::string>& algo_names,
    int sector_count, int layer_count, bool random_indices) {

    const int runs = 5;
    std::cout << "Running benchmark for case: " << case_name << std::endl;
    size_t matrix_size=512*512*8;
    size_t n_layers=8;
    // Create a single random matrix and vector for all runs in this case
    pcm_size_t* matrix = new pcm_size_t[matrix_size];
    input_size_t* vector = new input_size_t[max_x];
    int64_t* result = new int64_t[512];
    
    

    int sectors[n_sectors + 1];
    volatile int layers[n_layers + 1];


    std::vector<std::ofstream> outfiles;
    outfiles.reserve(algos.size());

    for (size_t i = 0; i < algos.size(); ++i) {
        std::string filename = "./new_mvm_tests_20/" + algo_names[i] + "_" + case_name + ".csv";
        outfiles.emplace_back(filename);
        if (!outfiles.back().is_open()) {
            std::cerr << "Failed to open " << filename << '\n';
            std::exit(EXIT_FAILURE);
        }
        outfiles.back() << "duration_ns\n"; // Header
    }

    for (int j = 0; j < runs; ++j) {
        std::mt19937 gen(time(NULL)); 
        std::uniform_int_distribution<int> pcm_dist(0, 15);
        std::uniform_int_distribution<int> vec_dist(-128, 127);
        generate_fixed_indices(sectors, sector_count);
        generate_random_indices(const_cast<int*>(layers), layer_count, n_layers);
    

        for (size_t i = 0; i < matrix_size; ++i) matrix[i] = pcm_dist(gen);
        for (int i = 0; i < max_x; ++i) vector[i] = vec_dist(gen);

        for(int i=0;i<algos.size();++i)
        {
         
            auto start = std::chrono::high_resolution_clock::now();
            algos[i](matrix, vector, layers, sectors, result);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            outfiles[i] << duration << "\n";

        }
        
    }
    

    for (size_t i = 0; i < outfiles.size(); ++i) {
        outfiles[i].close();
        std::string filename = "./new_mvm_tests_20/" + algo_names[i] + "_" + case_name + ".csv";
        std::cout << "  Finished: " << algo_names[i] << ", results in " << filename << '\n';
    }

    delete[] matrix;
    delete[] vector;
    delete[] result;
}

void benchmark_mvm_algorithms() {
    std::vector<std::function<void(pcm_size_t*, input_size_t*, volatile int*, int*, int64_t*)>> algorithms = {
        new_mvm, new_mvm_3, new_mvm_4, new_mvm_mtd_4, new_mvm_mtd_8,new_mvm_mtd_16
    };
    std::vector<std::string> algo_names = {
        "new_mvm", "new_mvm_3", "new_mvm_4", "new_mvm_mtd_4", "new_mvm_mtd_8","new_mvm_mtd_16"
    };

    // Define benchmark cases
    run_benchmark("1s_1l", algorithms, algo_names, 1, 1, true);
    run_benchmark("2s_1l", algorithms, algo_names, 2, 2, true);
    run_benchmark("2s_2l", algorithms, algo_names, 2, 2, true);
    run_benchmark("3s_1l", algorithms, algo_names, 3, 2, true);
    run_benchmark("3s_2l", algorithms, algo_names, 3, 2, true);
    run_benchmark("4s_1l", algorithms, algo_names, 4, 1, true);
    run_benchmark("4s_2l", algorithms, algo_names, 4, 2, true);
}
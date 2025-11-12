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
void generate_random_indices(int** arr, int count, int max_val,int*sectors) {
    std::vector<int> all_indices(max_val);
    std::iota(all_indices.begin(), all_indices.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all_indices.begin(), all_indices.end(), g);

    int i=0;
    while(sectors[i]!=-1){
        int s=sectors[i];
        for (int j = 0; j < count; ++j) {
            arr[s][j] = all_indices[j];
        }
        arr[s][count] = -1; // Terminator
        i++;
    }
    
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
    const std::vector<std::function<void(pcm_size_t*, pcm_size_t****, input_size_t*, int**, int*, int64_t*)>>& algos,
    const std::vector<std::string>& algo_names,
    int sector_count, int layer_count, bool random_indices) {

    const int runs = 100;
    std::cout << "Running benchmark for case: " << case_name << std::endl;
    size_t matrix_size=512*512*8;
    size_t n_layers=8;
    // Create both matrix formats
    pcm_size_t* flat_matrix = new pcm_size_t[matrix_size];
    input_size_t* vector = new input_size_t[max_x];
    int64_t* result = new int64_t[512];
    
    // Allocate 4D matrix for sslow_mvm
    pcm_size_t**** matrix_4d = new pcm_size_t***[n_sectors];
    for(int s=0;s<n_sectors;++s){
        matrix_4d[s]=new pcm_size_t**[n_layers];
        for(int l=0;l<n_layers;++l){
            matrix_4d[s][l]=new pcm_size_t*[tile_size];
            for(int y=0;y<tile_size;++y){
                matrix_4d[s][l][y]=new pcm_size_t[max_x];
            }
        }
    }

    int sectors[n_sectors + 1];
    int **layers=new int*[n_sectors+1];
    for(int i=0;i<n_sectors;++i){
        layers[i]=new int[n_layers+1];
    }


    std::vector<std::ofstream> outfiles;
    outfiles.reserve(algos.size());

    for (size_t i = 0; i < algos.size(); ++i) {
        std::string filename = "./new_mvm_full_o/" + algo_names[i] + "_" + case_name + ".csv";
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
        generate_random_indices(layers, layer_count, n_layers,sectors);
    
        // Populate both matrices with the same random data
        for(int s=0; s<n_sectors; ++s) {
            for(int l=0; l<n_layers; ++l) {
                for(int y=0; y<tile_size; ++y) {
                    for(int x=0; x<max_x; ++x) {
                        pcm_size_t val = pcm_dist(gen);
                        matrix_4d[s][l][y][x] = val;
                        flat_matrix[m_inedx(s,l,y,x)] = val;
                    }
                }
            }
        }

        for (int i = 0; i < max_x; ++i) vector[i] = vec_dist(gen);

        for(size_t i=0;i<algos.size();++i)
        {
         
            auto start = std::chrono::high_resolution_clock::now();
            algos[i](flat_matrix, matrix_4d, vector, layers, sectors, result);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            outfiles[i] << duration << "\n";

        }
        
    }
    
    
    for (size_t i = 0; i < outfiles.size(); ++i) {
        outfiles[i].close();
        std::string filename = "./new_mvm_full_o/" + algo_names[i] + "_" + case_name + ".csv";
        std::cout << "  Finished: " << algo_names[i] << ", results in " << filename << '\n';
    }

    delete[] flat_matrix;
    delete[] vector;
    delete[] result;

    // Deallocate 4D matrix
    for(int s=0;s<n_sectors;++s){
        for(int l=0;l<n_layers;++l){
            for(int y=0; y<tile_size; ++y){
                delete[] matrix_4d[s][l][y];
            }
            delete[] matrix_4d[s][l];
        }
        delete[] matrix_4d[s];
    }
    delete[] matrix_4d;
}

void benchmark_mvm_algorithms() {
    std::vector<std::function<void(pcm_size_t*, pcm_size_t****, input_size_t*, int**, int*, int64_t*)>> algorithms = {
        // Wrapper for sslow_mvm
        [](pcm_size_t* flat, pcm_size_t**** m4d, input_size_t* v, int** l, int* s, int64_t* r){ sslow_mvm(m4d, v, l, s, r); },
        // Wrappers for other functions
        [](pcm_size_t* flat, pcm_size_t****, input_size_t* v, int** l, int* s, int64_t* r){ new_mvm(flat, v, l, s, r); },
        [](pcm_size_t* flat, pcm_size_t****, input_size_t* v, int** l, int* s, int64_t* r){ new_mvm_3(flat, v, l, s, r); },
        [](pcm_size_t* flat, pcm_size_t****, input_size_t* v, int** l, int* s, int64_t* r){ new_mvm_4(flat, v, l, s, r); },
        [](pcm_size_t* flat, pcm_size_t****, input_size_t* v, int** l, int* s, int64_t* r){ new_mvm_mtd_4(flat, v, l, s, r); },
        [](pcm_size_t* flat, pcm_size_t****, input_size_t* v, int** l, int* s, int64_t* r){ new_mvm_mtd_8(flat, v, l, s, r); },
        [](pcm_size_t* flat, pcm_size_t****, input_size_t* v, int** l, int* s, int64_t* r){ new_mvm_mtd_16(flat, v, l, s, r); }
    };
    std::vector<std::string> algo_names = {
        "Original_Naive", "Naive_Flat_Buffer", "Flat_Buffer_Cycle_Opt", "Flat_Buffer_Cycle_Opt_Reg_Opt", "4_Threads", "8_Threads","16_Threads"
    };

    // Define benchmark cases
    run_benchmark("1Sector_1Layer", algorithms, algo_names, 1, 1, true);
    run_benchmark("2Sector_1Layer", algorithms, algo_names, 2, 2, true);
    run_benchmark("2Sector_2Layer", algorithms, algo_names, 2, 2, true);
    run_benchmark("3Sector_1Layer", algorithms, algo_names, 3, 2, true);
    run_benchmark("3Sector_2Layer", algorithms, algo_names, 3, 2, true);
    run_benchmark("4Sector_1Layer", algorithms, algo_names, 4, 1, true);
    run_benchmark("4Sector_2Layer", algorithms, algo_names, 4, 2, true);
}

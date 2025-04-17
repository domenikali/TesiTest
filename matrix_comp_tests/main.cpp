#include <stdlib.h> 
#include <iostream>
#include <string>
#include <chrono>
#include <cstdint>
#include "../logger.hpp" 
#include "matrix.hpp"
#include <chrono>
#include <thread>
#include <filesystem>
#include "matrix_multisec.hpp"

#define EXT_ERROR_CHECKS 0
#define TIMES 500



unsigned int test_number =0;


std::string pretty_vector_64(int64_t * vector, int size){
  std::string result = "[";
  for(int i=0;i<size;i++){
    result += std::to_string(vector[i]);
    if(i != size-1){
      result += ", ";
    }
  }
  result += "]";
  return result;
}

std::string pretty_vector_8(int8_t * vector, int size){
  std::string result = "[";
  for(int i=0;i<size;i++){
    result += std::to_string(vector[i]);
    if(i != size-1){
      result += ", ";
    }
  }
  result += "]";
  return result;
}


  
//tests singnatures:
//mtd: multithreaded
//std: standard
void mtd_mvm_test(int8_t ***** matrix, int8_t *vector, int sector);
void std_mvm_test(int8_t ***** matrix, int8_t * vector, int sector);
void mtd_mvm_2_test(int8_t ***** matrix, int8_t *vector, int sector);
void std_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector);
void mtd_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector);
void mtd_mvm_2_time_test(int8_t ***** matrix, int8_t *vector, int sector);
void mtd_mvm_2_1_test(int8_t ***** matrix, int8_t *vector, int sector);
void comparison_test(int8_t ***** matrix, int8_t * vector, int sector);
void data(int8_t ***** matrix, int8_t * vector);
void flat_data(int8_t* matrix,int8_t * vector);





Logger logger("logs.txt");

int main(int args,char ** argv){

  int8_t * vector = new int8_t[max_vect];
  std::cout<<"befre alloc: " << std::endl;
  int8_t * f = flat(); 
	  
  flat_data(f,vector);
  
  delete[] vector;
  delete[] f;



  logger.log(LogLevel::INFO, "Matrix Vector Multiplication Tests Completed");
  return 0;
}

/**@simple mvm test
 * make a computation, count the time elaped during the computation then create logs file for input vector, matrix and result readable from an external matrix script to check the result accuracy
 * then log everything on the txt log file 
 * lastly delalocate the result vector
 * @param matrix the matrix to be used for the computation
 * @param vector the input vector to be used for the computation
 * @param sector the sector of the matrix to be used for the computation
 * TODO: make the sectors indipendant 
 */
void std_mvm_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;
  logger.log(LogLevel::INFO, "Test: Standard MVM Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void mtd_mvm_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;
  logger.log(LogLevel::INFO, "Test: Multithreade MVM Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm_512_t(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

 
  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void mtd_mvm_2_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreade MVM 2.0 Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm_4_t(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  
  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void std_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Standard MVM Time Test");
  
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
  for(int i=0;i<TIMES;i++){
    random_matrix(matrix);
    random_vector(vector);
    auto start = std::chrono::high_resolution_clock::now();

    int64_t * result = mvm(matrix, vector,sector);
    
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    delete[] result;

  }


  logger.log(LogLevel::INFO, "Time taken to compelte " +std::to_string(TIMES) +" mvm with std algo: " + std::to_string(elapsed.count()/TIMES) + " seconds");
  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  
}

void mtd_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreader MVM Time Test");
  
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
  for(int i=0;i<TIMES;i++){
    random_matrix(matrix);
    random_vector(vector);
    auto start = std::chrono::high_resolution_clock::now();

    int64_t * result = mvm_512_t(matrix, vector,sector);
    
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    delete[] result;

  }
  
  logger.log(LogLevel::INFO, "Time taken to compelte " +std::to_string(TIMES) +" mvm with mtd algo: " + std::to_string(elapsed.count()/TIMES) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  
}

void mtd_mvm_2_time_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreader MVM 2.0 Time Test");
  
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
  for(int i=0;i<TIMES;i++){
    random_matrix(matrix);
    random_vector(vector);
    auto start = std::chrono::high_resolution_clock::now();

    int64_t * result = mvm_4_t(matrix, vector,sector);
    
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    delete[] result;
    
  }
  

  
  logger.log(LogLevel::INFO, "Time taken to compelte " +std::to_string(TIMES) +" mvm with mtd algo: " + std::to_string(elapsed.count()/TIMES) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  
}

void mtd_mvm_2_1_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreade MVM 2.1 Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm_8_t(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  
  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void comparison_test(int8_t ***** matrix, int8_t * vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Comparison Test");

  random_matrix(matrix);
  random_vector(vector);

  auto start = std::chrono::high_resolution_clock::now();
  int64_t * result = mvm(matrix, vector,sector);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  logger.log(LogLevel::INFO, "Time taken for standard mvm: " + std::to_string(elapsed.count()) + " seconds");
  auto start_1 = std::chrono::high_resolution_clock::now();
  int64_t * result_2 = mvm_512_t(matrix, vector,sector);
  auto end_1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_1 = end_1 - start_1;
  logger.log(LogLevel::INFO, "Time taken for multithreaded mvm: " + std::to_string(elapsed_1.count()) + " seconds");
  auto start_2 = std::chrono::high_resolution_clock::now();
  int64_t * result_3 = mvm_4_t(matrix, vector,sector);
  auto end_2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_2 = end_2 - start_2;
  logger.log(LogLevel::INFO, "Time taken for multithreaded mvm 2.0: " + std::to_string(elapsed_2.count()) + " seconds");

  auto start_3 = std::chrono::high_resolution_clock::now();
  int64_t * result_4 = mvm_8_t(matrix, vector,sector);
  auto end_3 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_3 = end_3 - start_3;
  logger.log(LogLevel::INFO, "Time taken for multithreaded mvm 2.1: " + std::to_string(elapsed_3.count()) + " seconds");


  std::cout << "Test "<<test_number<<" Competed" << std::endl;

  delete[] result;
  delete[] result_2;
  delete[] result_3;
  delete[] result_4;

}

void data(int8_t ***** matrix, int8_t * vector){
  unsigned int n_thread = std::thread::hardware_concurrency();
  std::string path = "core_"+std::to_string(n_thread);
  std::filesystem::create_directory(path.c_str());
  FILE * std = fopen((path+"/"+"std_mvm.txt").c_str(), "w+");
  FILE * mtd_4 = fopen((path+"/" +"mtd_4_mvm.txt").c_str(), "w+");
  FILE * mtd_8 = fopen((path+"/" + "mtd_8_mvm.txt").c_str(), "w+");
  FILE * mtd_16 = fopen((path+"/" +"mtd_16_mvm.txt").c_str(), "w+");
  FILE * mtd_20 = fopen((path+"/" +"mtd_20_mvm.txt").c_str(),"w+");
  FILE * mtd_32 = fopen((path+"/" +"mtd_32_mvm.txt").c_str(),"w+");
  FILE * mtd_512 = fopen((path+"/" +"mtd_512_mvm.txt").c_str(),"w+");
  int sector =0;
  std::string s;

  for(int i=0;i<TIMES;i++){
    std::cout<<"Running test " << i << std::endl;
    random_matrix(matrix);
    random_vector(vector);

    auto start = std::chrono::high_resolution_clock::now();
    int64_t * result = mvm(matrix, vector,sector);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    delete[] result;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), std);
    fflush(std);  

    auto start_1 = std::chrono::high_resolution_clock::now();
    int64_t * result_2 = mvm_512_t(matrix, vector,sector);
    auto end_1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_1 = end_1 - start_1;
    delete[] result_2;
    s = std::to_string(elapsed_1.count())+"\n";
    fputs(s.c_str(), mtd_512);
    fflush(mtd_512);
    
    auto start_2 = std::chrono::high_resolution_clock::now();
    int64_t * result_3 = mvm_4_t(matrix, vector,sector);
    auto end_2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_2 = end_2 - start_2;
    delete[] result_3;
    s = std::to_string(elapsed_2.count())+"\n";
    fputs(s.c_str(), mtd_4);
    fflush(mtd_4);

    
    auto start_3 = std::chrono::high_resolution_clock::now();
    int64_t * result_4 = mvm_8_t(matrix, vector,sector);
    auto end_3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_3 = end_3 - start_3;
    delete[] result_4;
    s = std::to_string(elapsed_3.count())+"\n";
    fputs(s.c_str(), mtd_8);
    fflush(mtd_8);  

    auto start_4 = std::chrono::high_resolution_clock::now();
    int64_t * result_5 = mvm_16_t(matrix, vector,sector);
    auto end_4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_4 = end_4 - start_4;
    delete[] result_5;
    s = std::to_string(elapsed_4.count())+"\n";
    fputs(s.c_str(), mtd_16);
    fflush(mtd_16);
    
    auto start_5 = std::chrono::high_resolution_clock::now();
    int64_t * result_6 = mvm_20_t(matrix, vector,sector);
    auto end_5 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_5 = end_5 - start_5;
    delete[] result_6;
    s = std::to_string(elapsed_5.count())+"\n";
    fputs(s.c_str(), mtd_20);
    fflush(mtd_20); 

    auto start_6 = std::chrono::high_resolution_clock::now();
    int64_t * result_7 = mvm_32_t(matrix, vector,sector);
    auto end_6 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_6 = end_6 - start_6;
    delete[] result_7;
    s = std::to_string(elapsed_6.count())+"\n";
    fputs(s.c_str(), mtd_32);
    fflush(mtd_32);
    
  }

  fclose(std);
  fclose(mtd_4);
  fclose(mtd_8);
  fclose(mtd_16);
  fclose(mtd_20);
  fclose(mtd_32);
  fclose(mtd_512);

}


void flat_data(int8_t* matrix,int8_t * vector){
  unsigned int n_thread = std::thread::hardware_concurrency();
  std::string path = "flat_core_"+std::to_string(n_thread);
  std::filesystem::create_directory(path.c_str());
  FILE * std = fopen((path+"/"+"std_mvm.txt").c_str(), "w+");
  FILE * mtd_4 = fopen((path+"/" +"mtd_4_mvm.txt").c_str(), "w+");
  FILE * mtd_8 = fopen((path+"/" + "mtd_8_mvm.txt").c_str(), "w+");
  FILE * mtd_16 = fopen((path+"/" +"mtd_16_mvm.txt").c_str(), "w+");
  FILE * mtd_32 = fopen((path+"/" +"mtd_32_mvm.txt").c_str(),"w+");
  
  

  int ** sector = new int*[n_sectors];
  for(int i=0;i<n_sectors;i++){
    sector[i] = new int[5];
    sector[i][0] = 0;
    sector[i][1] = 1;
    sector[i][2] = 2;
    sector[i][3] = 3;
    sector[i][4] = -1;
  }

  std::string s;

  for(int i=0;i<TIMES;i++){
    std::cout<<"Running test " << i << std::endl;
    random_flat(matrix);
    random_vector(vector);

    auto start = std::chrono::high_resolution_clock::now();
    int64_t * result = flat_mvm(matrix, vector,sector);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    delete[] result;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), std);
    fflush(std);  

    
    
    auto start_2 = std::chrono::high_resolution_clock::now();
    int64_t * result_3 = flat_4t(matrix, vector,sector);
    auto end_2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_2 = end_2 - start_2;
    delete[] result_3;
    s = std::to_string(elapsed_2.count())+"\n";
    fputs(s.c_str(), mtd_4);
    fflush(mtd_4);

    
    auto start_3 = std::chrono::high_resolution_clock::now();
    int64_t * result_4 = flat_8t(matrix, vector,sector);
    auto end_3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_3 = end_3 - start_3;
    delete[] result_4;
    s = std::to_string(elapsed_3.count())+"\n";
    fputs(s.c_str(), mtd_8);
    fflush(mtd_8);  

    auto start_4 = std::chrono::high_resolution_clock::now();
    int64_t * result_5 = flat_16t(matrix, vector,sector);
    auto end_4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_4 = end_4 - start_4;
    delete[] result_5;
    s = std::to_string(elapsed_4.count())+"\n";
    fputs(s.c_str(), mtd_16);
    fflush(mtd_16);
    
     

    auto start_6 = std::chrono::high_resolution_clock::now();
    int64_t * result_7 = flat_32t(matrix, vector,sector);
    auto end_6 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_6 = end_6 - start_6;
    delete[] result_7;
    s = std::to_string(elapsed_6.count())+"\n";
    fputs(s.c_str(), mtd_32);
    fflush(mtd_32);
    
  }

  fclose(std);
  fclose(mtd_4);
  fclose(mtd_8);
  fclose(mtd_16);
  fclose(mtd_32);

}






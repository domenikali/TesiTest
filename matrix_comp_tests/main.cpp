#include <stdlib.h> 
#include <iostream>
#include <string.h>
#include "../logger.hpp" 
#include "matrix.hpp"
#include <chrono>





std::string preatty_vector_64(int64_t * vector, int size){
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

std::string preatty_vector_8(int8_t * vector, int size){
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
bool tes1(int8_t ***** matrix, int8_t * vector);
bool timeTest(int8_t ***** matrix, int8_t * vector,int8_t init);
bool mulitTimeTest(int8_t ***** matrix, int8_t * vector);
bool mulitCellTimeTest(int8_t ***** matrix, int8_t * vector);
bool multiCellTest(int8_t ***** matrix, int8_t * vector);


Logger log("logs.txt");

int main(int args,char ** argv){

  log.init();

  log.log(LogLevel::INFO, "Matrix Vector Multiplication Tests");

  int8_t ***** matrix = alloc_matrix();
  int8_t * vector = new int8_t[max_vect];

  log.log(LogLevel::INFO, "Matrix and Vector allocated");

  std::cout<<"Test 1 running.."<<std::endl;
  bool res = tes1(matrix, vector);
  std::cout << "Test 1 completed:"<< std::string(res ? "Passed" : "Failed") << std::endl;
  log.log(LogLevel::INFO, "Test 1 completed: " + std::string(res ? "Passed" : "Failed"));

  /*
  std::cout<<"Test 2 running.."<<std::endl;
  bool res2 = timeTest(matrix, vector,1);
  std::cout << "Test 2 completed:"<< std::string(res2 ? "Passed" : "Failed") << std::endl;
  log.log(LogLevel::INFO, "Test 2 completed: " + std::string(res2 ? "Passed" : "Failed"));

  std::cout<<"Test 3 running.."<<std::endl;
  bool res3 = timeTest(matrix, vector,100);
  std::cout << "Test 3 completed:"<< std::string(res2 ? "Passed" : "Failed") << std::endl;
  log.log(LogLevel::INFO, "Test 3 completed: " + std::string(res2 ? "Passed" : "Failed"));

  std::cout<<"Test 4 running.."<<std::endl;
  bool res4 = mulitTimeTest(matrix, vector);
  std::cout << "Test 4 completed:"<< std::string(res3 ? "Passed" : "Failed") << std::endl;
  log.log(LogLevel::INFO, "Test 4 completed: " + std::string(res3 ? "Passed" : "Failed"));

  std::cout<<"Test 5 running.."<<std::endl;
  bool res5 = multiCellTest(matrix, vector);
  std::cout << "Test 5 completed:"<< std::string(res5 ? "Passed" : "Failed") << std::endl;
  log.log(LogLevel::INFO, "Test 5 completed: " + std::string(res5 ? "Passed" : "Failed"));

  std::cout<<"Test 6 running.."<<std::endl;
  bool res6 = mulitCellTimeTest(matrix, vector);
  std::cout << "Test 6 completed:"<< std::string(res6 ? "Passed" : "Failed") << std::endl;
  log.log(LogLevel::INFO, "Test 6 completed: " + std::string(res6 ? "Passed" : "Failed"));
  */

  delete[] vector;
  log.log(LogLevel::INFO, "Matrix Vector Multiplication Tests Completed");
  return 0;
}

// test 1, simple matrix multiplication : 
bool tes1(int8_t ***** matrix, int8_t * vector){
  log.log(LogLevel::INFO, "Test 1: Simple Matrix Vector Multiplication");
  int8_t init = 1;

  init_vecotr(vector, init);
  init_matrix(matrix, init);
  log.log(LogLevel::INFO, "Matrix and Vector initialized");
  int64_t * result = mvm(matrix, vector,0);
  
  log.log(LogLevel::INFO, "vector: " + preatty_vector_8(vector, max_vect));
  log.log(LogLevel::INFO, "Matrix: " + preatty_matrix(matrix));
  log.log(LogLevel::INFO, "result: " + preatty_vector_64(result, max_vect));

  
  for(int i=0;i<max_vect;i++){
    if(result[i] != max_x){
      return false;
    }
  }
  std::cout << "Test 1 passed" << std::endl;
  delete[] result;
  
  return true;

}

bool timeTest(int8_t ***** matrix, int8_t * vector,int8_t init){
  log.log(LogLevel::INFO, "Test 2: Time Test");

  init_vecotr(vector, init);
  init_matrix(matrix, init);

  auto start = std::chrono::high_resolution_clock::now();
  int64_t * result = mvm(matrix, vector,0);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  
  log.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");
  
  delete[] result;
  return true;
}

bool mulitTimeTest(int8_t ***** matrix, int8_t * vector){
  log.log(LogLevel::INFO, "Test 3: Multi Time Test");
  std::chrono::duration<double> elapsed= std::chrono::duration<double>::zero();
  int64_t * result ;
  for(int i=0;i<10000;i++){
    int8_t init = i%100;

    init_vecotr(vector, init);
    init_matrix(matrix, init);
  
    auto start = std::chrono::high_resolution_clock::now();
    result= mvm(matrix, vector,0);
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    
  }
  
  
  log.log(LogLevel::INFO, "Time taken: " + std::to_string((elapsed/10000).count()) + " seconds");
  delete[] result;
  
  return true;
}

bool multiCellTest(int8_t ***** matrix, int8_t * vector){
  log.log(LogLevel::INFO, "Test: Multi Cell Test");
  int8_t init = 1;

  init_vecotr(vector, init);
  init_matrix(matrix, init);
  int64_t * result = mvm(matrix, vector,0);
  //print
  
  //log.log(LogLevel::INFO, "Matrix: " + preatty_mvm(matrix[0],vector,result, max_x, max_y));

  
  for(int i=0;i<max_vect;i++){
    if(result[i] != max_x*init){
      return false;
    }
  }
  std::cout << "multi cell test passed" << std::endl;
  delete[] result;
  
  return true;
}

bool mulitCellTimeTest(int8_t ***** matrix, int8_t * vector){
  log.log(LogLevel::INFO, "Test 5: Multi Cell Time Test");
  std::chrono::duration<double> elapsed= std::chrono::duration<double>::zero();
  int64_t * result ;
  for(int i=0;i<10000;i++){
    int8_t init = i%100;

    init_vecotr(vector, init);
    init_matrix(matrix, init);
  
    auto start = std::chrono::high_resolution_clock::now();
    result= mvm(matrix, vector,0);
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    
  }
  
  
  log.log(LogLevel::INFO, "Time taken: " + std::to_string((elapsed/10000).count()) + " seconds");
  delete[] result;
  
  return true;
}

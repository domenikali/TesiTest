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
#include "bttd.hpp"
#include "benchmark.hpp"
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
void scattered_matrix();
void new_mvm_test();
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
void cache_grind_prf();




double cpuSecond() {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  return ((double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9);
}

Logger logger("logs.txt");

#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <cstdint>

// Function to generate a vector of random int8_t numbers with a sum in the range [-128, 127]
int8_t* generateRandomVectorWithSumConstraint() {
    const int size = 512;
    int8_t* numbers=new int8_t[size];

    // Use a random_device to seed the random number engine
    std::random_device rd;
    // Use the Mersenne Twister engine for high-quality random numbers
    std::mt19937 gen(rd());
    // Distribution for int8_t values
    std::uniform_int_distribution<int16_t> dist(0, 15);

    // 1. Generate 512 random int8_t numbers
    int64_t current_sum = 0;
    for (int i = 0; i < size; ++i) {
        numbers[i] = static_cast<int8_t>(dist(gen));
        current_sum += numbers[i];
    }

    // 2. Choose a random target sum in the range [-128, 127]
    std::uniform_int_distribution<int16_t> target_dist(INT8_MIN, INT8_MAX);
    int8_t target_sum = static_cast<int8_t>(target_dist(gen));

    // 3. Calculate the difference to be distributed
    int64_t difference = current_sum - target_sum;

    // 4. Distribute the difference among the numbers
    // This part is crucial to ensure the final numbers are still well-distributed
    // and within the int8_t range.

    while (difference != 0) {
        // Pick a random index to adjust
        std::uniform_int_distribution<int> index_dist(0, size - 1);
        int index = index_dist(gen);
        if (index < 0 || index >= size) continue; // Safety check

        if (difference > 0) {
            // If the sum is too high, we need to decrease a number
            if (numbers[index] > INT8_MIN) {
                numbers[index]--;
                difference--;
            }
        } else { // difference < 0
            // If the sum is too low, we need to increase a number
            if (numbers[index] < INT8_MAX) {
                numbers[index]++;
                difference++;
            }
        }
    }

    return numbers;
}


int main(int args,char ** argv){

  cache_grind_prf();
  exit(0);
  // benchmark_mvm_algorithms();
  // exit(0);
  // scattered_matrix();
  // new_mvm_test();
  // exit(0); 
  

  uint64_t size;
  input_size_t * vector = new input_size_t[max_vect];
  std::cout<<"befre alloc: " << std::endl;
  pcm_size_t * f = flat(&size); 
  int64_t * result = new int64_t[max_vect];

  // srand(time(nullptr));
  // int sum=0;
  // for (int i = 0; i < 512; ++i) {
  //   int mag = std::rand() % 128;

  //   int sign = (std::rand() % 2) ? 1 : -1;


  //   vector[i] = static_cast<int8_t>(sign * mag);
  //   sum+=vector[i];
  // }

  //vector = generateRandomVectorWithSumConstraint();

  //std::cout << "Generated vector: " << pretty_vector_8(vector, 512) << std::endl;
 
  
  
  //for(long long i=0;i<size;++i){
    // f[i]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
    // f[i]&= 0x0F;
    //f[i]=0;
    ///std::cout<<static_cast<int32_t>(f[i]);
  //
  //}
  int range = INT8_MAX - INT8_MIN + 1;
  srand(time(nullptr));

  pcm_size_t **** matrix = new pcm_size_t***[n_sectors];
  for(int s=0;s<n_sectors;++s){
    matrix[s]=new pcm_size_t**[8];
    for(int l=0;l<8;++l){
      matrix[s][l]=new pcm_size_t*[tile_size];
      for(int y=0;y<tile_size;++y){
        matrix[s][l][y]=new pcm_size_t[max_x];
        for(int x=0;x<max_x;++x){
          matrix[s][l][y][x]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
        }
      }
    }
  }

  int* sector = new int[4+1];
  int**layer =new int*[n_sectors+1];
  for(int i=0;i<n_sectors;++i){
    layer[i]=new int[8+1];
  }

  for(int i=0;i<4;++i){
    sector[i]=i;
    layer[i][0]=0;
    layer[i][1]=7;
    layer[i][2]=-1;
  }
  sector[4]=-1;

  sslow_mvm(matrix,vector,layer,sector,result);




  exit(0);
  
  
  // int sum=0;
  // for(int i=0;i<512;i++){
  //   sum+=vector[i];
  //   for(int j=0;j<128;j++){
      
  //     f[((0*8+0)*128+j)*512+i]=1;
  //   }
  // }



  // std::cout<<"Sum: "<<sum<<std::endl;

  // int* sector = new int[4+1];
  // int**layer =new int*[n_sectors+1];
  // for(int i=0;i<n_sectors;++i){
  //   layer[i]=new int[8+1];
  // }

  // for(int i=0;i<4;++i){
  //   sector[i]=i;
  //   layer[i][0]=0;  
  //   layer[i][1]=1;
  //   layer[i][2]=-1;
  // }
  // sector[1]=-1;

  

  double start = cpuSecond();
  new_mvm_4(f,vector,layer,sector,result);
  double end = cpuSecond();
  std::cout << "Time taken mtd 16: \t" << (end - start) << " seconds" << std::endl;
  print_mvm(f,layer,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);
  exit(0);


  start = cpuSecond();
  new_mvm_mtd_8(f,vector,layer,sector,result);
  end = cpuSecond();
  std::cout << "Time taken mtd 8: \t" << (end - start) << " seconds" << std::endl;
  print_mvm(f,layer,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);


  start = cpuSecond();
  new_mvm_mtd_4(f,vector,layer,sector,result);
  end = cpuSecond();
  std::cout << "Time taken mtd 4: \t" << (end - start) << " seconds" << std::endl;
  print_mvm(f,layer,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  start = cpuSecond();
  new_mvm_4(f,vector,layer,sector,result);
  end = cpuSecond();
  std::cout << "Time taken 4: \t\t" << (end - start) << " seconds" << std::endl;
  print_mvm(f,layer,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

   
  start = cpuSecond();
  new_mvm_3(f,vector,layer,sector,result);
  end = cpuSecond();
  std::cout << "Time taken 3: \t\t" << (end - start) << " seconds" << std::endl;
  print_mvm(f,layer,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  
  start = cpuSecond();
  new_mvm(f,vector,layer,sector,result);
   end = cpuSecond();
  std::cout << "Time taken std: \t" << (end - start) << " seconds" << std::endl;
  print_mvm(f,layer,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  delete[] vector;
  delete[] f;
  delete[] result;
  delete[] sector;
  delete[] layer;

  return 0;
}

void cache_grind_prf(){
  uint64_t size;
  input_size_t * vector = new input_size_t[max_vect];
  std::cout<<"befre alloc: " << std::endl;
  pcm_size_t * f = flat(&size); 
  int64_t * result = new int64_t[max_vect];

  srand(time(nullptr));
  int range = INT8_MAX - INT8_MIN + 1;

  pcm_size_t **** matrix = new pcm_size_t***[n_sectors];
  for(int s=0;s<n_sectors;++s){
    matrix[s]=new pcm_size_t**[8];
    for(int l=0;l<8;++l){
      matrix[s][l]=new pcm_size_t*[tile_size];
      for(int y=0;y<tile_size;++y){
        matrix[s][l][y]=new pcm_size_t[max_x];
        for(int x=0;x<max_x;++x){
          matrix[s][l][y][x]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
        }
      }
    }
  }



  
  for(int i=0;i<512;i++){
    vector[i]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
    
  }
  for(int i=0;i<256;i++){
    result[i]=0;
  }
  for(long long i=0;i<size;++i){
    f[i]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
    ///std::cout<<static_cast<int32_t>(f[i]);
  }

  int* sector = new int[4+1];
  int**layer =new int*[n_sectors+1];
  for(int i=0;i<n_sectors;++i){
    layer[i]=new int[8+1];
  }

  for(int i=0;i<4;++i){
    sector[i]=i;
    layer[i][0]=0;
    layer[i][1]=7;
    layer[i][2]=-1;
  }
  sector[4]=-1;

  new_mvm_mtd_16(f,vector,layer,sector,result);
  new_mvm_mtd_8(f,vector,layer,sector,result);
  new_mvm_mtd_4(f,vector,layer,sector,result);
  new_mvm_4(f,vector,layer,sector,result);
  new_mvm_3(f,vector,layer,sector,result);
  new_mvm(f,vector,layer,sector,result);
  sslow_mvm(matrix,vector,layer,sector,result);


  delete[] vector;
  delete[] f;
  delete[] result;
  delete[] sector;
  delete[] layer;

}

void scattered_matrix(){
  uint64_t size;
  input_size_t * vector = new input_size_t[max_vect];
  std::cout<<"befre alloc: " << std::endl;
  pcm_size_t * f = flat(&size); 
  int64_t * result = new int64_t[max_vect];

  srand(time(nullptr));
  int range = INT8_MAX - INT8_MIN + 1;
  for(int i=0;i<512;i++){
    vector[i]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
    
  }
  for(int i=0;i<256;i++){
    result[i]=0;
  }
  for(long long i=0;i<size;++i){
    f[i]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
    f[i]&= 0x0F;
  }
 
  int* sector = new int[4+1];
  int**layer =new int*[n_sectors+1];
  for(int i=0;i<n_sectors;++i){
    layer[i]=new int[8+1];
  }

  
  sector[0]=0;
  layer[0][0]=0;
  layer[0][1]=-1;
  sector[1]=-1;
  
  new_mvm_mtd_16(f,vector,layer,sector,result);
  print_mvm(f,layer,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);
  

  delete[] vector;
  delete[] f;
  delete[] result;
  delete[] sector;
  delete[] layer;

}

void new_mvm_test(){
  uint64_t size;
  input_size_t * vector = new input_size_t[max_vect];
  std::cout<<"befre alloc: " << std::endl;
  pcm_size_t * f = flat(&size); 
  int64_t * result = new int64_t[max_vect];

  srand(time(nullptr));
  int range = INT8_MAX - INT8_MIN + 1;
  for(int i=0;i<512;i++){
    vector[i]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
    
  }
  for(int i=0;i<256;i++){
    result[i]=0;
  }
  for(long long i=0;i<size;++i){
    f[i]=static_cast<int8_t>(INT8_MIN + (std::rand() % range));
    f[i]&= 0x0F;
    ///std::cout<<static_cast<int32_t>(f[i]);
  }  

  int* sector = new int[4+1];
  int**layer =new int*[n_sectors+1];
  for(int i=0;i<n_sectors;++i){
    layer[i]=new int[8+1];
  }

  for(int i=0;i<4;++i){
    sector[i]=i;
    layer[i][0]=0;
    layer[i][1]=7;
    layer[i][2]=-1;
  }
  sector[4]=-1;
  for(int i=0;i<7;++i){
    layer[0][0]=i;layer[1][0]=i;layer[2][0]=i;layer[3][0]=i;
    new_mvm_mtd_16(f,vector,layer,sector,result);
    print_mvm(f,layer,sector);
    create_vector_conf_file(vector);
    create_result_conf_file(result);
  }

  delete[] vector;
  delete[] f;
  delete[] result;
  delete[] sector;
  delete[] layer;

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


void flat_data(pcm_size_t* matrix,int8_t * vector){
  unsigned int n_thread = std::thread::hardware_concurrency();
  std::string path = "flat_weight_core_"+std::to_string(n_thread);
  std::filesystem::create_directory(path.c_str());
  FILE * std = fopen((path+"/"+"std_mvm.txt").c_str(), "w+");
  FILE * mtd_2 = fopen((path+"/" +"mtd_2_mvm.txt").c_str(),"w+");
  FILE * mtd_4 = fopen((path+"/" +"mtd_4_mvm.txt").c_str(), "w+");
  FILE * mtd_8 = fopen((path+"/" + "mtd_8_mvm.txt").c_str(), "w+");
  FILE * mtd_16 = fopen((path+"/" +"mtd_16_mvm.txt").c_str(), "w+");
  FILE * mtd_32 = fopen((path+"/" +"mtd_32_mvm.txt").c_str(),"w+");
  FILE * mtd_64 = fopen((path+"/" +"mtd_64_mvm.txt").c_str(),"w+");
  FILE * mtd_128 = fopen((path+"/" +"mtd_128_mvm.txt").c_str(),"w+");
  FILE * mtd_512 = fopen((path+"/" +"mtd_512_mvm.txt").c_str(),"w+");



  
  

  int ** sector = new int*[n_sectors];
  for(int i=0;i<n_sectors;i++){
    sector[i] = new int[5];
    sector[i][0] = 0;
    sector[i][1] = 1;
    sector[i][2] = 2;
    sector[i][3] = 3;
    sector[i][4] = -1;
  }
  init_mvm(sector);

  std::string s;
  int64_t * result = new int64_t[max_vect];

  for(int i=0;i<TIMES;i++){
    std::cout<<"Running test " << i << std::endl;
    random_flat(matrix);
    random_vector(vector);

    auto start = std::chrono::high_resolution_clock::now();
    flat_mvm_weight(matrix, vector,sector,result);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), std);
    fflush(std);  

    
    
    start = std::chrono::high_resolution_clock::now();
    flat_4t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
     elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_4);
    fflush(mtd_4);

    
    start = std::chrono::high_resolution_clock::now();
    flat_8t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
     elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_8);
    fflush(mtd_8);  

    start = std::chrono::high_resolution_clock::now();
    flat_16t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
     elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_16);
    fflush(mtd_16);
    
     

    start = std::chrono::high_resolution_clock::now();
    flat_32t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
     elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_32);
    fflush(mtd_32);

    start = std::chrono::high_resolution_clock::now();
    flat_64t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
     elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_64);
    fflush(mtd_64);

    start = std::chrono::high_resolution_clock::now();
    flat_2t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_2);
    fflush(mtd_2);


    start = std::chrono::high_resolution_clock::now();
    flat_128t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_128);
    fflush(mtd_128);


    start = std::chrono::high_resolution_clock::now();
    flat_512t(matrix, vector,sector,result);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    s = std::to_string(elapsed.count())+"\n";
    fputs(s.c_str(), mtd_512);
    fflush(mtd_512);
    
  }

  fclose(std);
  fclose(mtd_2);
  fclose(mtd_4);
  fclose(mtd_8);
  fclose(mtd_16);
  fclose(mtd_32);
  fclose(mtd_64);
  fclose(mtd_128);
  fclose(mtd_512);




}



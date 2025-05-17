#include "matrix.hpp"

unsigned int computations=0;

int8_t***** alloc_matrix(){
  
  int8_t ***** matrix = new int8_t****[n_sectors];

  for(int x=0;x<n_sectors;x++){
    matrix[x] = new int8_t***[tile_per_arry];
    for(int i=0;i<tile_per_arry;i++){
      matrix[x][i]=new int8_t**[tile_size];

      for(int j=0;j<tile_size;j++){
        matrix[x][i][j]=new int8_t*[max_x];

        for(int k=0;k<max_x;k++){
          matrix[x][i][j][k]= new int8_t[nCells];
        }
      }
    }
  }
  
  return matrix;
}



void init_matrix(int8_t***** matrix, int8_t init){
  int range = init;
  range = INT8_MIN;
  for(int x=0;x<n_sectors;x++){
    for(int i=0;i<tile_per_arry;i++){
      for(int j=0;j<tile_size;j++){
        
        for(int k=0;k<max_x;k++){

          for(int l=0;l<nCells;l++){
            
            matrix[x][i][j][k][l] = range;
            if(range == INT8_MAX){
              range = INT8_MIN;
            }
            else{
              range++;
            }
          }

        }
      }
    }
  }
}

void init_vecotr(input_size_t * vector, int8_t init){
  for(int i=0;i<max_vect;i++){
    vector[i] = init;
  }
}

void handle_alloc_error(void *ptr){
  if(ptr == NULL){
    std::cerr << "Memory allocation failed" << std::endl;
    exit(EXIT_FAILURE); 
  }
}

void free_matrix(int8_t***** matrix){
  for(int x=0;x<n_sectors;x++){
    for(int i=0;i<tile_per_arry;i++){
      for(int j=0;j<tile_size;j++){
        for(int k=0;k<max_x;k++){
          delete[] matrix[x][i][j][k];
        }
        
        delete[] matrix[x][i][j];
      }
      delete[] matrix[x][i];
    }
    delete[] matrix[x];
  }
  delete[] matrix;
 
} 



void create_matrix_conf_file(int8_t ***** matrix, int secotr){
  std::string filename ="./result/matrix"+std::to_string(computations)+".conf";
  FILE* fp = fopen(filename.c_str(), "w+");

  if (fp == NULL) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }


  for(int k=0;k<max_x;k++){
    std::string line = "";
    for(int i=0;i<tile_per_arry;i++){
      for(int j=0;j<tile_size;j++){
        for(int y=0;y<nCells;y++){
          line += std::to_string(matrix[secotr][i][j][k][y]);
          if(y != nCells-1){
            line += " ";
          }
        }
        if(j != tile_size-1){
          line += "|";
        }
      }
      if(i != tile_per_arry-1){
        line += "|";
      }
    }
    if(k != max_x-1){
      line += "\n";
    }
    fputs(line.c_str(), fp);
  }

  fclose(fp);
}

void create_vector_conf_file(input_size_t *vector){
  std::string filename ="./result/vector"+std::to_string(computations)+".conf";
  FILE* fp = fopen(filename.c_str(), "w+");

  if (fp == NULL) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  std::string line="";

  for(int i=0;i<max_vect;i++){
    line += std::to_string(vector[i]);
    if(i != max_vect-1){
      line += "|";
    }
  }
  fputs(line.c_str(), fp);
  fclose(fp);
}

void create_result_conf_file(int64_t *result){
  std::string filename ="./result/result"+std::to_string(computations)+".conf";
  FILE* fp = fopen(filename.c_str(), "w+");

  if (fp == NULL) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  std::string line="";

  for(int i=0;i<max_vect;i++){
    line += std::to_string(result[i]);
    if(i != max_vect-1){
      line += "|";
    }
  }
  fputs(line.c_str(), fp);
  fclose(fp);
}




std::string preatty_matrix(int8_t ***** matrix){
  std::string result = "\n";
  for(int y=0;y<n_sectors;y++){
    for(int c=0;c<max_y;c++){
      for(int k=0;k<tile_per_arry;k++){
        for(int j=0;j<tile_size;j++){  
          result+="[";
          for(int i=0;i<nCells;i++){
            result += std::to_string(matrix[y][k][j][c][i]);
            if(i != nCells-1){
              result += ",";
            }
          }
          result+="]";
          if(j != tile_size-1){
            result += ",";
          }
        }
        result += "\t";
      }
      result += "\n";
    }
    result += "\n";
  }
  return result+="\n";
}

void random_matrix(int8_t***** matrix){

  int range = INT8_MAX - INT8_MIN + 1;
  srand(time(nullptr)); 
  for(int x=0;x<n_sectors;x++){
    for(int i=0;i<tile_per_arry;i++){
      for(int j=0;j<tile_size;j++){
        
        for(int k=0;k<max_x;k++){

          for(int l=0;l<nCells;l++){
            matrix[x][i][j][k][l] = static_cast<int8_t>(INT8_MIN + (std::rand() % range));
          }

        }
      }
    }
  }
}
void random_vector(input_size_t * vector){
  int range = INT8_MAX - INT8_MIN + 1;
  srand(time(nullptr)); 
  for(int i=0;i<max_vect;i++){
    vector[i] = static_cast<int8_t>(INT8_MIN + (std::rand() % range));;
  }
}


//////////////////////////////////////////////////////////////////////////////////////
////////                                                                      ////////
////////                Different MVM implementations                         ////////
////////                                                                      ////////
//////////////////////////////////////////////////////////////////////////////////////



int64_t * mvm(int8_t***** matrix, int8_t * vector,int sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);
  //each weight is comosped of the multiplication of each cell at the same xyz position
  //the sum is the weight * the vector in the same x position, with this configuartion 
  


  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<tile_size;j++){

      for(int k=0;k<max_x;k++){
        int64_t weight=1;
        for(int x=0;x<nCells;x++){
          weight*= matrix[sector][i][j][k][x];
        }
        result[i*tile_size+j] += weight * vector[k];
      }
    }
  }

  return result;
}

void compute_colum(int8_t **colum,int8_t * vector, int64_t * result){
  for(int k=0;k<max_x;k++){
    int64_t weight=1;
    for(int x=0;x<nCells;x++){
      weight*= colum[k][x];
    }
    *result+= weight * vector[k];

  }
}

int64_t * mvm_512_t(int8_t***** matrix, int8_t * vector, int sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);
  std::vector<std::thread> threads;


  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<tile_size;j++){
      std::thread t(compute_colum, matrix[sector][i][j], vector, &result[i*tile_size+j]);
      threads.push_back(move(t));
    }
  }
  for(auto &t : threads){
    t.join();
  }

  return result;
}

void compute_tile(int8_t ***** matrix, int8_t * vector, int64_t * result, int sector, int tile){
  for(int j=0;j<tile_size;j++){

    for(int k=0;k<max_x;k++){
      int64_t weight=1;
      for(int x=0;x<nCells;x++){
        weight*= matrix[sector][tile][j][k][x];
      }
      result[tile*tile_size+j] += weight * vector[k];
    }
  }
}

int64_t * mvm_4_t(int8_t***** matrix, int8_t * vector, int sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  std::vector<std::thread> threads;

  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  for(int i=0;i<tile_per_arry;i++){
    std::thread t(compute_tile, matrix, vector, result, sector, i);
    threads.push_back(move(t));
  }
  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}


void compute_tile_max_t(int8_t ***** matrix, int8_t * vector, int64_t * result, int sector, int tile, int i,int inc){
  for(int j=i;j<i+inc;j++){
    for(int k=0;k<max_x;k++){
      int64_t weight=1;
      for(int x=0;x<nCells;x++){
        weight*= matrix[sector][tile][j][k][x];
      }
      result[tile*tile_size+j] += weight * vector[k];
    }
  }
}

int64_t * mvm_8_t(int8_t***** matrix, int8_t * vector, int sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<2;j++){
      std::thread t(compute_tile_max_t, matrix, vector, result, sector, i,j*64, 64);
      threads.push_back(move(t));
    }
  }

  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}


int64_t * mvm_16_t(int8_t***** matrix, int8_t * vector, int sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<4;j++){
      std::thread t(compute_tile_max_t, matrix, vector, result, sector, i,j*32, 32);
      threads.push_back(move(t));
    }
  }

  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}

int64_t * mvm_20_t(int8_t***** matrix, int8_t * vector, int sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<5;j++){
      if(j!=4){
        std::thread t(compute_tile_max_t, matrix, vector, result, sector, i,j*25, 25);
        threads.push_back(move(t));
      }
      else{
        std::thread t(compute_tile_max_t, matrix, vector, result, sector, i,100,28);
        threads.push_back(move(t));
      }
    
    }
  }

  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}

int64_t * mvm_32_t(int8_t***** matrix, int8_t * vector, int sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<8;j++){
      std::thread t(compute_tile_max_t, matrix, vector, result, sector, i,j*16, 16);
      threads.push_back(move(t));    
    }
  }

  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}




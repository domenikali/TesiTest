#include "matrix_multisec.hpp"

//////////////////////////////////////////////////////////////////////////////////////
////////                                                                      ////////
////////                Matrix Multisector                                    ////////
////////                                                                      ////////
//////////////////////////////////////////////////////////////////////////////////////

extern unsigned int computations;
//////////////////////////////////////////////////////////////////////////////////////
////////                                                                      ////////
////////                FLAT IMPL                                             ////////
////////                                                                      ////////
//////////////////////////////////////////////////////////////////////////////////////
int8_t * flat(){
  int8_t *matrix = new int8_t[n_sectors * tile_per_arry * tile_size * max_x * nCells];
  return matrix;
}

void random_flat(int8_t * matrix){
  int range = INT8_MAX - INT8_MIN + 1;
  srand(time(nullptr)); 
  for(int i=0;i<n_sectors * tile_per_arry * tile_size * max_x * nCells;i++){
    matrix[i] = static_cast<int8_t>(INT8_MIN + (std::rand() % range));
  }
}
int64_t * flat_mvm(int8_t * matrix, int8_t * vector,int**sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<tile_size;j++){

      for(int k=0;k<max_x;k++){
        int64_t weight=1;
        int y=0;
        
        while(sector[i][y]>=0){
          for(int x=0;x<nCells;x++){
            int index = (((sector[i][y]*tile_per_arry)+i)*tile_size+j)*max_x+k*nCells+x;
            weight*= matrix[index];
          }
          y++;
        }
        result[i*tile_size+j] += weight * vector[k];
      }
    }
  }

  return result;
}

void compute_flat_4(int8_t * matrix, int8_t * vector, int64_t * result, int i, int * sector){
  for(int j=0;j<tile_size;j++){

    for(int k=0;k<max_x;k++){
      int64_t weight=1;
      int y=0;
      
      while(sector[y]>=0){
        for(int x=0;x<nCells;x++){
          int index = (((sector[y]*tile_per_arry)+i)*tile_size+j)*max_x+k*nCells+x;
          weight*= matrix[index];
        }
        y++;
      }
      result[i*tile_size+j] += weight * vector[k];
    }
  }
}

int64_t *flat_4t(int8_t * matrix, int8_t * vector, int**sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);
  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    std::thread t(compute_flat_4, matrix, vector, result, i, sector[i]);
    threads.push_back(move(t));
  }
  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }
  return result;
}

void compute_flat_i(int8_t *matrix, int8_t * vector, int64_t * result, int *sector, int tile, int i,int inc){
  for(int j=i;j<i+inc;j++){
    for(int k=0;k<max_x;k++){
      int64_t weight=1;
      int y=0;
      while(sector[y]>0){
        for(int x=0;x<nCells;x++){
          int index = (((sector[y]*tile_per_arry)+i)*tile_size+j)*max_x+k*nCells+x;
          weight*= matrix[index];
        }
        y++;
        result[tile*tile_size+j] += weight * vector[k];
      }
    }
  }
}

int64_t * flat_8t(int8_t* matrix, int8_t * vector, int **sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<2;j++){
      std::thread t(compute_flat_i, matrix, vector, result, sector[i], i,j*64, 64);
      threads.push_back(move(t));
    }
  }

  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}

int64_t * flat_16t(int8_t* matrix, int8_t * vector, int **sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<4;j++){
      std::thread t(compute_flat_i, matrix, vector, result, sector[i], i,j*32, 32);
      threads.push_back(move(t));
    }
  }

  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}

int64_t * flat_32t(int8_t* matrix, int8_t * vector, int **sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  std::vector<std::thread> threads;
  for(int i=0;i<tile_per_arry;i++){
    for(int j=0;j<8;j++){
      std::thread t(compute_flat_i, matrix, vector, result, sector[i], i,j*16, 16);
      threads.push_back(move(t));
    }
  }

  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}

void flat_matrix_config(int8_t * matrix, int **sector){
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
        int x=0;
        
        while(sector[i][x]>=0){
          for(int y=0;y<nCells;y++){
            //std::cout<<matrix[i][j][k][sector[x]][y]<<std::endl;
            int index = (((sector[i][x]*tile_per_arry)+i)*tile_size+j)*max_x+k*nCells+y;
          
            line += std::to_string(matrix[index]);
            if(y != nCells-1){
              line += " ";
            }
          }
          if(sector[i][x+1]>0){
            line += " ";
          }
          x++;
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

//////////////////////////////////////////////////////////////////////////////////////
////////                                                                      ////////
////////                MULTISECTOR IMP                                       ////////
////////                                                                      ////////
//////////////////////////////////////////////////////////////////////////////////////

int8_t***** alloc_matrix_multisector(){
  
  int8_t ***** matrix = new int8_t****[tile_per_arry];  
  for(int x=0;x<tile_per_arry;x++){
      matrix[x] = new int8_t***[tile_size];
      for(int i=0;i<tile_size;i++){
        matrix[x][i]=new int8_t**[max_x];
  
        for(int j=0;j<max_x;j++){
          matrix[x][i][j]=new int8_t*[n_sectors];
  
          for(int k=0;k<n_sectors;k++){
            matrix[x][i][j][k]= new int8_t[nCells];
          }
        }
      }
    }
    
  return matrix;
}


void free_matrix_multisector(int8_t***** matrix){
  for(int x=0;x<tile_per_arry;x++){
    for(int i=0;i<tile_size;i++){
      for(int j=0;j<max_x;j++){
        for(int k=0;k<n_sectors;k++){
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

void random_matrix_multisec(int8_t***** matrix){

  int range = INT8_MAX - INT8_MIN + 1;
  srand(time(nullptr)); 
  for(int x=0;x<tile_per_arry;x++){

    for(int i=0;i<tile_size;i++){

      for(int j=0;j<max_x;j++){

        for(int k=0;k<n_sectors;k++){

          for(int l=0;l<nCells;l++){
            //std::cout<<"X: "<<x<<" i: "<<i<<" j: "<<j<<" k: "<<k<<" l: "<<l<<std::endl;
            matrix[x][i][j][k][l] = static_cast<int8_t>(INT8_MIN + (std::rand() % range));
            //std::cout<<static_cast<int16_t>(matrix[x][i][j][k][l])<<std::endl;
          }

        }
      }
    }
  }
}

void create_matrix_conf_file_multisec(int8_t ***** matrix, int *sector){
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
        int x=0;
        
        while(sector[x]>=0){
          for(int y=0;y<nCells;y++){
            //std::cout<<matrix[i][j][k][sector[x]][y]<<std::endl;
            line += std::to_string(matrix[i][j][k][sector[x]][y]);
            if(y != nCells-1){
              line += " ";
            }
          }
          if(sector[x+1]>0){
            line += " ";
          }
          x++;
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

int64_t * mvm_multisect_performance(int8_t***** matrix, int8_t * vector,int **sector){
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
        int y=0;
        
        while(sector[i][y]>=0){
          for(int x=0;x<nCells;x++){
            weight*= matrix[i][j][k][sector[i][y]][x];
          }
          y++;
        }
        result[i*tile_size+j] += weight * vector[k];
      }
    }
  }

  return result;
}

int64_t * mvm_multisect(int8_t***** matrix, int8_t * vector,int **sector){
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
        int y=0;
        while(sector[i][y]>=0){
          for(int x=0;x<nCells;x++){
            weight*= matrix[sector[i][y]][i][j][k][x];
          }
          y++;
        }
        result[i*tile_size+j] += weight * vector[k];
      }
    }
  }

  return result;
}



void create_matrix_conf_file_2(int8_t ***** matrix, int *secotr){
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
        int x=0;
        while(secotr[x]>=0){
          for(int y=0;y<nCells;y++){
            line += std::to_string(matrix[secotr[x]][i][j][k][y]);
            if(y != nCells-1){
              line += " ";
            }
          }
          if(secotr[x+1]>0){
            line += " ";
          }
          x++;
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


void compute_tile_4_std(int8_t ***** matrix, int8_t * vector, int64_t * result, int *sector, int tile){
  for(int j=0;j<tile_size;j++){

    for(int k=0;k<max_x;k++){
      int64_t weight=1;
      int y=0;
      while(sector[y]>=0){
        for(int x=0;x<nCells;x++){
          weight*= matrix[sector[y]][tile][j][k][x];
        }
        y++;
      }
      result[tile*tile_size+j] += weight * vector[k];
    }
  }
}

int64_t * mvm_std_4_t(int8_t***** matrix, int8_t * vector, int **sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  std::vector<std::thread> threads;

  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  for(int i=0;i<tile_per_arry;i++){
    std::thread t(compute_tile_4_std, matrix, vector, result, sector[i], i);
    threads.push_back(move(t));
  }
  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}

void compute_tile_4_fast(int8_t ***** matrix, int8_t * vector, int64_t * result, int *sector, int tile){
  for(int j=0;j<tile_size;j++){

    for(int k=0;k<max_x;k++){
      int64_t weight=1;
      int y=0;
      while(sector[y]>=0){
        for(int x=0;x<nCells;x++){
          weight*= matrix[tile][j][k][sector[y]][x];
        }
        y++;
      }
      result[tile*tile_size+j] += weight * vector[k];
    }
  }
}

int64_t * mvm_fast_4_t(int8_t***** matrix, int8_t * vector, int **sector){
  computations++;
  int64_t * result = new int64_t[max_vect];
  std::vector<std::thread> threads;

  
  handle_alloc_error(result);
  memset(result,0,sizeof(int64_t)*max_vect);

  for(int i=0;i<tile_per_arry;i++){
    std::thread t(compute_tile_4_fast, matrix, vector, result, sector[i], i);
    threads.push_back(move(t));
  }
  for(int i=0;i<threads.size();i++){
    threads[i].join();
  }

  return result;
}


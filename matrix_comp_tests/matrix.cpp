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
  for(int x=0;x<n_sectors;x++){
    for(int i=0;i<tile_per_arry;i++){
      for(int j=0;j<tile_size;j++){
        
        for(int k=0;k<max_x;k++){

          for(int l=0;l<nCells;l++){
            matrix[x][i][j][k][l] = init;
          }

        }
      }
    }
  }
}

void init_vecotr(int8_t * vector, int8_t init){
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

void free_matrix(int8_t**** matrix){
 
} 

//TODO: a way to handle multiple sectors for same matrix
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
        result[i*tile_size+j] += weight * vector[i*tile_size+j];
      }
    }
  }

  return result;
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
          line += "-";
        }
      }
      if(i != tile_per_arry-1){
        line += "-";
      }
    }
    if(k != max_x-1){
      line += "\n";
    }
    fputs(line.c_str(), fp);
  }

  fclose(fp);
}

void create_vector_conf_file(int8_t *vector){
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
      line += "-";
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
      line += "-";
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



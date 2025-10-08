#include "bttd.hpp"

//sector{1,2,3,4}-1;
void new_mvm(pcm_size_t*matrix,input_size_t*vector,volatile int*volatile layers,int*sectors,int64_t*result ){
    ++computations;
    memset(result,0,512*8);

    int s=0;
    while(sectors[s]!=-1){
        
        for(int y=0;y<tile_size;++y){
            for(int x=0;x<max_x;++x){
                int l=0;
                uint64_t wheight=0;
                while(layers[l]!=-1){
                    wheight+=matrix[m_inedx(sectors[s],layers[l],y,x)];
                    ++l;
                }
                result[sectors[s]*128+y]+=wheight*vector[x];
            }
        }
            
        s++;
    }

}

void new_mvm_2(pcm_size_t*matrix,input_size_t*vector,volatile int*volatile layers,int*sectors,int64_t*result ){
    ++computations;
    memset(result,0,512*8);

    int s=0;
    while(sectors[s]!=-1){
        
        for(int y=0;y<tile_size;++y){
            for(int x=0;x<max_x;++x){
                int l=0;
                uint64_t wheight=0;
                while(layers[l]!=-1){
                    wheight+=matrix[m_inedx_2(sectors[s],layers[l],y,x)];
                    ++l;
                }
                result[sectors[s]*128+y]+=wheight*vector[x];
            }
        }
            
        s++;
    }

}

void new_mvm_3(pcm_size_t* matrix, input_size_t* vector, volatile int* volatile layers, int* sectors, int64_t* result) {
    ++computations;
    memset(result, 0, 512 * sizeof(int64_t)); 

    int s_idx = 0;
    while (sectors[s_idx] != -1) {
        int s = sectors[s_idx];

        int l_idx = 0;
        while (layers[l_idx] != -1) {
            int l = layers[l_idx];

            
            pcm_size_t* matrix_base = &matrix[m_inedx(s, l, 0, 0)];

            for (int y = 0; y < tile_size; ++y) {
                int64_t* result_row_ptr = &result[s * 128 + y];

                for (int x = 0; x < max_x; ++x) {
                    uint64_t wheight = *matrix_base; 
                    *result_row_ptr += wheight * vector[x];
                    
                    matrix_base++; 
                }
            }
            l_idx++;
        }
        s_idx++;
    }
}

void new_mvm_4(pcm_size_t* matrix, input_size_t* vector, volatile int* volatile layers, int* sectors, int64_t* result) {
    ++computations;
    memset(result, 0, 512 * sizeof(int64_t)); 

    int s_idx = 0;
    while (sectors[s_idx] != -1) {
        int s = sectors[s_idx];

        int l_idx = 0;
        while (layers[l_idx] != -1) {
            int l = layers[l_idx];

            
            pcm_size_t* matrix_base = &matrix[m_inedx(s, l, 0, 0)];

            for (int y = 0; y < tile_size; ++y) {
                int64_t* result_row_ptr = &result[s * 128 + y];
                int64_t row_sum = 0;

                for (int x = 0; x < max_x; ++x) {
                    
                    row_sum += *matrix_base * vector[x];
                    
                    matrix_base++; 
                }
                result[s * 128 + y] += row_sum;
            }
            l_idx++;
        }
        s_idx++;
    }
}


void acc_test(pcm_size_t*matrix,volatile int*volatile layers,int*sectors){
    int s=0;
    while(sectors[s]!=-1){
        int l=0;
        while(layers[l]!=-1){
            for(int y=0;y<tile_size;++y){
                for(int x=0;x<max_x;++x){

                    std::cout<<std::to_string(static_cast<int>(matrix[m_inedx(sectors[s],layers[l],y,x)]))<<"\t";
                }
            }
            l++;
        }
        s++;
    }
}

void print_mvm(pcm_size_t*matrix,volatile int*volatile layers,int*sectors){
    std::string filename ="./result/matrix"+std::to_string(computations)+".conf";
    FILE* fp = fopen(filename.c_str(), "w+");

    if (fp == NULL) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    int s=0;
    for(int i=0;i<n_sectors;++i){
        if(sectors[s]!=i){
            int l=0;
            std::string line = "";
            while(layers[l]!=-1){
                for(int y=0;y<tile_size;++y){
                    for(int x=0;x<max_x;++x){

                        line += "0";
                        //std::cout<<line<<std::endl;
                        if(x != max_x-1){
                            line += "|";
                        }
                    }
                    line+="\n";
                }
                l++;
            }
            fputs(line.c_str(), fp);
        }
        else{
            
            std::string line = "";
            
                for(int y=0;y<tile_size;++y){
                    for(int x=0;x<max_x;++x){
                        uint64_t wheight=0;
                        int l=0;
                        while(layers[l]!=-1){
                            wheight+=matrix[m_inedx(sectors[s],layers[l],y,x)];
                            l++;
                        }
                        line += std::to_string(static_cast<int>(wheight));
                        //std::cout<<line<<std::endl;
                        if(x != max_x-1){
                            line += "|";
                        }
                    }
                    line+="\n";
                }
                            
            fputs(line.c_str(), fp);
            s++;
        }
       
    }
    fclose(fp);

}
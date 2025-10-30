#include "bttd.hpp"

void sslow_mvm(pcm_size_t****matrix,input_size_t*vector,int**layers,int*sectors,int64_t*result){
    ++computations;
    memset(result,0,512*8);

    int s_idx=0;
    while(sectors[s_idx]!=-1){
        
        for(int y=0;y<tile_size;++y){
            for(int x=0;x<max_x;++x){
                int l=0;
                uint64_t wheight=0;
                while(layers[sectors[s_idx]][l]!=-1){
                    wheight+=matrix[sectors[s_idx]][layers[sectors[s_idx]][l]][y][x];
                    l++;
                }
                result[sectors[sectors[s_idx]]*128+y]+=wheight*vector[x];
            }
        }
            
        s_idx++;
    }
    
}

//sector{1,2,3,4}-1;
void new_mvm(pcm_size_t*matrix,input_size_t*vector, int* *layers,int*sectors,int64_t*result ){
    ++computations;
    memset(result,0,512*8);

    int s_idx=0;
    while(sectors[s_idx]!=-1){
        int s=sectors[s_idx];
        for(int y=0;y<tile_size;++y){
            for(int x=0;x<max_x;++x){
                int l=0;
                uint64_t wheight=0;
                while(layers[s][l]!=-1){
                    wheight+=matrix[m_inedx(s,layers[s][l],y,x)];
                    ++l;
                }
                result[sectors[s]*128+y]+=wheight*vector[x];
            }
        }
            
        s_idx++;
    }

}

void new_mvm_2(pcm_size_t*matrix,input_size_t*vector, int** layers,int*sectors,int64_t*result ){
    ++computations;
    memset(result,0,512*8);

    int s_idx=0;
    while(sectors[s_idx]!=-1){
        int s=sectors[s_idx];
        for(int y=0;y<tile_size;++y){
            for(int x=0;x<max_x;++x){
                int l=0;
                uint64_t wheight=0;
                while(layers[s][l]!=-1){
                    wheight+=matrix[m_inedx_2(s,layers[s][l],y,x)];
                    ++l;
                }
                result[sectors[s]*128+y]+=wheight*vector[x];
            }
        }
            
        s_idx++;
    }

}

void new_mvm_3(pcm_size_t* matrix, input_size_t* vector,  int**  layers, int* sectors, int64_t* result) {
    ++computations;
    memset(result, 0, 512 * sizeof(int64_t)); 

    int s_idx = 0;
    while (sectors[s_idx] != -1) {
        int s = sectors[s_idx];

        int l_idx = 0;
        while (layers[s][l_idx] != -1) {
            int l = layers[s][l_idx];

            
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

void new_mvm_4(pcm_size_t* matrix, input_size_t* vector,  int**  layers, int* sectors, int64_t* result) {
    ++computations;
    memset(result, 0, 512 * sizeof(int64_t)); 

    int s_idx = 0;
    while (sectors[s_idx] != -1) {
        int s = sectors[s_idx];

        int l_idx = 0;
        int* curr_layers = layers[s];
        while (curr_layers[l_idx] != -1) {
            int l = curr_layers[l_idx];

            
            pcm_size_t* matrix_base = &matrix[m_inedx(s, l, 0, 0)];

            for (int y = 0; y < tile_size; ++y) {
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

void multi_thread_sector(int s, pcm_size_t* matrix, input_size_t* vector,  int** layers, int64_t* result) {

        int l_idx = 0;
        int* curr_layers = layers[s];
        while (curr_layers[l_idx] != -1) {
            int l = curr_layers[l_idx];

            
            pcm_size_t* matrix_base = &matrix[m_inedx(s, l, 0, 0)];

            for (int y = 0; y < tile_size; ++y) {
                int64_t row_sum = 0;

                for (int x = 0; x < max_x; ++x) {
                    
                    row_sum += *matrix_base * vector[x];
                    
                    matrix_base++; 
                }
                result[s * 128 + y] += row_sum;
            }
            l_idx++;
        }
}

void new_mvm_mtd_4(pcm_size_t* matrix, input_size_t* vector,  int**  layers, int* sectors, int64_t* result) {
    ++computations;
    memset(result, 0, 512 * sizeof(int64_t)); 
    std::vector<std::thread> threads;

    int s_idx = 0;
    while (sectors[s_idx] != -1) {
        std::thread t(multi_thread_sector, sectors[s_idx], matrix, vector, layers, result);
        threads.push_back(move(t));
        s_idx++;
    }

    for(int i=0;i<threads.size();i++){
        threads[i].join();
    }
}

void multi_thread_layer( pcm_size_t* matrix, int s,input_size_t* vector, std::atomic<int64_t>* result) {

    for (int y = 0; y < tile_size; ++y) {
        int64_t row_sum = 0;

        for (int x = 0; x < max_x; ++x) {
            
            row_sum += *matrix * vector[x];
            
            matrix++; 
        }
        result[s * 128 + y].fetch_add(row_sum, std::memory_order_relaxed);
    }
        
}

void new_mvm_mtd_8(pcm_size_t* matrix, input_size_t* vector,  int**  layers, int* sectors, int64_t* result) {
    ++computations;
    memset(result, 0, 512 * sizeof(int64_t)); 
    std::vector<std::thread> threads;
    std::atomic<int64_t> temp_result[512];
    for(int i=0;i<512;++i)temp_result[i]=0;


    int s_idx = 0;
    while (sectors[s_idx] != -1) {
        int l_idx = 0;
        int s=sectors[s_idx];
        int* curr_layers = layers[s];
        while (curr_layers[l_idx] != -1) {
            int l = curr_layers[l_idx];

            std::thread t(multi_thread_layer, &matrix[m_inedx(s, l, 0, 0)],s, vector, temp_result);
            threads.push_back(move(t));
            
            l_idx++;
        }

        s_idx++;
    }

    for(int i=0;i<threads.size();i++){
        threads[i].join();
    }

    for(int i=0;i<512;++i){
        result[i]=temp_result[i].load(std::memory_order_relaxed);
    }
}

void new_mvm_mtd_16(pcm_size_t* matrix, input_size_t* vector,  int** layers, int* sectors, int64_t* result) {
    ++computations;
    memset(result, 0, 512 * sizeof(int64_t)); 
    std::vector<std::thread> threads;
    int thread_count = 2;
    std::atomic<int64_t> temp_result[512];
    for(int i=0;i<512;++i)temp_result[i]=0;
    

    int s_idx = 0;
    while (sectors[s_idx] != -1) {
        int s=sectors[s_idx];
        int l_idx = 0;
        int* curr_layers = layers[s];
        while (curr_layers[l_idx] != -1) {
            int l = curr_layers[l_idx];

            for(int i=0;i<thread_count;++i){
                int y_start = i * (tile_size / thread_count);
                int end = (i + 1) * (tile_size / thread_count); 
                
                std::thread t(multi_thread_layer_2, &matrix[m_inedx(s, l, 0, 0)+y_start*max_x],s, vector, temp_result,y_start,end);
                threads.push_back(move(t));
            
            }
            l_idx++;
        }

        s_idx++;
    }
    for(int i=0;i<threads.size();i++){
        threads[i].join();
    }
    for(int i=0;i<512;++i){
        result[i]=temp_result[i].load(std::memory_order_relaxed);
    }
}


void multi_thread_layer_2( pcm_size_t* matrix, int s,input_size_t* vector, std::atomic<int64_t>* result,int y_start,int end) {
    for (int y = y_start; y < end; ++y) {
        int64_t row_sum = 0;

        for (int x = 0; x < max_x; ++x) {
            
            row_sum += *matrix * vector[x];
            
            matrix++; 
        }
        result[s * 128 + y].fetch_add(row_sum, std::memory_order_relaxed);
    }
        
}


void acc_test(pcm_size_t*matrix, int* layers,int*sectors){
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

void print_mvm(pcm_size_t*matrix, int** layers,int*sectors){
    std::string filename ="./result/matrix"+std::to_string(computations)+".conf";
    FILE* fp = fopen(filename.c_str(), "w+");

    if (fp == NULL) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    int s_idx=0;
    for(int i=0;i<n_sectors;++i){
        int s=sectors[s_idx];
        if(s!=i){
            if(s==-1)s_idx--;
            int l=0;
            
            std::string line = "";
            while(layers[s_idx][l]!=-1){
                for(int y=0;y<tile_size;++y){
                    for(int x=0;x<max_x;++x){

                        line += "0";
                        if(x != max_x-1){
                            line += "|";
                        }
                    }
                    line+="\n";
                }
                l++;
            }
            fputs(line.c_str(), fp);
            if(s==-1)s_idx++;
        }
        else{
            
            std::string line = "";
            
                for(int y=0;y<tile_size;++y){
                    for(int x=0;x<max_x;++x){
                        uint64_t wheight=0;
                        int l=0;
                        while(layers[s][l]!=-1){
                            wheight+=matrix[m_inedx(sectors[s],layers[s][l],y,x)];
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
            s_idx++;
        }
       
    }
    fclose(fp);

}
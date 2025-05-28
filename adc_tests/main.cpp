#include "adc.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdint>
#include <string>
#include "../logger.hpp"
#include <time.h>
#include <bitset>
#include <random>
#include <cstdlib>
#include <cassert>
#include <sys/syscall.h> 
#include <unistd.h> 
#include <thread>
#include <signal.h>



void do_something();
void thread_tests();



void test();

Logger logger("adc.log");
int main(){

    // size_t size =4;
    // uint8_t *buff = new uint8_t[size];

    // for(int i=0;i<size;i++){

    //     buff[i] = static_cast<uint8_t>(0b10101010);
    //     std::cout<<std::bitset<8>(buff[i])<<std::endl;

    // }
    // uint64_t value = buffer_extraction(buff,size);
    // std::cout<<std::bitset<64>(value)<<std::endl;


    // srand(time(NULL));    
    // for(int i=0;i<10;i++){
    //     adc((int64_t)rand(),true);
    // }
    //cell_weight_conversion();

    // uint8_t n = (1 << CELL_SIZE) - 1;
    // std::cout<<std::bitset<8>(n)<<std::endl;
    
}

void do_something(){
    pid_t tid = static_cast<pid_t>(::syscall(SYS_gettid));
    std::cout << "Thread TID: " << tid <<" eeping"<< std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100000));
}

void thread_tests(){
    pid_t tid = static_cast<pid_t>(::syscall(SYS_gettid));
    std::cout << "Main Thread TID: " << tid << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<std::thread> threads;
    for(int i=0;i<10;i++){
        std::thread t(do_something);
        threads.push_back(std::move(t));
    }
    for(int i=0;i<threads.size();i++){
        std::cout<<threads[i].get_id()<<std::endl;
    }

    for(int i=0;i<threads.size();i++){
        //pthread_kill(threads[i].get_id(), 9);
    }

    
    std::this_thread::sleep_for(std::chrono::milliseconds(100000));
    for(int i=0;i<threads.size();i++){
        threads[i].join();
    }
}


void test(){

    
}
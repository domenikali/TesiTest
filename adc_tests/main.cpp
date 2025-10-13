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
#include <string.h>
#include <signal.h>



void do_something();
void thread_tests();



void test_memcpy();

Logger logger("adc.log");
int main(){

    int output_size = 8;
    int unsigned_comp =1;
    uint32_t max_mask = 1<<output_size-1;

    std::cout << "Max Mask: " << std::bitset<32>(max_mask) << std::endl;
    
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


void test_memcpy(){

    uint8_t *buff = new uint8_t[8];
    for (int i = 0; i < 8; i++)
    {
        buff[i]=static_cast<uint8_t>(i);
    }

    uint32_t *value = new uint32_t[2];
    memcpy(value, buff, 8);
    std::cout << "Value: " << std::bitset<32>(value[0]) << std::endl;
    for(int i=0;i<4;i++){
        std::cout << "Byte " << i << ": " << std::bitset<8>(buff[i]) << std::endl;
    }
    std::cout << "Value: " << std::bitset<32>(value[1]) << std::endl;


    
}
# Dissertation Tests
Hey, welcome to my testing playground for the dissertation project!

# How to Run
In each folder, you'll find tests for different implementations. It's really simple to get things started:

''''
make

make run
''''

Everything will display clearly, and you'll also get a .txt file with detailed logs.

## MVM Tests
For the matrix-vector multiplication (MVM) tests, go to the matrix_comp_tests folder and run the following commands:

''''
make 

make run

less logs.txt
''''

 I'm working on improving the speed and efficiency of MVM algorithms for my PCM model in the GVSoC simulator. The goal is to find a good balance, as the original simulator struggled with space and time complexity in cluster setups.

 Right now, my second implementation—where I split the computation among threads—is showing a speedup of about 34 to 36 times faster than the standard one (on average). I expected that using more threads might require more memory, but Valgrind's Massif shows that the memory usage remains nearly the same.


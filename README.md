# Dissertation Tests
Hey, welcome to my testing playground for the dissertation project!

# How to Run
In each folder, you'll find tests for different implementations. It's really simple to get things started:

```
make

make run
```

Everything will display clearly, and you'll also get a .txt file with detailed logs.

## MVM Tests
For the matrix-vector multiplication (MVM) tests, go to the matrix_comp_tests folder and run the following commands:

```
make 

make run
```

This will create a new folder with the number of hardware core of your machine and will execute 500 runs for each algorithm on the same mvm

Running the "my_graph.py" script will produce 3 graph to evaluate each implementation on different machines.

Use the following comand to check MVMs results with python numpy in case the standard test is runned.
```
make checks
```
### Results:
I'm working on improving the speed and efficiency of MVM algorithms for my PCM model in the GVSoC simulator. The goal is to find a good balance, as the original simulator struggled with space and time complexity in cluster setups.

After running extensive tests on multiple machines, it's clear that a multithreaded version is faster than the standard implementation, although it's quite difficult to establish a direct correlation between hardware thread count and the performance gain from multithreaded division.
 

![alt text][/matrix_comp_tests/core_20/my_violin.png]
![alt text][/matrix_comp_tests/core_16/my_violin.png]
![alt text][/matrix_comp_tests/core_2/my_violin.png]


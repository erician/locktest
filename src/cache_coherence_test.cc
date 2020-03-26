/*
 * This is the main file of cas test.
 */
#include <thread>
#include <iostream>
#include <sys/time.h>

#define MAX_NR_THREADS      4
#define NR_ADD_OPERATIONS   1000*1000*100

volatile unsigned long long counter = 0;
double *times;

void Add(int nr_ops) {
    unsigned long long tmp = counter;
    for(int i=0; i<nr_ops; i++) {
        // the counter is not coherent.
        tmp = counter;
    }
}

void PrintResult(int nr_thread) {
    std::cout << ", thread num: " << nr_thread << \
    ", total time: " << times[1]-times[0] << \
    ", the number of add operations: " << NR_ADD_OPERATIONS << \
    ", ops: " << NR_ADD_OPERATIONS*1.0/(times[1]-times[0]) << std::endl;
}

double mysecond() {
    struct timeval tp;
    int i;
    i = gettimeofday(&tp, NULL);
    return ( (double)tp.tv_sec + (double)tp.tv_usec*1.e-6);
}

int main() {
    std::thread *threads[MAX_NR_THREADS];
    times = new double [2];
    //test cpp mutex lock
    for(int i=1; i<=MAX_NR_THREADS; i++) {
        counter = 0;
        times[0] = mysecond();
        for(int j=0; j<i; j++)
            threads[j] = new std::thread(Add, NR_ADD_OPERATIONS/i);
        for(int j=0; j<i; j++)
            threads[j]->join();
        times[1] = mysecond();
        PrintResult(i);
    }
    return 0;
}
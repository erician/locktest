/*
 * This is the main file of cas test.
 */
#include <thread>
#include <iostream>
#include <sys/time.h>

#define CAS(_p, _u, _v)  (__atomic_compare_exchange_n \
    (_p, _u, _v, false, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE))

#define MAX_NR_THREADS      4
#define NR_ADD_OPERATIONS   1000*1000*100

volatile unsigned long long counters[MAX_NR_THREADS];
volatile unsigned long long thread_epoch[MAX_NR_THREADS][MAX_NR_THREADS]; // default INF
thread_local unsigned long long ends[MAX_NR_THREADS];

double *times;

void Add(int nr_ops, int thread_id) {
    for(int i=0; i<nr_ops; i++) {
        // if(i%100 == 0)
        for(int j=1; j<=MAX_NR_THREADS; j++) {
            if(j != thread_id) {
                ends[j-1]= counters[j-1];
                //if(i%100 == 0)
                thread_epoch[thread_id-1][j-1] = counters[j-1];
            }
        }
        counters[thread_id-1] += 1;
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
        counters[i-1] = 0;
        times[0] = mysecond();
        for(int j=0; j<i; j++)
            threads[j] = new std::thread(Add, NR_ADD_OPERATIONS/i, i);
        for(int j=0; j<i; j++)
            threads[j]->join();
        times[1] = mysecond();
        PrintResult(i);
    }
    return 0;
}
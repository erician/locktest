/*
 * This is the main file of lock test.
 */

#ifndef LOCK_TEST_ABSTRACT_LOCK_H_
#include "abstract_lock.h"
#endif

#ifndef LOCK_TEST_CPP_MUTEX_LOCK_H_
#include "cpp_mutex_lock.h"
#endif

#ifndef LOCK_TEST_LEVELHASHING_SPINLOCK_H_
#include "levelhashing_spinlock.h"
#endif

#include <thread>
#include <iostream>
#include <sys/time.h>

#define MAX_NR_THREADS      4
#define NR_ADD_OPERATIONS   1000*1000*100

volatile unsigned long counter = 0;
double *times;

void Add(locktest::AbstractLock *lock, int nr_ops) {
    for(int i=0; i<nr_ops; i++) {
        lock->Lock();
        counter++;
        lock->Unlock();
    }
}

void PrintResult(locktest::AbstractLock *lock, int nr_thread) {
    std::cout << lock->GetName();
    if(NR_ADD_OPERATIONS/nr_thread*nr_thread != counter) {
        std::cout << ", error, the counter is: " << counter << \
        ", but expected: " << NR_ADD_OPERATIONS/nr_thread*nr_thread;
        exit(-1);
    }else {
        std::cout << ", ok";
    }
    std::cout << ", thread num: " << nr_thread << \
    ", total time: " << times[1]-times[0] << \
    ", the number of add operations: " << NR_ADD_OPERATIONS << std::endl;
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
    locktest::CppMutexLock *cpp_mutex_lock = new locktest::CppMutexLock();;
    for(int i=1; i<=MAX_NR_THREADS; i++) {
        counter = 0;
        times[0] = mysecond();
        for(int j=0; j<i; j++)
            threads[j] = new std::thread(Add, cpp_mutex_lock, NR_ADD_OPERATIONS/i);
        for(int j=0; j<i; j++)
            threads[j]->join();
        times[1] = mysecond();
        PrintResult(cpp_mutex_lock, i);
    }

    //test levelhashing spinlock
    locktest::LevelhashingSpinlock *levelhashing_spinlock = new locktest::LevelhashingSpinlock();
    for(int i=1; i<=MAX_NR_THREADS; i++) {
        counter = 0;
        times[0] = mysecond();
        for(int j=0; j<i; j++)
            threads[j] = new std::thread(Add, levelhashing_spinlock, NR_ADD_OPERATIONS/i);
        for(int j=0; j<i; j++)
            threads[j]->join();
        times[1] = mysecond();
        PrintResult(levelhashing_spinlock, i);
    }

    return 0;
}
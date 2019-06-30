/*
 * This is the main file of rwlock test.
 */

#ifndef LOCK_TEST_ABSTRACT_RW_LOCK_H_
#include "abstract_rwlock.h"
#endif

#ifndef LOCK_TEST_CPP_MUTEX_LOCK_H_
#include "cpp_shared_mutex_lock.h"
#endif

#ifndef LOCK_TEST_ABSTRACT_RW_LOCK_H_
#include "abstract_rwlock.h"
#endif

#ifndef LOCK_TEST_CCEH_SELF_RW_LOCK_H_
#include "cceh_self_rwlock.h"
#endif

#include <thread>
#include <iostream>
#include <sys/time.h>
/*
 * To test rwlock, this value should be even and must be equal or greater than 2.
 * We will use the half of MAX_NR_THREADS to run read lock, and
 * the other half to run read lock.
 */
#define MAX_NR_THREADS      8
#define NR_ADD_OPERATIONS   1000*1000

volatile unsigned long counter = 0;
unsigned long cur_counter = 0;
double *times;

double mysecond() {
    struct timeval tp;
    int i;
    i = gettimeofday(&tp, NULL);
    return ( (double)tp.tv_sec + (double)tp.tv_usec*1.e-6);
}

void Add(locktest::AbstractRwLock *rwlock, int nr_ops) {
    double sub_time;
    sub_time = mysecond();
    for(int i=0; i<nr_ops; i++) {
        rwlock->WriteLock();
        counter++;
        rwlock->WriteUnlock();
    }
}

void Read(locktest::AbstractRwLock *rwlock, int nr_ops) {
    double sub_time;
    sub_time = mysecond();
    for(int i=0; i<nr_ops; i++) {
        rwlock->ReadLock();
        cur_counter = counter;
        rwlock->ReadUnlock();
    }
}

void PrintResult(locktest::AbstractRwLock *rwlock, int nr_thread) {
    std::cout << rwlock->GetName();
    if(NR_ADD_OPERATIONS/(nr_thread/2)*(nr_thread/2) != counter) {
        std::cout << ", error, the counter is: " << counter << \
        ", but expected: " << NR_ADD_OPERATIONS/(nr_thread/2)*(nr_thread/2);
        exit(-1);
    }else {
        std::cout << ", ok";
    }
    std::cout << ", thread num: " << nr_thread << \
    ", total time: " << times[1]-times[0] << \
    ", the number of add operations: " << NR_ADD_OPERATIONS << std::endl;
}

int main() {
    std::thread *threads[MAX_NR_THREADS];
    times = new double [2];
    //test cpp mutex lock
    //the result looks like weird in my desktop computer, it looks normal in our servers,
    //seriously, I do not know why.
    locktest::CppSharedMutexLock *cpp_shared_mutex_lock = new locktest::CppSharedMutexLock();;
    for(int i=2; i<=MAX_NR_THREADS; i+=2) {
        counter = 0;
        times[0] = mysecond();
        for(int j=0; j<i; j++) {
            if(j%2 == 0)
                threads[j] = new std::thread(Read, cpp_shared_mutex_lock, NR_ADD_OPERATIONS);
            else
                threads[j] = new std::thread(Add, cpp_shared_mutex_lock, NR_ADD_OPERATIONS/(i/2));
        }
        for(int j=0; j<i; j++) {
            threads[j]->join();
        }
        times[1] = mysecond();
        PrintResult(cpp_shared_mutex_lock, i);
    }
    //test cceh self rwlock, there is a bug in it
    // locktest::CcehSelfRwLock *cceh_self_rwlock = new locktest::CcehSelfRwLock();;
    // for(int i=2; i<=MAX_NR_THREADS; i+=2) {
    //     counter = 0;
    //     times[0] = mysecond();
    //     for(int j=0; j<i; j++) {
    //         if(j%2 == 0)
    //             threads[j] = new std::thread(Read, cceh_self_rwlock, NR_ADD_OPERATIONS);
    //         else
    //             threads[j] = new std::thread(Add, cceh_self_rwlock, NR_ADD_OPERATIONS/(i/2));
    //     }
    //     for(int j=0; j<i; j++) {
    //         threads[j]->join();
    //     }
    //     times[1] = mysecond();
    //     PrintResult(cceh_self_rwlock, i);
    // }
    return 0;
}
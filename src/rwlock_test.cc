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

#include <thread>
#include <iostream>

#define MAX_NR_THREADS      4
#define NR_ADD_OPERATIONS   1000*1000*100

volatile unsigned long counter = 0;

void Add(locktest::AbstractRwLock *rwlock, int nr_ops) {
    for(int i=0; i<nr_ops; i++) {
        rwlock->WriteLock();
        counter++;
        rwlock->WriteUnlock();
    }
}

void Read(locktest::AbstractRwLock *rwlock, int nr_ops) {
    for(int i=0; i<nr_ops; i++) {
        rwlock->ReadLock();
        counter++;
        rwlock->ReadUnlock();
    }
}

void PrintResult(locktest::AbstractRwLock *rwlock, int nr_thread) {
    std::cout << rwlock->GetName();
    if(NR_ADD_OPERATIONS/nr_thread*nr_thread != counter) {
        std::cout << ", error, the counter is: " << counter << \
        ", but expected: " << NR_ADD_OPERATIONS/nr_thread*nr_thread << \
        ", thread num: " << nr_thread << std::endl;
        exit(-1);
    }else {
        std::cout << ", ok" << \
        ", thread num: " << nr_thread << std::endl;
    }
}

int main() {
    std::thread *threads[MAX_NR_THREADS];
    //test cpp mutex lock
    locktest::CppSharedMutexLock *cpp_shared_mutex_lock = new locktest::CppSharedMutexLock();;
    for(int i=1; i<=MAX_NR_THREADS; i++) {
        counter = 0;
        for(int j=0; j<i; j++)
            threads[j] = new std::thread(Add, cpp_shared_mutex_lock, NR_ADD_OPERATIONS/i);
        for(int j=0; j<i; j++)
            threads[j]->join();
        PrintResult(cpp_shared_mutex_lock, i);
    }
    return 0;
}
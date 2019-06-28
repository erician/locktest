/*
 * This is the main file of locktest.
 */

#ifndef LOCK_TEST_ABSTRACT_LOCK_H_
#include "abstract_lock.h"
#endif

#ifndef LOCK_TEST_CPP_MUTEX_LOCK_H_
#include "cpp_mutex_lock.h"
#endif

#ifdef LOCK_TEST_ABSTRACT_LOCK_H_
#include "levelhashing_spinlock.h"
#endif

#include <thread>
#include <iostream>

#define MAX_NR_THREADS      4
#define NR_ADD_OPERATIONS   1000*1000*100

volatile unsigned long counter = 0;

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
    locktest::CppMutexLock *cpp_mutex_lock = new locktest::CppMutexLock();;
    for(int i=1; i<=MAX_NR_THREADS; i++) {
        counter = 0;
        for(int j=0; j<i; j++)
            threads[j] = new std::thread(Add, cpp_mutex_lock, NR_ADD_OPERATIONS/i);
        for(int j=0; j<i; j++)
            threads[j]->join();
        PrintResult(cpp_mutex_lock, i);
    }

    //test levelhashing spinlock
    locktest::LevelhashingSpinlock *levelhashing_spinlock = new locktest::LevelhashingSpinlock();
    for(int i=1; i<=MAX_NR_THREADS; i++) {
        counter = 0;
        for(int j=0; j<i; j++)
            threads[j] = new std::thread(Add, levelhashing_spinlock, NR_ADD_OPERATIONS/i);
        for(int j=0; j<i; j++)
            threads[j]->join();
        PrintResult(levelhashing_spinlock, i);
    }

    return 0;
}
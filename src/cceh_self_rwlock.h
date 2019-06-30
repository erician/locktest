/*
 * This rwlock is extracted from CCEH, the github url is: 
 * https://github.com/DICL/CCEH.git.
 */

#ifndef LOCK_TEST_CCEH_SELF_RW_LOCK_H_
#define LOCK_TEST_CCEH_SELF_RW_LOCK_H_

#ifndef LOCK_TEST_ABSTRACT_RW_LOCK_H_
#include "abstract_rwlock.h"
#endif

#define CAS(_p, _u, _v)  (__atomic_compare_exchange_n (_p, _u, _v, false, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE))

#include <string>

namespace locktest {

class CcehSelfRwLock: public AbstractRwLock {
public:
    CcehSelfRwLock() {};
    ~CcehSelfRwLock() {};
    void ReadLock();
    void ReadUnlock();
    void WriteLock();
    void WriteUnlock();
    std::string GetName();
private:
    int64_t sema_ = 0;
};

void CcehSelfRwLock::ReadLock() {
    while(sema_ == -1);     // this and next statement is not atomic, here is the bug.
    auto lock = sema_;
    while (!CAS(&sema_, &lock, lock+1)) {
        lock = sema_;
    }
}

void CcehSelfRwLock::ReadUnlock() {
    auto lock = sema_;
    while (!CAS(&sema_, &lock, lock-1)) {
        lock = sema_;
    }
}

void CcehSelfRwLock::WriteLock() {
    int64_t lock = 0;
    while(CAS(&sema_, &lock, -1) == false);
}

void CcehSelfRwLock::WriteUnlock() {
    sema_ = 0;
}

std::string CcehSelfRwLock::GetName() {
    return "CcehSelfRwLock";
}

} // namespace locktest

#endif
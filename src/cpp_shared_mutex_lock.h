#ifndef LOCK_TEST_CPP_MUTEX_LOCK_H_
#define LOCK_TEST_CPP_MUTEX_LOCK_H_

#ifndef LOCK_TEST_ABSTRACT_RW_LOCK_H_
#include "abstract_rwlock.h"
#endif

#include <shared_mutex>
#include <mutex>
#include <string>

namespace locktest {

class CppSharedMutexLock: public AbstractRwLock {
public:
    CppSharedMutexLock() {};
    ~CppSharedMutexLock() {};
    void ReadLock();
    void ReadUnlock();
    void WriteLock();
    void WriteUnlock();
    std::string GetName();
private:
    std::shared_mutex rwlock_;
};

void CppSharedMutexLock::ReadLock() {
    rwlock_.lock_shared();
}

void CppSharedMutexLock::ReadUnlock() {
    rwlock_.unlock_shared();
}

void CppSharedMutexLock::WriteLock() {
    rwlock_.lock();
}

void CppSharedMutexLock::WriteUnlock() {
    rwlock_.unlock();
}

std::string CppSharedMutexLock::GetName() {
    return "CppSharedMutexLock";
}

} // namespace locktest

#endif
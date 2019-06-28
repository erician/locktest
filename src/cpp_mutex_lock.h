#ifndef LOCK_TEST_CPP_MUTEX_LOCK_H_
#define LOCK_TEST_CPP_MUTEX_LOCK_H_

#ifndef LOCK_TEST_ABSTRACT_LOCK_H_
#include "abstract_lock.h"
#endif

#include <mutex>
#include <string>

namespace locktest {

class CppMutexLock: public AbstractLock {
public:
    CppMutexLock() {};
    ~CppMutexLock() {};
    void Lock();
    void Unlock();
    std::string GetName();
private:
    std::mutex mtx_;
};

void CppMutexLock::Lock() {
    mtx_.lock();
}

void CppMutexLock::Unlock() {
    mtx_.unlock();
}

std::string CppMutexLock::GetName() {
    return "CppMutexLock";
}

} // namespace locktest

#endif
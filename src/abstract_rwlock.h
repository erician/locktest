#ifndef LOCK_TEST_ABSTRACT_RW_LOCK_H_
#define LOCK_TEST_ABSTRACT_RW_LOCK_H_

#include <string>

namespace locktest {

class AbstractRwLock {
public:
    AbstractRwLock() {};
    virtual ~AbstractRwLock() {};
    virtual void ReadLock() = 0;        // your code should implement this func
    virtual void ReadUnlock() = 0;      // your code should implement this func
    virtual void WriteLock() = 0;       // your code should implement this func
    virtual void WriteUnlock() = 0;     // your code should implement this func
    virtual std::string GetName() = 0;  // your code should implement this func
private:

};

} // namespace locktest

#endif
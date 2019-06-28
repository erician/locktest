#ifndef LOCK_TEST_ABSTRACT_LOCK_H_
#define LOCK_TEST_ABSTRACT_LOCK_H_

#include <string>

namespace locktest {

class AbstractLock {
public:
    AbstractLock() {};
    virtual ~AbstractLock() {};
    virtual void Lock() = 0;        // your code should implement this func
    virtual void Unlock() = 0;      // your code should implement this func
    virtual std::string GetName() = 0;   // your code should implement this func
private:

};

} // namespace locktest

#endif
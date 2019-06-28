/* 
 * This is the spinlock that levelhashing uses, and the github url of levelhasing is:
 * https://github.com/Pfzuo/Level-Hashing. 
 * Spin lock using xchg
 * Copied from http://locklessinc.com/articles/locks/.
 */

#ifndef LOCK_TEST_ABSTRACT_LOCK_H_
#include "abstract_lock.h"
#endif

#include <string>
#include <iostream>

namespace locktest {

/* Compile read-write barrier */
#define barrier() asm volatile("": : :"memory")

/* Pause instruction to prevent excess processor bus usage */
#define cpu_relax() asm volatile("pause\n": : :"memory")

static inline unsigned short xchg_8(void *ptr, unsigned char x)
{
    __asm__ __volatile__("xchgb %0,%1"
                :"=r" (x)
                :"m" (*(volatile unsigned char *)ptr), "0" (x)
                :"memory");

    return x;
}

#define BUSY 1
typedef unsigned char spinlock;

#define SPINLOCK_INITIALIZER 0

static inline void spin_lock(spinlock *lock)
{
    while (1) {
        if (!xchg_8(lock, BUSY)) return;
    
        while (*lock) cpu_relax();
    }
}

static inline void spin_unlock(spinlock *lock)
{
    barrier();
    *lock = 0;
}

static inline int spin_trylock(spinlock *lock)
{
    return xchg_8(lock, BUSY);
}

class LevelhashingSpinlock: public AbstractLock {
public:
    LevelhashingSpinlock() {
        lock_ = SPINLOCK_INITIALIZER;    
    }
    ~LevelhashingSpinlock() {};
    void Lock();
    void Unlock();
    std::string GetName();
private:
    spinlock lock_;
};

void LevelhashingSpinlock::Lock() {
    spin_lock(&lock_);
}
void LevelhashingSpinlock::Unlock() {
    spin_unlock(&lock_);
}
std::string LevelhashingSpinlock::GetName() {
    return "LevelhashingSpinlock";
}

} // namespace locktest
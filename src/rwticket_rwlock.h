/*
 * Copied from: http://locklessinc.com/articles/locks/
 */

#ifndef LOCK_TEST_RWTICKET_RWLOCK_RW_LOCK_H_
#define LOCK_TEST_RWTICKET_RWLOCK_RW_LOCK_H_

#ifndef LOCK_TEST_ABSTRACT_RW_LOCK_H_
#include "abstract_rwlock.h"
#endif

#include <string>

/* Compile read-write barrier */
#define barrier() asm volatile("": : :"memory")
/* Pause instruction to prevent excess processor bus usage */
#define cpu_relax() asm volatile("pause\n": : :"memory")
#define atomic_xadd(P, V) __sync_fetch_and_add((P), (V))
#define atomic_inc(P) __sync_add_and_fetch((P), 1)

typedef union rwticket rwticket;

union rwticket
{
	unsigned u;
	unsigned short us;
	__extension__ struct
	{
		unsigned char write;
		unsigned char read;
		unsigned char users;
	} s;
};

namespace locktest {

class RwTicketRwLock: public AbstractRwLock {
public:
    RwTicketRwLock() {
        l = new rwticket();
    };
    ~RwTicketRwLock() {};
    void ReadLock();
    void ReadUnlock();
    void WriteLock();
    void WriteUnlock();
    std::string GetName();
private:
    rwticket *l;
};

void RwTicketRwLock::ReadLock() {
    unsigned me = atomic_xadd(&l->u, (1<<16));
	unsigned char val = me >> 16;
	
	while (val != l->s.read) cpu_relax();
	l->s.read++;
}

void RwTicketRwLock::ReadUnlock() {
    atomic_inc(&l->s.write);
}

void RwTicketRwLock::WriteLock() {
    unsigned me = atomic_xadd(&l->u, (1<<16));
	unsigned char val = me >> 16;
	
	while (val != l->s.write) cpu_relax();
}

void RwTicketRwLock::WriteUnlock() {
    rwticket t = *l;
	
	barrier();

	t.s.write++;
	t.s.read++;
	
	*(unsigned short *) l = t.us;
}

std::string RwTicketRwLock::GetName() {
    return "RwTicketRwLock";
}

} // namespace locktest

#endif
#ifndef _rA_ATOMIC_H_
#define _rA_ATOMIC_H_

// Atomic Operations 
// (Interlocked CompareExchange, Add .. and so on ..)
// 
// Implementation varies / depends on:
//	- Architecture
//	- Compiler
//	- Operating System
//
// our Abstraction is fully API-Compatible to Microsofts implementation @ NT5.0+
// 
#include "../common/cbasetypes.h"

#if defined(MSVC)
#include <Windows.h>

#elif defined(__GNUC__)

#if !defined(__x86_64__) && !defined(__i386__)
#error Your Target Platfrom is not supported
#endif

forceinline volatile long InterlockedExchangeAdd64(volatile long *addend, long increment){
	long ret;
	
	__asm__ __volatile__(
		"lock xadd %0, (%1)"
		:"=r" (ret)
		:"r" (addend), "0" (increment)
		:"memory"
	);
	
	return ret;
}//end: InterlockedExchangeAdd64()


forceinline volatile long InterlockedExchangeAdd(volatile int *addend, int increment){
	int ret;
	
	__asm__ __volatile__(
		"lock xaddl %0, (%1)"
		:"=r" (ret)
		:"r" (addend), "0" (increment)
		:"memory"	
	);
	
	return ret;
}//end: InterlockedExchangeAdd()


forceinline volatile long InterlockedIncrement64(volatile long *addend){
	return InterlockedExchangeAdd64(addend, 1)+1;
}//end: InterlockedIncrement64()


forceinline volatile int InterlockedIncrement(volatile int *addend){
	return InterlockedExchangeAdd(addend, 1)+1;
}//end: InterlockedIncrement()


forceinline volatile long InterlockedDecrement64(volatile long *addend){
	return InterlockedExchangeAdd64(addend, -1)-1;
}//end: InterlockedDecrement64()


forceinline volatile int InterlockedDecrement(volatile int *addend){
	return InterlockedExchangeAdd(addend, -1)-1;
}//end: InterlockedDecrement()


forceinline volatile long InterlockedCompareExchange64(volatile long *dest, long exch, long cmp){
	long old;
	
	__asm__ __volatile__(
		"lock cmpxchg %2, %0"
		:"=m" (*dest), "=a" (old)
		:"r"(exch), "m" (*dest), "a"(cmp)
	);
	
	return old;
}//end: InterlockedCompareExchange64()


forceinline volatile int InterlockedCompareExchange(volatile int *dest, int exch, int cmp){
	int old;
	
	__asm__ __volatile__(
		"lock cmpxchgl %2, %0"
		:"=m" (*dest), "=a" (old)
		:"r"(exch), "m" (*dest), "a"(cmp)
	);
	
	return old;
}//end: InterlockedCompareExchange()


forceinline volatile long InterlockedExchange64(volatile long *target, long val){
	int ret;
	
	__asm__ __volatile__(
		"lock xchg %2, (%1)"
		:"=r" (ret)
		:"r" (target), "0" (val)
		:"memory"
	);
	
	return ret;
}//end: InterlockedExchange64()


forceinline volatile int InterlockedExchange(volatile int *target, int val){
	int ret;
	
	__asm__ __volatile__(
		"lock xchgl %2, (%1)"
		:"=r" (ret)
		:"r" (target), "0" (val)
		:"memory"
	);
	
	return ret;
}//end: InterlockedExchange()


#endif //endif compiler decission


#endif

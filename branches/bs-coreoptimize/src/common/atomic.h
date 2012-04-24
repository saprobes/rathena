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

forceinline volatile int64 InterlockedExchangeAdd64(volatile int64 *addend, int64 increment){
	long ret;
	
	__asm__ __volatile__(
		"lock xadd %0, (%1)"
		:"=r" (ret)
		:"r" (addend), "0" (increment)
		:"memory"
	);
	
	return ret;
}//end: InterlockedExchangeAdd64()


forceinline volatile int32 InterlockedExchangeAdd(volatile int32 *addend, int32 increment){
	int ret;
	
	__asm__ __volatile__(
		"lock xaddl %0, (%1)"
		:"=r" (ret)
		:"r" (addend), "0" (increment)
		:"memory"	
	);
	
	return ret;
}//end: InterlockedExchangeAdd()


forceinline volatile int64 InterlockedIncrement64(volatile int64 *addend){
	return InterlockedExchangeAdd64(addend, 1)+1;
}//end: InterlockedIncrement64()


forceinline volatile int32 InterlockedIncrement(volatile int32 *addend){
	return InterlockedExchangeAdd(addend, 1)+1;
}//end: InterlockedIncrement()


forceinline volatile int64 InterlockedDecrement64(volatile int64 *addend){
	return InterlockedExchangeAdd64(addend, -1)-1;
}//end: InterlockedDecrement64()


forceinline volatile int32 InterlockedDecrement(volatile int32 *addend){
	return InterlockedExchangeAdd(addend, -1)-1;
}//end: InterlockedDecrement()


forceinline volatile int64 InterlockedCompareExchange64(volatile int64 *dest, int64 exch, int64 cmp){
	long old;
	
	__asm__ __volatile__(
		"lock cmpxchg %2, %0"
		:"=m" (*dest), "=a" (old)
		:"r"(exch), "m" (*dest), "a"(cmp)
	);
	
	return old;
}//end: InterlockedCompareExchange64()


forceinline volatile int32 InterlockedCompareExchange(volatile int32 *dest, int32 exch, int32 cmp){
	int old;
	
	__asm__ __volatile__(
		"lock cmpxchgl %2, %0"
		:"=m" (*dest), "=a" (old)
		:"r"(exch), "m" (*dest), "a"(cmp)
	);
	
	return old;
}//end: InterlockedCompareExchange()


forceinline volatile int64 InterlockedExchange64(volatile int64 *target, int64 val){
	int ret;
	
	__asm__ __volatile__(
		"lock xchg %2, (%1)"
		:"=r" (ret)
		:"r" (target), "0" (val)
		:"memory"
	);
	
	return ret;
}//end: InterlockedExchange64()


forceinline volatile int32 InterlockedExchange(volatile int32 *target, int32 val){
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

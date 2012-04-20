#ifndef _rA_SPINLOCK_H_
#define _rA_SPINLOCK_H_

//
// CAS based Spinlock Implementation
//
// CamelCase names are choosen to be consistent with microsofts winapi
// which implements CriticalSection by this naming...
//
// Author: Florian Wilemeyer <fw@f-ws.de>
//
// Copyright (c) rAthena Project (www.rathena.org) - Licensed under GNU GPL
// For more information, see LICENCE in the main folder
//
//
 
 
#include <sched.h> // for yield() 

#include "atomic.h"
#include "thread.h"

typedef struct SPIN_LOCK{
		volatile long lock;
		volatile long nest; // nesting level.
		
		volatile long sync_lock;
} __attribute__((aligned(64))) SPIN_LOCK, *PSPIN_LOCK;




__attribute__((always_inline))  inline void InitializeSpinLock(PSPIN_LOCK lck){
		lck->lock = 0;
		lck->nest = 0;
		lck->sync_lock = 0;
}

__attribute__((always_inline)) inline void FinalizeSpinLock(PSPIN_LOCK lck){
		return;
}


#define getsynclock(l) { while(1){ if(InterlockedCompareExchange64(l, 1, 0) == 0) break; sched_yield(); } }
#define dropsynclock(l) { InterlockedExchange64(l, 0); }

__attribute__((always_inline)) inline void EnterSpinLock(PSPIN_LOCK lck){
		int tid = rathread_get_tid();
		
		// Get Sync Lock && Check if the requester thread already owns the lock. 
		// if it owns, increase nesting level
		getsynclock(&lck->sync_lock);
		if(InterlockedCompareExchange64(&lck->lock, tid, tid) == tid){
				InterlockedIncrement64(&lck->nest);
				dropsynclock(&lck->sync_lock);
				return; // Got Lock
		}
		// drop sync lock
		dropsynclock(&lck->sync_lock);
		
		
		// Spin until we've got it ! 
		while(1){
				
				if(InterlockedCompareExchange64(&lck->lock, tid, 0) == 0){
						
						InterlockedIncrement64(&lck->nest);
						return; // Got Lock
				}
				
				sched_yield(); // Force ctxswitch to another thread.
		}

}


__attribute__((always_inline)) inline void LeaveSpinLock(PSPIN_LOCK lck){
		int tid = rathread_get_tid();

		getsynclock(&lck->sync_lock);
		
		if(InterlockedCompareExchange64(&lck->lock, tid, tid) == tid){ // this thread owns the lock.
			if(InterlockedDecrement64(&lck->nest) == 0)
					InterlockedExchange64(&lck->lock, 0); // Unlock!
		}
		
		dropsynclock(&lck->sync_lock);
}




//
#if 0
#include <pthread.h>
#include <stdio.h>
#define THRC 13
#define PERINC 100000
static volatile long done_threads = 0;
static int val = 0;
static SPIN_LOCK lock;

void *addProc(void *dat){
		int i;
		for(i = 0; i < PERINC; i++){
				EnterSpinLock(&lock);
				EnterSpinLock(&lock);
				val++;
//				printf("Thread %p %u \n", dat, val);
				LeaveSpinLock(&lock);
				LeaveSpinLock(&lock);
		}
		
		InterlockedIncrement64(&done_threads);				
}

int main(int c, char **v){
		pthread_t threads[THRC] = {0};
		int i; 
		volatile long result __attribute__(( aligned(64) ));

		InitializeSpinLock(&lock);

		for(i = 0; i < THRC; i++)
				pthread_create(&threads[i], NULL, addProc, (void*)i);
		
		while(1){
				if(InterlockedCompareExchange64(&done_threads, THRC, THRC) == THRC)	break;
				sched_yield();
		}
		
		printf("Done! - Result: %u\n", val);
		printf("      Expected: %u\n", PERINC*THRC);
		if(val != (THRC*PERINC)){
				printf("failed.\n");
				return 1;
		}else{
				printf("ok.\n");
		}

return 0;
}

#endif

#endif

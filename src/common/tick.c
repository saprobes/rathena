
#ifdef WIN32
#include <Windows.h>
#else
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h> // memset

#include "../common/cbasetypes.h"
#include "../common/tick.h"
#include "../common/thread.h"
#include "../common/showmsg.h"

////
// About this subsystem
//
// Windows:
//  upon server start, we're creating a thread which will tick every millisecond
//  and increases g_CurrentTick by one.
//
// Linux/BSD:
//  adding interval timer that causes one thread to interrupt every ms.
//
//
// I'm not going to use lock xadd opcodes, because:
//	Reading g_CurrentTick is an atomic operation, so
//	not synchronized access will result in not actual time 
//	which is acceptable, because we're talking about max -1ms
//
// 
// Author: Florian Wilkemeyer <fw@f-ws.de>
//
// Copyright (c) RAthena Project (www.rathena.org) - Licensed under GNU GPL
// For more information, see LICENCE in the main folder
//
////



// Global Tick Value
usysint g_CurrentTick = 0;


/////
// Subsystem Implementation:
//
static rAthread l_hThread = NULL;



static void *tick_workerProc(void *p){
	(void)p; // unused param.
#ifdef WIN32
	// WINDOWS Implementation

	while(1){
		Sleep(1);
		g_CurrentTick = GetTickCount();
	}

#else
	// Linux/BSD Implementation
	struct timespec ts;

	ts.tv_sec = 0;
	ts.tv_nsec = 1000000;

	while(1){
		//g_CurrentTick++;
		#if defined(HAVE_MONOTONIC_CLOCK)
			struct timespec tval;
			clock_gettime(CLOCK_MONOTONIC, &tval);
			g_CurrentTick = tval.tv_sec * 1000 + tval.tv_nsec / 1000000;
		#else
			struct timeval tval;
			gettimeofday(&tval, NULL);
			g_CurrentTick = tval.tv_sec * 1000 + tval.tv_usec / 1000;
		#endif
        
		nanosleep(&ts, NULL);
	}
	
#endif

	return NULL; //	
}//end: tick_workerProc()


void tick_init(){
	
	g_CurrentTick = 0;
	
	l_hThread = rathread_createEx(tick_workerProc, NULL, 1024*512, RAT_PRIO_HIGH);
	if(l_hThread == NULL){
		ShowFatalError("tick_init: cannot spawn worker Thread - aborting.\n");
		exit(EXIT_FAILURE);
	}
	
}//end: tick_init()



void tick_final(){
	
	// Simple handling => kill the thread ~
	if(l_hThread != NULL){
		rathread_destroy(l_hThread);
		l_hThread = NULL;
	}
	
}//end: tick_final()



unsigned int gettick(){
	return g_CurrentTick;
}

unsigned int gettick_nocache(){
	return g_CurrentTick;
}


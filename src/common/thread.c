
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "cbasetypes.h"
#include "malloc.h"
#include "showmsg.h"
#include "thread.h"

#ifdef WIN32
#error implement me!
#else
#include <pthread.h>
#endif



struct rAthread {
	struct rAthread *next, *prev; 
	
	RATHREAD_PRIO  prio;
	rAthreadProc proc;
	void *param; 

	#ifdef WIN32
	#else
	pthread_t hThread;
	#endif
};


///
/// Subystem Code
///
static rAthread l_ThreadListBegin = NULL;

void rAthread_init(){
	// Nothing todo ~ 
	
}//end: rAthread_init()


void rAthread_final(){
	rAthread handle;

	// Unterminated Threads Left? 
	// Should'nt happen ..
	// Kill 'em all! 
	//
	_getone:
	// @TODO lock!
	handle = l_ThreadListBegin;
	// @TODO unlock!
	
	if(handle != NULL){
		ShowWarning("rAthread_final: unterminated Thread (entryPoint %p) - forcing to terminat (kill)\n", handle->proc);
		rAthreadDestroy(handle);
		goto _getone;
	}
	
	
	
}//end: rAthread_final()


static rAthread rat_pool_get(){
	void *dat = aMalloc( sizeof(struct rAthread) );
	
	if(dat == NULL){
		ShowFatalError("rAthread: cannot allocate new rAthread structure (%u bytes)\n", sizeof(struct rAthread));
		exit(EXIT_FAILURE);
	}

	memset(dat, 0x00, sizeof(struct rAthread) );
	
	return (rAthread)dat;
}//end: rat_pool_get()

static void rat_pool_put( rAthread handle ){
	aFree(handle);
}//end: rat_pool_put()


// gets called whenever a thread terminated ..
static void rat_thread_terminated( rAthread handle ){

	// Unlink from thread list
	// free ressources.
	//
	
	// @TODO lock!
	if(handle == l_ThreadListBegin)
		l_ThreadListBegin = handle->next;
		
	if(handle->next != NULL)
		handle->next->prev = handle->prev;
	
	if(handle->prev != NULL)
		handle->prev->next = handle->next;
	// @TODO unlock!

	// Free..
	rat_pool_put(handle);
	

}//end: rat_thread_terminated()


static void *_raThreadMainRedirector( void *p ){
	void *ret;

	ret = ((rAthread)p)->proc( ((rAthread)p)->param ) ;
	
	rat_thread_terminated( (rAthread)p );

	return ret;
}//end: _raThreadMainRedirector()





///
/// API Level
/// 
rAthread rAthreadCreate( rAthreadProc entryPoint,  void *param ){
	return rAthreadCreateEx( entryPoint, param,  (1<<23) /*8MB*/,  RAT_PRIO_NORMAL );
}//end: rAthreadCreate()


rAthread rAthreadCreateEx( rAthreadProc entryPoint,  void *param,  size_t szStack,  RATHREAD_PRIO prio ){
	pthread_attr_t attr;
	size_t tmp;
	rAthread handle;


	// given stacksize aligned to systems pagesize?
	tmp = szStack % getpagesize();
	if(tmp != 0)
		szStack += tmp;


	handle = rat_pool_get();
	handle->proc = entryPoint;
	handle->param = param;
	
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, szStack);
	
	if(pthread_create(&handle->hThread, &attr, _raThreadMainRedirector, (void*)handle) != 0){
		rat_pool_put(handle);
		return NULL;
	}
	
	pthread_attr_destroy(&attr);

	rAthreadSetPrio( handle,  prio );
	
	// @TODO lock!
	handle->next = l_ThreadListBegin;
	if(l_ThreadListBegin != NULL)
		l_ThreadListBegin->prev = handle;
	
	handle->prev = NULL;
	l_ThreadListBegin = handle;
	// @TODO unlock!
	
	return handle;
}//end: rAthreadCreateEx()



void rAthreadDestroy ( rAthread handle ){
	
	if( pthread_cancel( handle->hThread ) == 0){
	
		// We have to join it, otherwise pthread wont re-cycle its internal ressources assoc. with this thread.
		// 
		pthread_join( handle->hThread, NULL );
		
		
		// Tell our manager to release ressources ;)
		rat_thread_terminated(handle);
	}
	
}//end: rAthreadDestroy()


rAthread rAthreadGetSelf( ){
	pthread_t pt;
	rAthread it;
	
	pt = pthread_self();
	
	for(it = l_ThreadListBegin; it != NULL; it = it->next)
		if(it->hThread == pt)
			return it; 

	return NULL;	
}//end: rAthreadGetSelf()



bool rAthreadWait( rAthread handle,  RATHREAD_WAITMODE  mode,  void* *out_exitCode ){
	
	// Hint:
	// no thread data cleanup routine call here!
	// its managed by the callProxy itself..
	//
	
	if(mode == RAT_WAIT){
		if(pthread_join(handle->hThread, out_exitCode) == 0)
			return true;
	}
	// NO WAIT is not supported with pthread.
	
	return false;
}//end: rAthreadWait()


void rAthreadSetPrio( rAthread handle, RATHREAD_PRIO prio ){
	// @TODO
}//end: rAthreadSetPrio()


RATHREAD_PRIO rAthreadGetPrio( rAthread handle){
	// @TODO
	return RAT_PRIO_NORMAL;
}//end: rAthreadGetPrio()

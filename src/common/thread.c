
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

void rathread_init(){
	// Nothing todo ~ 
	
}//end: rathread_init()


void rathread_final(){
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
		rathread_destroy(handle);
		goto _getone;
	}
	
	
	
}//end: rathread_final()


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
rAthread rathread_create( rAthreadProc entryPoint,  void *param ){
	return rathread_createEx( entryPoint, param,  (1<<23) /*8MB*/,  RAT_PRIO_NORMAL );
}//end: rathread_create()


rAthread rathread_createEx( rAthreadProc entryPoint,  void *param,  size_t szStack,  RATHREAD_PRIO prio ){
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

	rathread_prio_set( handle,  prio );
	
	// @TODO lock!
	handle->next = l_ThreadListBegin;
	if(l_ThreadListBegin != NULL)
		l_ThreadListBegin->prev = handle;
	
	handle->prev = NULL;
	l_ThreadListBegin = handle;
	// @TODO unlock!
	
	return handle;
}//end: rathread_createEx


void rathread_destroy ( rAthread handle ){
	
	if( pthread_cancel( handle->hThread ) == 0){
	
		// We have to join it, otherwise pthread wont re-cycle its internal ressources assoc. with this thread.
		// 
		pthread_join( handle->hThread, NULL );
		
		
		// Tell our manager to release ressources ;)
		rat_thread_terminated(handle);
	}
	
}//end: rathread_destroy()

rAthread rathread_self( ){
	pthread_t pt;
	rAthread it;
	
	pt = pthread_self();
	
	for(it = l_ThreadListBegin; it != NULL; it = it->next)
		if(it->hThread == pt)
			return it; 

	return NULL;	
}//end: rathread_self()


bool rathread_wait( rAthread handle,  void* *out_exitCode ){
	
	// Hint:
	// no thread data cleanup routine call here!
	// its managed by the callProxy itself..
	//
	
	if(pthread_join(handle->hThread, out_exitCode) == 0)
		return true;
	
	return false;
}//end: rathread_wait()


void rathread_prio_set( rAthread handle, RATHREAD_PRIO prio ){
	// @TODO
}//end: rathread_prio_set()


RATHREAD_PRIO rathread_prio_get( rAthread handle){
	// @TODO
	return RAT_PRIO_NORMAL;
}//end: rathread_prio_get()


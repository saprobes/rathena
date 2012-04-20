
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

#define RA_THREADS_MAX 64

struct rAthread {
	unsigned int myID;
	
	RATHREAD_PRIO  prio;
	rAthreadProc proc;
	void *param; 

	#ifdef WIN32
	#else
	pthread_t hThread;
	#endif
};


__thread int g_rathread_ID = -1;


///
/// Subystem Code
///
static struct rAthread l_threads[RA_THREADS_MAX];

void rathread_init(){
	register unsigned int i;
	memset(&l_threads, 0x00, RA_THREADS_MAX * sizeof(struct rAthread) );
	
	for(i = 0; i < RA_THREADS_MAX; i++){
		l_threads[i].myID = i;
	}

	// now lets init thread id 0, which represnts the main thread
	g_rathread_ID = 0;
	l_threads[0].prio = RAT_PRIO_NORMAL;
//	l_threads[0].proc = (rAthreadProc)main;
	
		
}//end: rathread_init()



void rathread_final(){
	register unsigned int i;
	
	// Unterminated Threads Left? 
	// Should'nt happen ..
	// Kill 'em all! 
	//
	for(i = 1; i < RA_THREADS_MAX; i++){
		if(l_threads[i].proc != NULL){
			ShowWarning("rAthread_final: unterminated Thread (tid %u entryPoint %p) - forcing to terminat (kill)\n", i, l_threads[i].proc);
			rathread_destroy(&l_threads[i]);
		}
	}
	
	
}//end: rathread_final()



// gets called whenever a thread terminated ..
static void rat_thread_terminated( rAthread handle ){

	int id_backup = handle->myID;

	// Simply set all members to 0 (except the id)
	memset(handle, 0x00, sizeof(struct rAthread));
	
	handle->myID = id_backup; // done ;)

}//end: rat_thread_terminated()


static void *_raThreadMainRedirector( void *p ){
	void *ret;
	
	// Update myID @ TLS to right id.
	g_rathread_ID = ((rAthread)p)->myID; 

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
	unsigned int i;
	pthread_attr_t attr;
	size_t tmp;
	rAthread handle = NULL;


	// given stacksize aligned to systems pagesize?
	tmp = szStack % getpagesize();
	if(tmp != 0)
		szStack += tmp;


	// Get a free Thread Slot. 
	for(i = 0; i < RA_THREADS_MAX; i++){
		if(l_threads[i].proc == NULL){
			handle = &l_threads[i];
			break;
		}
	}
	
	if(handle == NULL){
		ShowError("rAthread: cannot create new thread (entryPoint: %p) - no free thread slot found!", entryPoint);
		return NULL;
	}
	
	
	
	handle->proc = entryPoint;
	handle->param = param;
	
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, szStack);
	
	if(pthread_create(&handle->hThread, &attr, _raThreadMainRedirector, (void*)handle) != 0){
		handle->proc = NULL;
		handle->param = NULL;
		return NULL;
	}
	
	pthread_attr_destroy(&attr);

	rathread_prio_set( handle,  prio );
	
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
	rAthread handle = &l_threads[g_rathread_ID];
	
	if(handle->proc != NULL) // entry point set, so its used!	
		return handle;
		
	return NULL;	
}//end: rathread_self()


int rathread_get_tid(){
	
	return g_rathread_ID;
	
}//end: rathread_get_tid()


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


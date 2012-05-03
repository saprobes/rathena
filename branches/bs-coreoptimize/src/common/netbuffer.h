#ifndef _rA_NETBUFFER_H_
#define _rA_NETBUFFER_H_

#include "../common/cbasetypes.h"

typedef struct netbuf{
	sysint	pool;				// The pool ID this buffer belongs to,
								// is set to -1 if its an emergency allocated buffer 

	volatile int32 refcnt;		// Internal Refcount, it gets lowered every call to netbuffer_put, 
								// if its getting zero, the buffer will returned back to the pool
								// and can be reused.

	// The Bufferspace itself.
	char buf[32];
} *netbuf;


void netbuffer_init();
void netbuffer_final();

/**
 * Gets a netbuffer that has atleast (sz) byes space.
 *
 * @note: The netbuffer system guarantees that youll always recevie a buffer.
 *			no check for null is required!
 *
 * @param sz - minimum size needed.
 *
 * @return pointer to netbuf struct
 */
netbuf netbuffer_get( sysint sz );


/** 
 * Returns the given netbuffer (decreases refcount, if its 0 - the buffer will get returned to the pool)
 *
 * @param buf - the buffer to return 
 */
void netbuffer_put( netbuf buf );


/** 
 * Increases the Refcount on the given buffer 
 * (used for areasends .. etc)
 *
 */
void netbuffer_incref( netbuf buf );


#endif

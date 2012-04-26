
#include <stdio.h>
#include <time.h>

#include "../common/cbasetypes.h"
#include "../common/showmsg.h"
#include "../common/mempool.h"
#include "../common/atomic.h"

#define ALIGN16	ra_align(16)
#define ALIGN_TO(x, a) (x + ( a - ( x % a) ) )
#define ALIGN_TO_16(x)	ALIGN_TO(x, 16)



#define MEMPOOLASSERT

struct node{
	void 	*next;
	void 	*segment;
#ifdef MEMPOOLASSERT
	bool	used;
	uint32	magic;
	#define NODE_MAGIC 0xBEEF00EAEACAFE07 
#endif
};


// The Pointer to this struct is the base address of the segment itself.
struct pool_segment{
	void	*pool; // pool, this segment belongs to
	int32	num_nodes_total;	
	int64	num_bytes;
};


struct mempool{
	// Settings
	char *name;
	uint64	elem_size;
	uint64	elem_realloc_step;
	
	// Callbacks that get called for every node that gets allocated
	// Example usage: initialization of mutex/lock for each node.
	memPoolOnNodeAllocationProc		*onalloc;
	memPoolOnNodeDeallocationProc	*ondealloc;


	// Internal 
	struct pool_segment *segments;
	struct node	*free_list;
		
	volatile int32  num_nodes_total;
	volatile int32	num_nodes_free;

	volatile int32	num_pools;
	volatile int64	num_bytes_total;
	
} ra_align(8); // Dont touch the alignment, otherwise interlocked functions are broken ..


void mempool_init(){
}//end: mempool_init()


void mempool_final(){
}//end: mempool_final()





mempool mempool_create(const char *name,
						uint64 elem_size,
						uint64 initial_count,
						uint64 realloc_count,
						memPoolOnNodeAllocationProc *onNodeAlloc,
						memPoolOnNodeDeallocationProc *onNodeDealloc){
	//..
	
}//end: mempool_create()



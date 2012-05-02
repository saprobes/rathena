#ifndef _rA_MEMPOOL_H_
#define _rA_MEMPOOL_H_

#include "../common/cbasetypes.h"

typedef struct mempool *mempool;

typedef void (*memPoolOnNodeAllocationProc)(void *ptr);
typedef void (*memPoolOnNodeDeallocationProc)(void *ptr);

void mempool_init();
void mempool_final();



/** 
 * Creates a new Mempool
 *
 * @param name - Name of the pool (used for debug / error messages)
 * @param elem_size - size of each element
 * @param initial_count - preallocation count 
 * @param realloc_count - #no of nodes being allocated when pool is running empty.
 * @param onNodeAlloc - Node Allocation callback (see @note!)
 * @param onNodeDealloc - Node Deallocation callback (see @note!)
 *
 * @note:
 *	The onNode(De)alloc callbacks are only called once during segment allocation 
 *	(pool initialization / rallocation )
 *  you can use this callbacks for example to initlaize a mutex or somethingelse
 *	you definitly need during runtime
 *
 * @return not NULL
 */
mempool mempool_create(const char *name,
						uint64 elem_size,
						uint64 initial_count,
						uint64 realloc_count,
						
						memPoolOnNodeAllocationProc	onNodeAlloc,
						memPoolOnNodeDeallocationProc onNodeDealloc);
						
						
/**
 * Destroys a Mempool
 * 
 * @param pool - the mempool to destroy
 *
 * @note:
 *	Everything gets deallocated, regardless if everything was freed properly!
 *	So you have to ensure that all references are cleared properly!
 */
void mempool_destroy(mempool pool);


/**
 * Gets a new / empty node from the given mempool.
 * 
 * @param pool - the pool to get an empty node from.
 *
 * @return Address of empty Node
 */
void *mempool_node_get(mempool pool);


/**
 * Returns the given node to the given mempool
 *
 * @param pool - the pool to put the node, to
 * @param node - the node to return 
 */
void mempool_node_put(mempool pool, void *node);





#endif

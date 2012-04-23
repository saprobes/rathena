#ifndef _rA_TICK_H_
#define _rA_TICK_H_

#include "../common/cbasetypes.h"

// Global value which helds the current tick count - resolution is: ms
extern usysint g_CurrentTick; 

unsigned int gettick();
unsigned int gettick_nocache();

// 
// Public Subsystem functions
void tick_init();
void tick_final();



#endif

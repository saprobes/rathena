// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _LUASCRIPT_H_
#define _LUASCRIPT_H_

extern int potion_flag; //For use on Alchemist improved potions/Potion Pitcher. [Skotlex]
extern int potion_hp, potion_per_hp, potion_sp, potion_per_sp;
extern int potion_target;

int lua_eventtimer[MAX_EVENTTIMER];
unsigned short lua_eventcount;

int menu_countoptions(const char* str, int max_count, int* total);

void luascript_run(const char *name,int char_id,const char *format,...);
void script_run_chunk(const char *chunk,int char_id);
void script_resume(struct map_session_data *sd,const char *format,...);

//extern enum { L_NRUN, L_CLOSE, L_NEXT, L_INPUT, L_MENU, L_END };

/* 
NOT_RUNNING = Currently not running any command that waits for user input.

IN MENU, NEXT_BUTTON, CLOSE_BUTTON, INPUT_WINDOW = Should be self-explanatory.

YIELD = Any other script pausing code uses this flag.
*/
enum { NOT_RUNNING, IN_MENU, YIELD, NEXT_BUTTON, CLOSE_BUTTON, INPUT_WINDOW };

#endif /* _LUASCRIPT_H_ */
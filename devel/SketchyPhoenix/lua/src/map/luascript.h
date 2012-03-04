// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _LUASCRIPT_H_
#define _LUASCRIPT_H_

int lua_eventtimer[MAX_EVENTTIMER];
unsigned short lua_eventcount;

int menu_countoptions(const char* str, int max_count, int* total);

void script_run_function(const char *name,int char_id,const char *format,...);
void script_run_chunk(const char *chunk,int char_id);
void script_resume(struct map_session_data *sd,const char *format,...);

extern enum { L_NRUN, L_CLOSE, L_NEXT, L_INPUT, L_MENU, L_END };

#endif /* _LUASCRIPT_H_ */
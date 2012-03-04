// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "../common/luaengine.h"
#include "../common/showmsg.h"

#include <stdio.h>
#include <lauxlib.h>
#include <lualib.h>

static struct LuaCommandInfo commands[] = {
	{"-End of list-", NULL},
};

// load commands into the engine
void load_script_commands()
{
	int i=0;
	
	for( i = 0; commands[i].f; i++ )
	{
		lua_pushstring(L, commands[i].command);
        lua_pushcfunction(L, commands[i].f);
        lua_settable(L, LUA_GLOBALSINDEX);
	}
	
	ShowStatus("Done registering '"CL_WHITE"%d"CL_RESET"' script build-in commands.\n",i);
}
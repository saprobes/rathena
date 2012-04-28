// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "luaengine.h"

#include <stdio.h>
#include <lauxlib.h>
#include <lualib.h>

/*
	Create our Lua environment. Each server has its own global Lua state.
*/
int do_init_luaengine()
{
	L = lua_open();
	luaL_openlibs(L);
	lua_rawset(L,LUA_GLOBALSINDEX);
	load_script_commands();
	return 0;
}

/*
	Clean-up function. Pretty simple.
*/
void do_final_luaengine()
{
	lua_close(L);
}

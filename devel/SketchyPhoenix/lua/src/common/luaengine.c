// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "../common/showmsg.h"

#include "luaengine.h"

#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/*
	Create our Lua environment. Each server has its own global Lua state.
*/
void do_init_luaengine()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	load_script_commands();
	
	if ( luaL_dofile(L,"script/map/script_main.lua") )
		ShowError("%s\n", lua_tostring(L,-1));
	
}

/*
	Clean-up function. Pretty simple.
*/
void do_final_luaengine()
{
	lua_close(L);
}
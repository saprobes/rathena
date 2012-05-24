// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "../common/showmsg.h"

#include "luaengine.h"

#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void luaengine_stackdump (lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		
		case LUA_TSTRING:  /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;
			
		case LUA_TBOOLEAN:  /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
            break;
    
		case LUA_TNUMBER:  /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
    
		default:  /* other values */
			printf("%s", lua_typename(L, t));
			break;
    
		}
		printf("  ");  /* put a separator */
	}
		printf("\n");  /* end the listing */
}

/*
	Create our Lua environment. Each server has its own global Lua state.
*/
void do_init_luaengine()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	load_script_commands();
	luaengine_open_config();
}

/*
	Clean-up function. Pretty simple.
*/
void do_final_luaengine()
{
	lua_close(L);
}
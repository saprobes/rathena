// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "../common/luaengine.h"
#include "../common/showmsg.h"

#include <stdio.h>
#include <lauxlib.h>
#include <lualib.h>

LUA_FUNC(stackDump)
{
	luaengine_stackdump(L);
	return 0;
}

static struct LuaCommandInfo commands[] = {
	LUA_DEF(stackDump),
	{"-End of list-", NULL},
};

void luaengine_open_config()
{
	if ( luaL_dofile(L,"script/char/script_main.lua") )
		ShowError("%s\n", lua_tostring(L,-1));
}

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

void charscript_run(const char *name,const char *format,...)
{
	va_list arg;
	int n=0;

	lua_getglobal(L,name);
	
	/*	Initialize argument list
		Pass arguments according to types defined by "format"
		d = double, i = integer, s = string
	*/
	va_start(arg,format);
	while (*format) 
	{
        switch (*format++) 
		{
          case 'd':
            lua_pushnumber(L,va_arg(arg,double));
            break;
          case 'i':
            lua_pushnumber(L,va_arg(arg,int));
            break;
          case 's':
            lua_pushstring(L,va_arg(arg,char*));
            break;
          default:
            ShowError("%c : Invalid argument type code, allowed codes are 'd'/'i'/'s'\n",*(format-1));
        }
        n++;
        luaL_checkstack(L,1,"Too many arguments");
    }
	va_end(arg);

	if ( lua_resume(L,n) !=0 && lua_tostring(L,-1) != NULL )
	{
		ShowError("Cannot run function %s : %s\n",name,lua_tostring(L,-1));
		return;
	}
	
}
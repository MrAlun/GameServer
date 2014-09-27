// testlua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>


#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

lua_State* L;
static int average(lua_State *L)
{
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	for (i = 1; i <= n; i++)
	{
		sum += lua_tonumber(L, i);
	}

	lua_pushnumber(L, sum / n);

	lua_pushnumber(L, sum);

	return 2;
}

int _tmain(int argc, _TCHAR* argv[])
{
// 	lua_State *L = lua_open();
// 	luaL_openlibs(L);
// 
// 	const char *buf = "print('Hello World')";
// 	luaL_dostring(L,buf);
// 
// 	lua_close(L);

// 	lua_State *L = lua_open();
// 	luaL_openlibs(L);
// 	luaL_dofile(L, "test.lua");
// 	lua_close(L);
// 
// 	while (1)
// 	{
// 
// 	}	/* 初始化Lua */
	L = luaL_newstate();
	/* 载入Lua基本库 */
	luaL_openlibs(L);
	/* 注册函数 */
	lua_register(L, "average", average);
	/* 运行脚本 */
	luaL_dofile(L, "avg.lua");
	/* 清除Lua */
	lua_close(L);

	/* 暂停 */
	printf( "Press enter to exit…" );
	getchar();

	return 0;
}
#include <stdio.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/luaxlib.h>

static int func( lua_State* L )
{
	printf( "call func" );
	printf( "call func" );
	printf( "call func" );
	printf( "call func" );
	printf( "call func" );
	printf( "call func" );
	return 0;
}

int main( int argc, char** argv )
{
	lua_State* L = lua_newstate();
	lual_openlibs( L );
	lua_register( L, "func", func );
	luaL_dofile( L, "test6.lua" );
	lua_close( L );
	return 0;
}

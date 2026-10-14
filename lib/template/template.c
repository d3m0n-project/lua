#include <lua.h>
#include <lauxlib.h>

#include "types.h"
#include "sys.h"
#include "stdlib.h"

// an example of a simple hello world function
// seek for the lua api documentation for further help

// test() -> prints 'Hello World!'
static int l_test(lua_State *L)
{
	write(1, "Hello World!\n", 14);
	return 0;
}

// list of all module functions and their definitions
static const luaL_Reg my_functions[] = {
	{"test",			l_test},
	{NULL, NULL}
};


// module loader function
int	luaopen_test_native(lua_State *L)
{
	luaL_newlib(L, my_functions);
	return 1;
}
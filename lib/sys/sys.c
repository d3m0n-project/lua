#include <lua.h>
#include <lauxlib.h>

#include "types.h"
#include "sys.h"
#include "stdlib.h"

static int l_exit(lua_State *L)
{
	int ret_code = (int)luaL_checkinteger(L, 1);
	exit(ret_code);
	return 0;
}

static const luaL_Reg sys_functions[] = {
	{"exit", l_exit},
	{NULL, NULL}
};

int luaopen_sys_native(lua_State *L)
{
	size_t count = sizeof(sys_functions) / sizeof(sys_functions[0]) - 1;

	if (TOTAL_SYSCALL_COUNT != count)
	{
		const char *err_msg = "\033[34mWARNING\033[0m: Some syscall definitions are missing in lib sys\n";
		write(2, err_msg, strlen(err_msg));
	}

	luaL_newlib(L, sys_functions);
	return 1;
}
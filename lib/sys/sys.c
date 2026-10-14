#include <lua.h>
#include <lauxlib.h>

#include "types.h"
#include "sys.h"

static int	l_uptime(lua_State *L)
{
	uint64_t ticks = kernel_uptime_ticks();

	lua_pushinteger(L, (lua_Integer)ticks);
	return 1;
}

static const luaL_Reg sys_functions[] = {
	{"uptime", l_uptime},
	{NULL, NULL}
};

int	luaopen_sys_native(lua_State *L)
{
	luaL_newlib(L, sys_functions);
	return 1;
}
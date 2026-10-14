#include <lua.h>
#include <lauxlib.h>
#include "sys.h"

// function to set a value of a uint32_t *buffer
int	l_c_buffer_uint32_set(lua_State *L)
{
    uint32_t *addr = lua_touserdata(L, 1);
    lua_Integer offset = luaL_checkinteger(L, 2);
    uint32_t value = (uint32_t)luaL_checkinteger(L, 3);
    if (!addr)
        return luaL_argerror(L, 1, "invalid buffer addr");
	if (offset < 0)
		return luaL_argerror(L, 1, "invalid buffer offset");

    addr[(size_t)offset] = value;

    return 0;
}
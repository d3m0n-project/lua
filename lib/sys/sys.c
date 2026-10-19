#include <lua.h>
#include <lauxlib.h>

#include "types.h"
#include "sys.h"
#include "stdlib.h"

#include "c_functions.c"

static int l_exit(lua_State *L)
{
	int ret_code = (int)luaL_checkinteger(L, 1);
	exit(ret_code);
	return 0;
}


static int	l_read(lua_State *L)
{
	int fd = (int)luaL_checkinteger(L, 1);
	size_t count = (size_t)luaL_checkinteger(L, 2);

	char *buffer = malloc(count ? count : 1);
	if (!buffer)
		return luaL_error(L, "out of memory");

	int ret = read(fd, buffer, (uint32_t)count);
	if (ret < 0)
	{
		free(buffer);
		lua_pushnil(L);
		lua_pushinteger(L, ret);
		return 2;
	}

	lua_pushlstring(L, buffer, (size_t)ret);
	lua_pushinteger(L, ret);
	free(buffer);
	return 2;
}

static int	l_write(lua_State *L)
{
	int fd = (int)luaL_checkinteger(L, 1);
	size_t len;
	const char *data = luaL_checklstring(L, 2, &len);

	lua_pushinteger(L, write(fd, data, (uint32_t)len));
	return 1;
}

static int	l_open(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	uint32_t flags = (uint32_t)luaL_checkinteger(L, 2);
	lua_pushinteger(L, open(path, flags));
	return 1;
}

static int	l_close(lua_State *L)
{
	lua_pushinteger(L, close((int)luaL_checkinteger(L, 1)));
	return 1;
}

static int	l_lseek(lua_State *L)
{
	size_t offset = (size_t)luaL_checkinteger(L, 2);
	int fd = (int)luaL_checkinteger(L, 1);
	int whence = (int)luaL_checkinteger(L, 3);

	lua_pushinteger(L, lseek(fd, offset, (e_seek_directive)whence));
	return 1;
}

static int	l_uname(lua_State *L)
{
	size_t len;
	const char *unused = NULL;
	(void)unused;

	lua_Integer n = luaL_checkinteger(L, 2);
	if (n < 0 || (lua_Unsigned)n > UINT32_MAX)
		return luaL_argerror(L, 2, "invalid buffer length");

	len = (size_t)n;
	char *buffer = malloc(len ? len : 1);
	if (!buffer)
		return luaL_error(L, "out of memory");

	int ret = uname(buffer, (uint32_t)len);

	if (ret < 0)
		lua_pushnil(L);
	else
		lua_pushlstring(L, buffer, len);

	lua_pushinteger(L, ret);
	free(buffer);
	return 2;
}

static int l_getdents(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	lua_Integer n = luaL_checkinteger(L, 2);
	if (n < 0 || (lua_Unsigned)n > UINT32_MAX)
		return luaL_argerror(L, 2, "invalid buffer length");

	size_t len = (size_t)n;
	char *buffer = malloc(len ? len : 1);
	if (!buffer)
		return luaL_error(L, "out of memory");

	int ret = getdents(path, buffer, (uint32_t)len);
	if (ret < 0)
		lua_pushnil(L);
	else
		lua_pushlstring(L, buffer, (size_t)ret);

	lua_pushinteger(L, ret);
	free(buffer);
	return 2;
}

static int l_rename(lua_State *L)
{
	lua_pushinteger(L, rename(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
	return 1;
}

static int l_mkdir(lua_State *L)
{
	lua_pushinteger(L, mkdir(luaL_checkstring(L, 1)));
	return 1;
}

static int l_rmdir(lua_State *L)
{
	lua_pushinteger(L, rmdir(luaL_checkstring(L, 1)));
	return 1;
}

static int l_remove(lua_State *L)
{
	lua_pushinteger(L, remove(luaL_checkstring(L, 1)));
	return 1;
}


static int	l_surface_create(lua_State *L)
{
	uint32_t width = (uint32_t)luaL_checkinteger(L, 1);
	uint32_t height = (uint32_t)luaL_checkinteger(L, 2);

	uint8_t *surface = NULL;
	int pitch = 0;

	int ret = surface_create(width, height, &surface, &pitch);
	lua_pushlightuserdata(L, surface);
	lua_pushinteger(L, pitch);
	lua_pushinteger(L, ret);
	return 3;
}

static int	l_surface_update(lua_State *L)
{
	uint8_t *surface = (uint8_t *)lua_touserdata(L, 1);
	if (!surface)
		return luaL_argerror(L, 1, "expected surface pointer");

	lua_pushinteger(L, surface_update(surface));
	return 1;
}

static int l_sbrk(lua_State *L)
{
	size_t increment = (size_t)luaL_checkinteger(L, 1);
	lua_pushinteger(L, (int)sbrk(increment));
	return 1;
}

static int l_time(lua_State *L)
{
	void *timezone = NULL;
	if (!lua_isnoneornil(L, 1))
		timezone = lua_touserdata(L, 1);

	lua_pushinteger(L, time(timezone));
	return 1;
}

static int l_clock(lua_State *L)
{
	lua_pushinteger(L, clock());
	return 1;
}

static int l_random_u32(lua_State *L)
{
	lua_pushinteger(L, (lua_Integer)random_u32());
	return 1;
}

static int l_exec(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	int argc = (int)luaL_len(L, 2);
	const char **argv = calloc((size_t)argc + 1, sizeof(*argv));
	if (!argv)
		return luaL_error(L, "out of memory");

	for (int i = 0; i < argc; i++)
	{
		lua_geti(L, 2, i + 1);
		argv[i] = luaL_checkstring(L, -1);
		lua_pop(L, 1);
	}
	uint32_t ret = exec(path, argv);
	free(argv);
	lua_pushinteger(L, (lua_Integer)ret);
	return 1;
}

static int	l_kill(lua_State *L)
{
	uint32_t pid = (uint32_t)luaL_checkinteger(L, 1);
	int code = (int)luaL_checkinteger(L, 2);
	lua_pushinteger(L, (lua_Integer)kill(pid, code));
	return 1;
}

static int	l_pstate(lua_State *L)
{
	uint32_t pid = (uint32_t)luaL_checkinteger(L, 1);

	lua_pushinteger(L, (lua_Integer)pstate(pid));
	return 1;
}

static int	l_evtpop(lua_State *L)
{
	event_t out = {0};
	int ret = evtpop(&out);
	if (ret != 0) {
		lua_pushnil(L);
		return 1;
	}

	// [type, timestamp, x, y]
	lua_createtable(L, 0, 4);
	lua_pushinteger(L, out.type);
	lua_setfield(L, -2, "type");
	lua_pushinteger(L, (lua_Integer)out.timestamp);
	lua_setfield(L, -2, "timestamp");
	lua_pushinteger(L, out.x);
	lua_setfield(L, -2, "x");
	lua_pushinteger(L, out.y);
	lua_setfield(L, -2, "y");
	return 1;
}

static const luaL_Reg sys_functions[] = {
	{"exit",			l_exit},
	{"read",			l_read},
	{"write",			l_write},
	{"open",			l_open},
	{"close",			l_close},
	{"lseek",			l_lseek},
	{"uname",			l_uname},
	{"getdents",		l_getdents},
	{"rename",			l_rename},
	{"mkdir",			l_mkdir},
	{"rmdir",			l_rmdir},
	{"remove",			l_remove},
	{"surface_create",	l_surface_create},
	{"surface_update",	l_surface_update},
	{"sbrk",			l_sbrk},
	{"time",			l_time},
	{"clock",			l_clock},
	{"random_u32",		l_random_u32},
	{"exec",			l_exec},
	{"kill",			l_kill},
	{"pstate",			l_pstate},
	{"evtpop",			l_evtpop},
	{NULL, NULL}
};

static const luaL_Reg c_functions[] = {
	{"c_buffer_uint32_set", l_c_buffer_uint32_set},
	{NULL, NULL}
};

int	luaopen_sys_native(lua_State *L)
{
	size_t count = sizeof(sys_functions) / sizeof(sys_functions[0]) - 1;
	if (TOTAL_SYSCALL_COUNT != count)
	{
		const char *err_msg = "\033[33mWARNING\033[0m: Some syscall definitions are missing in lib sys\n";
		write(2, err_msg, strlen(err_msg));
	}

	luaL_newlib(L, sys_functions);
	luaL_setfuncs(L, c_functions, 0);
	return 1;
}
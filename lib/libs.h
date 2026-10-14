#ifndef CUSTOM_LUA_LIBS
#define CUSTOM_LUA_LIBS

#include "../lua.h"
#include "../lauxlib.h"

// sys
int	luaopen_sys_native(lua_State *L);

typedef struct
{
	lua_CFunction	reg_fn;
	const char		*name;
} t_module_loading_struct;

static const t_module_loading_struct modules[] = {
    {.reg_fn=luaopen_sys_native, .name="sys"},
};

static inline void	register_module(lua_State *L, const char *name, lua_CFunction module_function)
{
	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
	lua_pushcfunction(L, module_function);
	lua_setfield(L, -2, name);
	lua_pop(L, 1);
}

static inline void	register_all_custom_modules(lua_State *L)
{
	for(size_t i=0; i<(sizeof(modules) / sizeof(modules[0])); i++)
	{
		register_module(L, modules[i].name, modules[i].reg_fn);
		printf("\033[35m=> Loaded lua module %s\033[0m\n", modules[i].name);
	}
}

#endif
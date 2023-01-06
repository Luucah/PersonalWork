
#include <testlib.hpp>

#include <lauxlib.h>
#include <lualib.h>

#include <iostream>
static int test_function(lua_State* L) {
    if (lua_gettop(L) == 1) {
        std::cout << "Recieved message: " << lua_tostring(L, 1) << std::endl;
        lua_pushboolean(L, false);
    } else {
        std::cout << "Called test function\n";
        lua_pushboolean(L, true);
    }
    return 1;
}

static const struct luaL_Reg testlib [] = {
    {"test", test_function},
    {NULL, NULL}
};

int open_testlib(lua_State* L) {
    // lua_newtable(L);
    // luaL_setfuncs(L, testlib, 0);
    // lua_setglobal(L, "testlib");
    luaL_newlib(L, testlib);
    return 1;
}

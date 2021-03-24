#include "log.h"

bool x11_test_global_log_info = false;
bool x11_test_global_log_error = false;
bool x11_test_global_log_debug = false;

void x11_test_open_log(x11_test_log_type_t type)
{
    switch (type)
    {
    case X11_TEST_LOG_TYPE_INFO:
        x11_test_global_log_info = true;
        break;

    case X11_TEST_LOG_TYPE_ERROR:
        x11_test_global_log_error = true;
        break;

    case X11_TEST_LOG_TYPE_DEBUG:
        x11_test_global_log_debug = true;
        break;
    }
}

void x11_test_close_log(x11_test_log_type_t type)
{
    switch (type)
    {
    case X11_TEST_LOG_TYPE_INFO:
        x11_test_global_log_info = false;
        break;

    case X11_TEST_LOG_TYPE_ERROR:
        x11_test_global_log_error = false;
        break;

    case X11_TEST_LOG_TYPE_DEBUG:
        x11_test_global_log_debug = false;
        break;
    }
}

int x11_test_lua_open_log(lua_State* L)
{
    x11_test_log_type_t type = luaL_checkinteger(L, 1);
    x11_test_open_log(type);
    return 0;
}

int x11_test_lua_close_log(lua_State* L)
{
    x11_test_log_type_t type = luaL_checkinteger(L, 1);
    x11_test_close_log(type);
    return 0;
}

int x11_test_lua_log_info(lua_State* L)
{
    if(!x11_test_global_log_info)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    luaL_argcheck(L, lua_gettop(L) == 1, 2, "too many arguments");
    const char* msg = luaL_checkstring(L, 1);
    printf("\033[1mINFO: %s\033[0m\n", msg);
    lua_pushboolean(L, true);
    return 1;
}

int x11_test_lua_log_error(lua_State* L)
{
    if(!x11_test_global_log_error)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    luaL_argcheck(L, lua_gettop(L) == 1, 2, "too many arguments");
    const char* msg = luaL_checkstring(L, 1);
    printf("\033[1;31mERROR: %s\033[0m\n", msg);
    lua_pushboolean(L, true);
    return 1;
}

int x11_test_lua_log_debug(lua_State* L)
{
    if(!x11_test_global_log_debug)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    luaL_argcheck(L, lua_gettop(L) == 1, 2, "too many arguments");
    const char* msg = luaL_checkstring(L, 1);
    printf("\033[1;34mDEBUG: %s\033[0m\n", msg);
    lua_pushboolean(L, true);
    return 1;
}
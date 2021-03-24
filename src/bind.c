#include "log.h"
#include "bind.h"

void x11_test_bind_int_to_global(lua_State* L, const x11_test_bind_int_node_t* node)
{
    lua_pushinteger(L, node->value);
    lua_setglobal(L, node->name);
}

void x11_test_bind_int_to_table(lua_State* L, const x11_test_bind_int_node_t* node)
{
    lua_pushinteger(L, node->value);
    lua_setfield(L, -2, node->name);
}

void x11_test_bind_func_to_global(lua_State* L, const x11_test_bind_func_node_t* node)
{
    lua_register(L, node->name, node->func);
}

void x11_test_bind_func_to_table(lua_State* L, const x11_test_bind_func_node_t* node)
{
    lua_pushcfunction(L, node->func);
    lua_setfield(L, -2, node->name);
}

void x11_test_bind_methods(lua_State*L, const char* metaname, const x11_test_bind_func_node_t* node_list)
{
    if(0 != luaL_newmetatable(L, metaname))
        X11_TEST_LOG_DEBUG("new metatable %s", metaname);
    lua_newtable(L); // __index
    for(const x11_test_bind_func_node_t* node = node_list; node != NULL && node->func != NULL; node++)
    {
        x11_test_bind_func_to_table(L, node);
    }
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);
}

void x11_test_bind_metamethods(lua_State*L, const char* metaname, const x11_test_bind_func_node_t* node_list)
{
    if(0 != luaL_newmetatable(L, metaname))
        X11_TEST_LOG_DEBUG("new metatable %s", metaname);
    for(const x11_test_bind_func_node_t* node = node_list; node != NULL && node->func != NULL; node++)
    {
        x11_test_bind_func_to_table(L, node);
    }
    lua_setmetatable(L, -2);
}
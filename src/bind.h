#ifndef X11TEST_BIND_H
#define X11TEST_BIND_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define X11_TESTREGISTRY_NAME "X11Test"

typedef struct x11_test_bind_int_node_t
{
    const char* name;
    int value;
}x11_test_bind_int_node_t;

typedef struct x11_test_bind_func_node_t
{
    const char* name;
    lua_CFunction func;
}x11_test_bind_func_node_t;


void x11_test_bind_int_to_global(lua_State* L, const x11_test_bind_int_node_t* node);
void x11_test_bind_int_to_table(lua_State* L, const x11_test_bind_int_node_t* node);
void x11_test_bind_func_to_global(lua_State* L, const x11_test_bind_func_node_t* node);
void x11_test_bind_func_to_table(lua_State* L, const x11_test_bind_func_node_t* node);
void x11_test_bind_methods(lua_State*L, const char* metaname, const x11_test_bind_func_node_t* node_list);
void x11_test_bind_metamethods(lua_State*L, const char* metaname, const x11_test_bind_func_node_t* node_list);

#endif
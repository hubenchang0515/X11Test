#ifndef X11TEST_WINDOW_H
#define X11TEST_WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xcb/xcb.h>
#include "bind.h"


typedef struct x11_test_window_t
{
    xcb_window_t window;
    xcb_connection_t* c;
    xcb_window_t root;
}x11_test_window_t;

#define X11_TEST_WINDOW_TYPE_NAME "x11_test_window_t"

// Lua
x11_test_window_t* x11_test_lua_new_window(lua_State* L, xcb_connection_t* c, xcb_window_t window, xcb_window_t root);
int x11_test_lua_window_to_string(lua_State* L);
int x11_test_lua_get_client_window_list(lua_State* L);
int x11_test_lua_get_window_id(lua_State* L);
int x11_test_lua_get_window_pid(lua_State* L);
int x11_test_lua_get_window_exec(lua_State* L);
int x11_test_lua_get_window_title(lua_State* L);
int x11_test_lua_get_window_class(lua_State* L);
int x11_test_lua_find_window_by_pid(lua_State* L);
int x11_test_lua_find_window_by_title(lua_State* L);
int x11_test_lua_find_window_by_exec(lua_State* L);
int x11_test_lua_find_window_by_class(lua_State* L);
int x11_test_lua_get_window_area(lua_State* L);


// C
uint32_t x11_test_query_tree(xcb_connection_t* c, xcb_window_t window, xcb_window_t* children, uint32_t length);
void x11_test_get_window_area(xcb_connection_t* c, xcb_window_t relative, xcb_window_t window, uint16_t* x, uint16_t* y, uint16_t* width, uint16_t* height);

#endif
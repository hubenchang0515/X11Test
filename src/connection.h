#ifndef X11TEST_CONNECTION_H
#define X11TEST_CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xcb/xcb.h>
#include "bind.h"

typedef struct x11_test_connection_t
{
    xcb_connection_t* c;
    xcb_window_t root;
}x11_test_connection_t;

#define X11_TEST_CONNECTION_TYPE_NAME "x11_test_connection_t"

// Lua
x11_test_connection_t* x11_test_lua_new_connection(lua_State* L, xcb_connection_t* c, xcb_window_t root);
int x11_test_lua_connect(lua_State* L);
int x11_test_lua_disconnect(lua_State* L);
int x11_test_lua_connection_to_string(lua_State* L);
int x11_test_lua_get_root_window(lua_State* L);

// C
xcb_connection_t* x11_test_connect(const char *displayname, int* screenp);
void x11_test_disconnect(xcb_connection_t* c);
xcb_window_t x11_test_get_root_window(xcb_connection_t* c);

#endif
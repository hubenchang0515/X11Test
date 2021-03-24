#ifndef X11_TEST_MOUSE_H
#define X11_TEST_MOUSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xcb/xcb.h>
#include "bind.h"

// Lua
int x11_test_lua_mouse_move(lua_State* L);
int x11_test_lua_mouse_move_to(lua_State* L);

// C
void x11_test_mouse_move(xcb_connection_t* c, xcb_window_t root, uint16_t x, uint16_t y);

#endif
#include "log.h"
#include "mouse.h"
#include "connection.h"
#include "window.h"

// Lua
int x11_test_lua_mouse_move_to(lua_State* L)
{
    x11_test_connection_t* conn = luaL_checkudata(L, 1, X11_TEST_CONNECTION_TYPE_NAME);
    uint16_t x = luaL_checkinteger(L, 2);
    uint16_t y = luaL_checkinteger(L, 3);
    x11_test_mouse_move(conn->c, conn->root, x, y);
    return 0;
}

int x11_test_lua_mouse_move(lua_State* L)
{
    x11_test_connection_t* conn = luaL_checkudata(L, 1, X11_TEST_CONNECTION_TYPE_NAME);
    uint16_t x = luaL_checkinteger(L, 2);
    uint16_t y = luaL_checkinteger(L, 3);
    x11_test_mouse_move(conn->c, XCB_NONE, x, y);
    return 0;
}

// C
void x11_test_mouse_move(xcb_connection_t* c, xcb_window_t relative, uint16_t x, uint16_t y)
{
    X11_TEST_LOG_DEBUG("mouse move to %u %u", x, y);
    xcb_warp_pointer(c, XCB_NONE, relative, 0, 0, 0, 0, x, y);
    xcb_flush(c);
}
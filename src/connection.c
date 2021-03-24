#include "log.h"
#include "connection.h"
#include "window.h"

// Lua
x11_test_connection_t* x11_test_lua_new_connection(lua_State* L, xcb_connection_t* c, xcb_window_t root)
{
    static x11_test_bind_func_node_t methods[] = 
    {
        { "disconnect", x11_test_lua_disconnect },
        { "root", x11_test_lua_get_root_window },
        { NULL, NULL },
    };

    static x11_test_bind_func_node_t metamethods[] = 
    {
        { "__gc", x11_test_lua_disconnect },
        { "__tostring", x11_test_lua_connection_to_string },
        { NULL, NULL },
    };

    if(c == NULL)
    {
        X11_TEST_LOG_ERROR(MSG_ARG_NULL(c));
        lua_pushnil(L);
        return NULL;
    }
    x11_test_connection_t* conn = lua_newuserdata(L, sizeof(x11_test_connection_t));
    if(conn == NULL)
    {
        X11_TEST_LOG_ERROR(MSG_INVOKE_FAILED(lua_newuserdata));
        lua_pushnil(L);
        return NULL;
    }

    conn->c = c;
    conn->root = root;
    x11_test_bind_methods(L, X11_TEST_CONNECTION_TYPE_NAME, methods);
    x11_test_bind_metamethods(L, X11_TEST_CONNECTION_TYPE_NAME, metamethods);
    return conn;
}

int x11_test_lua_connect(lua_State* L)
{
    const char* display = NULL;
    if(!lua_isnoneornil(L, 1))
    {
        display = luaL_checkstring(L, 1);
    }
    int* screenp = NULL;
    int screen = 0;
    if(!lua_isnoneornil(L, 2))
    {
        screen = luaL_checkinteger(L, 2);
        screenp = &screen;
    }
    xcb_connection_t* c = x11_test_connect(display, screenp);
    xcb_window_t root = x11_test_get_root_window(c);
    x11_test_lua_new_connection(L, c, root);
    return 1;
}

int x11_test_lua_connection_to_string(lua_State* L)
{
    x11_test_connection_t* conn = luaL_checkudata(L, 1, X11_TEST_CONNECTION_TYPE_NAME);
    char str[512];
    if(conn != NULL)
        sprintf(str, "%s <%p>", X11_TEST_CONNECTION_TYPE_NAME, conn->c);
    else
        sprintf(str, "%s <nil>", X11_TEST_CONNECTION_TYPE_NAME);
    lua_pushstring(L, str);
    return 1;
}

int x11_test_lua_disconnect(lua_State* L)
{
    x11_test_connection_t* conn = luaL_checkudata(L, 1, X11_TEST_CONNECTION_TYPE_NAME);
    if(conn == NULL || conn->c == NULL)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    X11_TEST_LOG_DEBUG("disconnect %p", conn->c);
    x11_test_disconnect(conn->c);
    conn->c = NULL;
    lua_pushboolean(L, true);
    return 1;
}

int x11_test_lua_get_root_window(lua_State* L)
{
    x11_test_connection_t* conn = luaL_checkudata(L, 1, X11_TEST_CONNECTION_TYPE_NAME);
    if(conn == NULL || conn->c == NULL)
    {
        lua_pushnil(L);
        return 1;
    }

    xcb_window_t root = conn->root;
    x11_test_lua_new_window(L, conn->c, root, root); // <conn> <window>
    lua_pushnil(L);                                  // <conn> <window> <nil>
    lua_copy(L, 1, 3);                               // <conn> <window> <conn>
    lua_setuservalue(L, -2);                         // <conn> <window>
    return 1;
}

// C
xcb_connection_t* x11_test_connect(const char *displayname, int* screenp)
{
    xcb_connection_t* c = xcb_connect(displayname, screenp);
    return c;
}

void x11_test_disconnect(xcb_connection_t* c)
{
    if(c == NULL)
        return;

    xcb_disconnect(c);
}

xcb_window_t x11_test_get_root_window(xcb_connection_t* c)
{
    xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(xcb_get_setup(c));
    return iterator.data->root;
}
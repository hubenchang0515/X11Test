#include "connection.h"
#include "window.h"
#include "mouse.h"
#include "log.h"

x11_test_bind_int_node_t global_enums[] = {
    { "LOG_INFO", X11_TEST_LOG_TYPE_INFO },
    { "LOG_ERROR", X11_TEST_LOG_TYPE_ERROR },
    { "LOG_DEBUG", X11_TEST_LOG_TYPE_DEBUG },
    { NULL, -1 },
};

x11_test_bind_func_node_t global_functions[] = 
{
    { "connect", x11_test_lua_connect },
    { "disconnect", x11_test_lua_disconnect },
    { "getRootWindow", x11_test_lua_get_root_window },
    { "getClientWindowList", x11_test_lua_get_client_window_list },
    { "getWindowPid", x11_test_lua_get_window_pid },
    { "getWindowExec", x11_test_lua_get_window_exec },
    { "getWindowTitle", x11_test_lua_get_window_title },
    { "getWindowClass", x11_test_lua_get_window_class },
    { "getWindowArea", x11_test_lua_get_window_area },
    { "findWindowByPid", x11_test_lua_find_window_by_pid },
    { "findWindowByTitle", x11_test_lua_find_window_by_title },
    { "findWindowByExec", x11_test_lua_find_window_by_exec },
    { "findWindowByClass", x11_test_lua_find_window_by_class },
    { "moveMouse", x11_test_lua_mouse_move, },
    { "setMouse", x11_test_lua_mouse_move_to, },
    { "openLog", x11_test_lua_open_log },
    { "closeLog", x11_test_lua_close_log },
    { "log", x11_test_lua_log_info },
    { "error", x11_test_lua_log_error },
    { "debug", x11_test_lua_log_debug },
    { NULL, NULL },
};

int luaopen_libX11Test(lua_State* L)
{
    lua_newtable(L);
    // bind enums
    for(x11_test_bind_int_node_t* node = global_enums; node->name != NULL; node++)
    {
        x11_test_bind_int_to_table(L, node);
    }

    // bind functions
    for(x11_test_bind_func_node_t* node = global_functions; node->name != NULL; node++)
    {
        x11_test_bind_func_to_table(L, node);
    }
    lua_setfield(L, LUA_REGISTRYINDEX, X11_TESTREGISTRY_NAME);
    lua_getfield(L, LUA_REGISTRYINDEX, X11_TESTREGISTRY_NAME);
    return 1;
}
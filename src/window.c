#include <unistd.h>
#include <limits.h>
#include <linux/limits.h>
#include "log.h"
#include "window.h"
#include "property.h"

// Lua

x11_test_window_t* x11_test_lua_new_window(lua_State* L, xcb_connection_t* c, xcb_window_t window, xcb_window_t root)
{
    static x11_test_bind_func_node_t methods[] = 
    {
        { "clients", x11_test_lua_get_client_window_list },
        { "id", x11_test_lua_get_window_id },
        { "pid", x11_test_lua_get_window_pid },
        { "exec", x11_test_lua_get_window_exec },
        { "title", x11_test_lua_get_window_title },
        { "className", x11_test_lua_get_window_class },
        { "area", x11_test_lua_get_window_area },
        { "findByPid", x11_test_lua_find_window_by_pid },
        { "findByTitle", x11_test_lua_find_window_by_title },
        { "findByExec", x11_test_lua_find_window_by_exec },
        { "findByClass", x11_test_lua_find_window_by_class },
        { NULL, NULL },
    };

    static x11_test_bind_func_node_t metamethods[] = 
    {
        // { "__gc", x11_test_lua_disconnect },
        { "__tostring", x11_test_lua_window_to_string },
        { NULL, NULL },
    };

    x11_test_window_t* w = lua_newuserdata(L, sizeof(x11_test_window_t));
    if(w == NULL)
    {
        X11_TEST_LOG_ERROR(MSG_INVOKE_FAILED(lua_newuserdata));
        return NULL;
    }
    w->c = c;
    w->window = window;
    w->root = root;
    x11_test_bind_methods(L, X11_TEST_WINDOW_TYPE_NAME, methods);
    x11_test_bind_metamethods(L, X11_TEST_WINDOW_TYPE_NAME, metamethods);
    return w;
}

int x11_test_lua_window_to_string(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);
    char str[512];
    if(w != NULL)
        sprintf(str, "%s <0x%x>", X11_TEST_WINDOW_TYPE_NAME, w->window);
    else
        sprintf(str, "%s <nil>", X11_TEST_WINDOW_TYPE_NAME);
    lua_pushstring(L, str);
    return 1;
}

int x11_test_lua_get_client_window_list(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);
    uint32_t size = 0;
    xcb_window_t* clients = x11_test_get_window_property(w->c, w->window, "_NET_CLIENT_LIST", "WINDOW", &size);
    lua_newtable(L); // <conn> <table>
    for(uint32_t i = 0; i < size/sizeof(xcb_window_t); i++)
    {
        x11_test_lua_new_window(L, w->c, clients[i], w->root); // <conn> <table> <window>
        lua_pushnil(L);                                        // <conn> <table> <window> <nil>
        lua_copy(L, 1, -1);                                    // <conn> <table> <window> <conn>
        lua_setuservalue(L, -2);                               // <conn> <table> <window>
        lua_rawseti(L, -2, i+1);                               // <conn> <table>
    }
    return 1;
}

int x11_test_lua_get_window_id(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);
    lua_pushinteger(L, w->window);
    return 1;
}

int x11_test_lua_get_window_pid(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);
    uint32_t size = 0;
    pid_t* pid = (pid_t*)x11_test_get_window_property(w->c, w->window, "_NET_WM_PID", "CARDINAL", &size);
    if(pid == NULL)
    {
        X11_TEST_LOG_ERROR(MSG_INVOKE_FAILED(x11_test_get_window_property));
        lua_pushnil(L);
    }
    else
    {
        lua_pushinteger(L, *pid);
    }
    
    return 1;
}

int x11_test_lua_get_window_exec(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);

    char symlink[PATH_MAX+1];
    char real[PATH_MAX+1];

    uint32_t size = 0;
    pid_t* pid = (pid_t*)x11_test_get_window_property(w->c, w->window, "_NET_WM_PID", "CARDINAL", &size);
    if(pid == NULL)
    {
        X11_TEST_LOG_ERROR(MSG_INVOKE_FAILED(x11_test_get_window_property));
        lua_pushnil(L);
        return 1;
    }

    snprintf(symlink, PATH_MAX, "/proc/%d/exe", *pid);
    free(pid);

    ssize_t len = readlink(symlink, real, PATH_MAX);
    if(len < 0)
    {
        X11_TEST_LOG_ERROR(MSG_INVOKE_FAILED(readlink));
        lua_pushnil(L);
        return 1;
    }

    lua_pushlstring(L, real, len);
    return 1;
}

int x11_test_lua_get_window_title(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);
    uint32_t size = 0;
    char* title = x11_test_get_window_property(w->c, w->window, "_NET_WM_NAME", "UTF8_STRING", &size);
    lua_pushlstring(L, title, size);
    free(title);
    return 1;
}

int x11_test_lua_get_window_class(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);
    uint32_t size = 0;
    char* window_class = x11_test_get_window_property(w->c, w->window, "WM_CLASS", "STRING", &size);

    // 实际得到的是以0分割点字符串数组
    // 这里只返回第一个字符串
    lua_pushstring(L, window_class);
    free(window_class);
    return 1;
}

int x11_test_lua_get_window_area(lua_State* L)
{
    x11_test_window_t* w = luaL_checkudata(L, 1, X11_TEST_WINDOW_TYPE_NAME);
    uint16_t x, y, width, height;
    x11_test_get_window_area(w->c, w->root, w->window, &x, &y, &width, &height);
    lua_newtable(L);
    lua_pushinteger(L, x);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, y);
    lua_setfield(L, -2, "y");
    lua_pushinteger(L, width);
    lua_setfield(L, -2, "width");
    lua_pushinteger(L, height);
    lua_setfield(L, -2, "height");
    return 1;
}

int x11_test_lua_find_window_by_pid(lua_State* L)
{
    pid_t target = luaL_checkinteger(L, 2);                     // STACK:
    x11_test_lua_get_client_window_list(L);                     // <root> <target> <client_list>
    for(int i = 1; lua_rawgeti(L, -1, i) != LUA_TNIL; i++)      // <root> <target> <client_list> <window>
    {
        lua_rotate(L, 1, -3);                                   // <window> <root> <target> <client_list> 
        x11_test_lua_get_window_pid(L);                         // <window> <root> <target> <client_list> <pid>
        pid_t pid = lua_tointeger(L, -1);
        lua_pop(L, 1);                                          // <window> <root> <target> <client_list>
        lua_rotate(L, 1, 3);                                    // <root> <target> <client_list> <window>
        if(pid == target)
        {
            return 1;
        }
        lua_pop(L, 1);                                          // <root> <target> <client_list>
    }

    lua_pushnil(L);
    return 1;
}

int x11_test_lua_find_window_by_title(lua_State* L)
{
    static const char* code = 
    "return function(root, title)\n"
    "    local ret = {}\n"
    "    local clients = root:clients()\n"
    "    for i, w in ipairs(clients) do\n"
    "        if string.find(w:title(), title) then\n"
    "            table.insert(ret, w)\n"
    "        end\n"
    "    end\n"
    "    return ret\n"
    "end\n";
                                // STACK:
    luaL_dostring(L, code);     // <root> <target> <func>
    lua_rotate(L, 1, -2);       // <func> <root> <target>
    lua_pcall(L, 2, 1, 0);
    return 1;
}

int x11_test_lua_find_window_by_exec(lua_State* L)
{
    static const char* code = 
    "return function(root, exec)\n"
    "    local ret = {}\n"
    "    local clients = root:clients()\n"
    "    for i, w in ipairs(clients) do\n"
    "        if w:exec() == exec then\n"
    "            table.insert(ret, w)\n"
    "        end\n"
    "    end\n"
    "    return ret\n"
    "end\n";
                                // STACK:
    luaL_dostring(L, code);     // <root> <target> <func>
    lua_rotate(L, 1, -2);       // <func> <root> <target>
    lua_pcall(L, 2, 1, 0);
    return 1;
}

int x11_test_lua_find_window_by_class(lua_State* L)
{
    static const char* code = 
    "return function(root, className)\n"
    "    local ret = {}\n"
    "    local clients = root:clients()\n"
    "    for i, w in ipairs(clients) do\n"
    "        if w:className() == className then\n"
    "            table.insert(ret, w)\n"
    "        end\n"
    "    end\n"
    "    return ret\n"
    "end\n";
                                // STACK:
    luaL_dostring(L, code);     // <root> <target> <func>
    lua_rotate(L, 1, -2);       // <func> <root> <target>
    lua_pcall(L, 2, 1, 0);
    return 1;
}


// C

uint32_t x11_test_query_tree(xcb_connection_t* c, xcb_window_t window, xcb_window_t* children, uint32_t length)
{
    xcb_query_tree_cookie_t cookie = xcb_query_tree(c, window);
    xcb_query_tree_reply_t* reply = xcb_query_tree_reply(c, cookie, NULL);
    xcb_window_t* raw_children = xcb_query_tree_children(reply);
    uint32_t real_length = reply->length;
    length = length > real_length ? real_length : length;
    memcpy(children, raw_children, length * sizeof(xcb_window_t));
    free(reply);
    return real_length;
}

void x11_test_get_window_area(xcb_connection_t* c, xcb_window_t relative, xcb_window_t window, uint16_t* x, uint16_t* y, uint16_t* width, uint16_t* height)
{
    xcb_get_geometry_cookie_t cookie = xcb_get_geometry(c, (xcb_drawable_t)window);
    xcb_get_geometry_reply_t* reply = xcb_get_geometry_reply(c, cookie, NULL);
    if(width != NULL)
        *width = reply->width;
    if(height != NULL)
        *height = reply->height;
    free(reply);

    if(x == NULL && y == NULL)
        return;

    xcb_translate_coordinates_cookie_t cookie2 = xcb_translate_coordinates(c, window, relative, 0, 0);
    xcb_translate_coordinates_reply_t* reply2 = xcb_translate_coordinates_reply(c, cookie2, NULL);
    if(x != NULL)
        *x = reply2->dst_x;
    if(y != NULL)
        *y = reply2->dst_y;
    free(reply2);
}
#include "log.h"
#include "property.h"

xcb_atom_t x11_test_get_atom(xcb_connection_t* c, const char* str)
{
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(c, false, strlen(str), str);
    xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(c, cookie, NULL);
    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

void* x11_test_get_window_property(xcb_connection_t* c, xcb_window_t window, const char* property_name, const char* property_type, uint32_t* size)
{
    if(size == NULL)
    {
        X11_TEST_LOG_ERROR(MSG_ARG_NULL(type));
        return NULL;
    }

    xcb_atom_t property = x11_test_get_atom(c, property_name);
    xcb_atom_t type = x11_test_get_atom(c, property_type);

#define INIT_VALUE_BYTE_SIZE 64
    void* value = NULL;
    *size = 0; // byte size
    while(true)
    {
        xcb_get_property_cookie_t cookie = xcb_get_property(c, 0, window, property, type, (*size)/4, INIT_VALUE_BYTE_SIZE/4);
        xcb_get_property_reply_t* reply = xcb_get_property_reply(c, cookie, NULL);
        uint32_t bytes = xcb_get_property_value_length(reply);
        if(reply == NULL || bytes == 0)
        {
            X11_TEST_LOG_ERROR(MSG_INVOKE_FAILED(xcb_get_property));
            break;
        }
        value = realloc(value, *size + bytes);
        memcpy(value+(*size), xcb_get_property_value(reply), bytes);
        *size += bytes;
        free(reply);
        if(bytes < INIT_VALUE_BYTE_SIZE)
            break;
    }
#undef INIT_VALUE_BYTE_SIZE
    
    return value;
}

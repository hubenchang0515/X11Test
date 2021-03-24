#ifndef X11_TEST_PROPERTY_H
#define X11_TEST_PROPERTY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xcb/xcb.h>

xcb_atom_t x11_test_get_atom(xcb_connection_t* c, const char* str);
void* x11_test_get_window_property(xcb_connection_t* c, xcb_window_t window, const char* property_name, const char* property_type, uint32_t* size);

#endif
#ifndef X11_TEST_LOG_H
#define X11_TEST_LOG_H

#include <stdio.h>
#include <stdbool.h>
#include "bind.h"

extern bool x11_test_global_log_info;
extern bool x11_test_global_log_error;
extern bool x11_test_global_log_debug;

#define X11_TEST_LOG_INFO(FMT, ...) \
            do{\
                if(!x11_test_global_log_info) \
                    break;\
                printf("\033[1mINFO[%s:%d]: " FMT "\033[0m\n", \
                        __FILE__, __LINE__, ##__VA_ARGS__);\
            }while(0)

#define X11_TEST_LOG_ERROR(FMT, ...) \
            do{\
                if(!x11_test_global_log_error) \
                    break;\
                fprintf(stderr, "\033[1;31mERROR[%s:%d]: " FMT "\033[0m\n",\
                         __FILE__, __LINE__, ##__VA_ARGS__);\
            }while(0)

#define X11_TEST_LOG_DEBUG(FMT, ...) \
            do{\
                if(!x11_test_global_log_debug) \
                    break;\
                printf("\033[1;34mDEBUG[%s:%d]: " FMT "\033[0m\n", \
                        __FILE__, __LINE__, ##__VA_ARGS__);\
            }while(0)

#define MSG_RET_NULL(X) "return value <" #X "> is null"
#define MSG_ARG_NULL(X) "argument <" #X "> is null"
#define MSG_INVOKE_FAILED(X) "invoke <" #X "> failed"

typedef enum x11_test_log_type_t
{
    X11_TEST_LOG_TYPE_INFO,
    X11_TEST_LOG_TYPE_ERROR,
    X11_TEST_LOG_TYPE_DEBUG,
}x11_test_log_type_t;

void x11_test_open_log(x11_test_log_type_t type);
void x11_test_close_log(x11_test_log_type_t type);

int x11_test_lua_open_log(lua_State* L);
int x11_test_lua_close_log(lua_State* L);
int x11_test_lua_log_info(lua_State* L);
int x11_test_lua_log_error(lua_State* L);
int x11_test_lua_log_debug(lua_State* L);

#endif
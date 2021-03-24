LUA_VERSION := 5.3

TARGET := libX11Test.so
CFLAGS := -fPIC -O3 -W -Wall -Wextra $(SYSFLAGS)
LDFLAGS := -lxcb

OBJS = libmain.o bind.o log.o connection.o window.o property.o mouse.o

.PHONY: all debug clean install

all: libX11Test.so

debug:
	$(MAKE) all SYSFLAGS="-DDEBUG"

clean:
	rm -f *.o

install: all
	install -Dm755 libX11Test.so /usr/local/lib/lua/$(LUA_VERSION)/libX11Test.so

libX11Test.so: $(OBJS)
	$(CC) -shared -o libX11Test.so $(OBJS) $(LDFLAGS)

libmain.o: libmain.c *.h
bind.o: bind.c bind.h log.h
log.o: log.c log.h bind.h
connection.o: connection.c connection.h bind.h log.h
window.o: window.c window.h property.h bind.h log.h
property.o: property.c property.h
mouse.o: mouse.c mouse.h connection.h bind.h 
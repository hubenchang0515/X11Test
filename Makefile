LUA_VERSION := 5.3

.PHONY: all test clean install

all:
	$(MAKE) all -C ./src
	cp ./src/libX11Test.so ./libX11Test.so

test:
	$(MAKE) all -C ./src
	cp ./src/libX11Test.so ./test/libX11Test.so
	$(MAKE) all -C ./test

clean:
	$(MAKE) clean -C ./src

install:
	$(MAKE) install -C ./src LUA_VERSION=$(LUA_VERSION)
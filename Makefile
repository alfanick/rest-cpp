CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2 -g
INCLUDES=-I./src/rest
LIBRARY=

.PHONY: clean

CPP_FILES := $(wildcard src/rest/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

example: librest
	$(MAKE) -C example/todo_server

librest: lib/librest.dylib

lib/librest.dylib: $(OBJ_FILES)
	$(CXX) -dynamiclib -Wl,-install_name,librest.dylib -o lib/librest.dylib $^

obj/%.o: src/rest/%.cpp
	   $(CXX) $(INCLUDES) -c -o $@ $<


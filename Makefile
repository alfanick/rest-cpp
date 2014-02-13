CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2 -g
INCLUDES=
LIBRARY=

.PHONY: clean

CPP_FILES := $(wildcard src/rest/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

example: librest
	$(MAKE) -C example/todo_server

ifeq ($(shell uname),Darwin)
librest: lib/librest.dylib
else
CXX=g++ -std=gnu++11 -Wall -pthread -O2
INCLUDES=-fPIC
librest: lib/librest.so
endif

lib/librest.dylib: $(OBJ_FILES)
	$(CXX) -dynamiclib -Wl,-install_name,librest.dylib -o lib/librest.dylib $^

lib/librest.so: $(OBJ_FILES)
	$(CXX) -fPIC -shared -o lib/librest.so $^ 

obj/%.o: src/rest/%.cpp
	   $(CXX) $(INCLUDES) -c -o $@ $<


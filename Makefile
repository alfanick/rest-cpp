CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2 -g
INCLUDES=
LIBRARY=

.PHONY: clean example librestcpp install docs

CPP_FILES := $(shell find src -type f -name '*.cpp')
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

default: librestcpp

example: librestcpp
	$(MAKE) -C example/todo_server build

ifeq ($(shell uname),Darwin)
librestcpp: lib/librestcpp.dylib
else
CXX=g++ -std=gnu++11 -Wall -pthread -O2
INCLUDES=-fPIC
librestcpp: lib/librestcpp.so
endif

lib/librestcpp.dylib: $(OBJ_FILES)
	$(CXX) -dynamiclib -Wl,-install_name,librestcpp.dylib -o lib/librestcpp.dylib $^

lib/librestcpp.so: $(OBJ_FILES)
	$(CXX) -fPIC -shared -o lib/librestcpp.so $^ 

obj/%.o: src/rest/%.cpp
	$(CXX) $(INCLUDES) -c -o $@ $<

obj/%.o: src/rest/dispatchers/%.cpp
	$(CXX) $(INCLUDES) -c -o $@ $<

docs:
	@doxygen docs/doxygen.conf

clean:
	@rm -rf docs/html
	@rm -f obj/*.o

install: librestcpp
	@echo "Installing headers"
	@rsync -r --exclude="*.cpp" src/ /usr/local/include/
	@echo "Installing library"
	@cp -f lib/librestcpp.* /usr/local/lib
	@echo "Installing generator"
	@cp -f bin/rest-cpp /usr/local/bin

CXX=/usr/bin/clang++ -Wall -Wextra -Wno-unused-parameter -std=c++11 -stdlib=libc++ -O2 -march=native -Ofast
INCLUDES=
LIBRARY=

.PHONY: clean example librestcpp install docs infolib

CPP_FILES := $(shell find src -type f -name '*.cpp')
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

default: librestcpp

example: librestcpp
	$(MAKE) -C example/todo_server build

infolib:
	@echo "Building rest-cpp"

ifeq ($(shell uname),Darwin)
librestcpp: infolib | lib/librestcpp.dylib
else
CXX=/usr/bin/clang++ -std=gnu++11 -Wall -pthread -O2 -march=native
INCLUDES=-fPIC
librestcpp: infolib | lib/librestcpp.so
endif

lib/librestcpp.dylib: $(OBJ_FILES)
	@echo "  building lib/librestcpp.dylib"
	@$(CXX) -dynamiclib -Wl,-install_name,librestcpp.dylib -o lib/librestcpp.dylib $^

lib/librestcpp.so: $(OBJ_FILES)
	@echo "  building lib/librestcpp.so"
	@$(CXX) -fPIC -shared -o lib/librestcpp.so $^ 

obj/%.o: src/rest/%.cpp
	@echo "  compiling $<"
	@$(CXX) $(INCLUDES) -c -o $@ $<

obj/%.o: src/rest/dispatchers/%.cpp
	@echo "  compiling $<"
	@$(CXX) $(INCLUDES) -c -o $@ $<

obj/%.o: src/rest/features/%.cpp
	@echo "  compiling $<"
	@$(CXX) $(INCLUDES) -c -o $@ $<

docs:
	@doxygen docs/doxygen.conf

clean:
	@echo "Cleaning build files"
	@rm -rf docs/html
	@rm -f obj/*.o

install: librestcpp
	@echo "Installing rest-cpp"
	@echo "  copying headers"
	@rsync -r --exclude="*.cpp" src/ /usr/local/include/
	@echo "  copying library"
	@cp -f lib/librestcpp.* /usr/local/lib
	@echo "  installing rest-cpp command"
	@cp -f bin/rest-cpp /usr/local/bin

address?=0.0.0.0
port?=8080
workers?=4
dispatcher?=lc
path?=NONE
size?=0
name?=todo_server

CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2 -march=native
INCLUDES=
LIBRARY=-DSERVER_BIND=$(address) -DSERVER_PORT=$(port) -DSERVER_WORKERS=$(workers) -DSERVER_DISPATCHER_$(dispatcher)

ifneq ($(path),NONE)
LIBRARY+= -DSERVER_PATH=$(path)
pe=$(shell seq 0 $$(( $(size) - 1 )) )
else
pe=$(shell seq $(port) $$(( $(port) + $(size) - 1 )) )
endif

ifneq ($(shell uname),Darwin)
CXX=g++-5 -std=gnu++11 -Wall -pthread -O2 -march=native -I../../src
endif

.PHONY: server build pool
default: server

server: build
	@./todo_server


pool:
ifeq ($(size),0)
	@echo "You need to provide size of the pool (size=n)."
else
	@echo "Building server pool: "
ifeq ($(path),NONE)
	@$(foreach nport,$(pe),echo "  building $(name).$(nport)"; $(MAKE) build --no-print-directory name="$(name).$(nport)" address=$(address) port=$(nport) workers=$(workers) dispatcher=$(dispatcher);)
else
	@$(foreach nport,$(pe),echo "  building $(name).$(nport)"; $(MAKE) build --no-print-directory name="$(name).$(nport)" workers=$(workers) dispatcher=$(dispatcher) path="$(path).$(nport)";)
endif

endif

build:
	@$(CXX) $(INCLUDES) *.cpp -o $(name) $(LIBRARY) ../../lib/librestcpp.a


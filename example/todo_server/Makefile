address?=0.0.0.0
port?=8080
workers?=4
dispatcher?=lc

CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2
INCLUDES=
LIBRARY=-lrestcpp -DSERVER_BIND=$(address) -DSERVER_PORT=$(port) -DSERVER_WORKERS=$(workers) -DSERVER_DISPATCHER_$(dispatcher)

ifneq ($(shell uname),Darwin)
CXX=g++ -std=gnu++11 -Wall -pthread -O2
endif

.PHONY: server build
default: server

server: build
	@DYLD_LIBRARY_PATH=/usr/local/lib LD_LIBRARY_PATH=/usr/local/lib ./todo_server

build:
	@$(CXX) $(INCLUDES) *.cpp -o todo_server $(LIBRARY)


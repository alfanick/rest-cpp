address?=0.0.0.0
port?=8080
workers?=4
dispatcher?=lc

CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2
INCLUDES=-I../../src
LIBRARY=-L. -lrestcpp -DSERVER_BIND=$(address) -DSERVER_PORT=$(port) -DSERVER_WORKERS=$(workers) -DSERVER_DISPATCHER_$(dispatcher)

ifneq ($(shell uname),Darwin)
CXX=g++ -std=gnu++11 -Wall -pthread -O2
endif

.PHONY: server run library
default: server

library:
	@echo Rebuilding REST library
	@rm ../../obj/*.o
	@$(MAKE) -s -C ../..

run: server
	@DYLD_LIBRARY_PATH=. LD_LIBRARY_PATH=. ./todo_server

server:
	@$(CXX) $(INCLUDES) *.cpp -o todo_server $(LIBRARY)

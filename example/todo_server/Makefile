address?=0.0.0.0
port?=8080
workers?=4

CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2
INCLUDES=-I../../src
LIBRARY=-L../../lib -lrest -DSERVER_BIND=$(address) -DSERVER_PORT=$(port) -DSERVER_WORKERS=$(workers)

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
	@echo Listening on $(address):$(port), $(workers) workers
	@./todo_server

server:
	@$(CXX) $(INCLUDES) *.cpp -o todo_server $(LIBRARY)

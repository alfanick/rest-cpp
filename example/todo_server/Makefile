CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2 -g
INCLUDES=-I../../src
LIBRARY=-L../../lib -lrest

.PHONY: server

server:
	$(CXX) $(INCLUDES) server.cpp -o todo_server $(LIBRARY)
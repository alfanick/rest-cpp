CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2
INCLUDES=-I../../src
LIBRARY=-L../../lib -lrest

.PHONY: server

server:
	$(CXX) $(INCLUDES) *.cpp -o todo_server $(LIBRARY)

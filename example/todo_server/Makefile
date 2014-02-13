CXX=/usr/bin/clang++ -Wall -std=c++11 -stdlib=libc++ -O2
INCLUDES=-I../../src
LIBRARY=-L../../lib -lrest

ifneq ($(shell uname),Darwin)
CXX=g++ -std=gnu++11 -Wall -pthread -O2
endif

.PHONY: server

server:
	$(CXX) $(INCLUDES) *.cpp -o todo_server $(LIBRARY)

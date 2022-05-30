CC = gcc
CXX = g++
DBG = -Wall -g -w

all:test pipe_line_server pollServer pollClient

test: test.c clib.so
	$(CC) $(DBG) test.c -o test ./clib.so

pollServer: pollserver.cpp cpplib.so
	$(CXX) $(DBG) pollserver.cpp ./cpplib.so -o pollServer

pollClient: pollclient.cpp cpplib.so
	$(CXX) $(DBG) pollclient.cpp ./cpplib.so -o pollClient

pipe_line_server: active_object_pipeline.c clib.so
	$(CC) $(DBG) active_object_pipeline.c ./clib.so -lpthread -o pipe_line_server

# shared library for all the c++ code files
cpplib.so: reactor.cpp reactor.hpp
	$(CXX) $(DBG) --shared -fPIC reactor.cpp -o cpplib.so -lpthread

# shared library for all the c code files
clib.so: queue.c queue.h active_object.c active_object.h
	$(CC) $(DBG) --shared -fPIC queue.c active_object.c -o clib.so -lpthread

clean:
	rm -f *.o *.so test pipe_line_server pollServer pollClient
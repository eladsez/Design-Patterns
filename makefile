CC = gcc
CXX = g++
FLAGS = -Wall -g -w

all:test pipe_line_server pollServer pollClient singleton_test guard_test

test: test.c clib.so
	$(CC) $(FLAGS) test.c -o test ./clib.so

singleton_test: singleton_test.cpp cpplib.so
	$(CXX) $(FLAGS) singleton_test.cpp -o singleton_test ./cpplib.so

guard_test: guard_test.cpp cpplib.so
	$(CXX) $(FLAGS) guard_test.cpp -o guard_test ./cpplib.so

pollServer: pollserver.cpp cpplib.so
	$(CXX) $(FLAGS) pollserver.cpp ./cpplib.so -o pollServer

pollClient: pollclient.cpp cpplib.so
	$(CXX) $(FLAGS) pollclient.cpp ./cpplib.so -o pollClient

pipe_line_server: active_object_pipeline.c clib.so
	$(CC) $(FLAGS) active_object_pipeline.c ./clib.so -lpthread -o pipe_line_server

# shared library for all the c++ code files
cpplib.so: reactor.cpp reactor.hpp singleton.hpp guard.cpp guard.hpp
	$(CXX) $(FLAGS) --shared -fPIC reactor.cpp singleton.hpp guard.cpp -o cpplib.so -lpthread

# shared library for all the c code files
clib.so: queue.c queue.h active_object.c active_object.h
	$(CC) $(FLAGS) --shared -fPIC queue.c active_object.c -o clib.so -lpthread

clean:
	rm -f *.o *.so test pipe_line_server pollServer pollClient singleton_test guard_test
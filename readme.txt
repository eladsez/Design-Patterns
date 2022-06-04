
                        *-FILES INFORMATION-*
The queue implementation is in queue.c file. (Question 1)
The queue is thread safe using mutex and condition variables as required.

The active object implementation is in active_object.c file. (Question 2)

The pipeline server implementation which use active objects is in active_object_pipeline.c file. (Question 3)
We implement the pipeline server using the server from assigment 4

We tested the above 3 questions in main1.c file, main1 file is well documented for every test.

The guard implementation is in guard.cpp and guard.hpp files the test for it is in guard_test.cpp. (Question 4)

The Singleton implementation is in singleton.hpp files the test for it is in singleton.cpp. (Question 5)

The reactor implementation is in reactor.cpp, reactor.hpp files. (Question 6)
We also implement a chat server and a client using are reactor and beej 7.3 server template as requested
the server is in pollserver.cpp file and the client is in pollclient.cpp file

                          *-HOW TO RUN-*
First open a terminal in the design_patterns directory and type <make all>.
Now after everything compiled you can run the following targets:
a) To run the test for the first 3 Questions type <./main1>
b) To run the pipeline server from Question 3 type <./pipe_line_server>
   you can connect to this server to test it using telnet open a new terminal and run
   <telnet 127.0.0.1 3490> to connect.
c) To run the guard test type <./guard_test>
d) To run the singleton test type <./singleton_test>
e) To run the pollserver type<./pollServer>, after you run the server you can run several clients
   and send messages between them, to run a client open a new terminal and run <./pollClient 127.0.0.1>


                              *-EXTRA-*
All our source code are compiled to a shared library and our tests use this shared library you can see it in the makefile.
The pollserver was created using beej server template in https://beej.us/guide/bgnet/html/#poll
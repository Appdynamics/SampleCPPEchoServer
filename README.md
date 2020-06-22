# SampleCPPEchoServer

This is a sample application to help with C++ applications instrumentation using the SDK.

This application is a tcp echo server: it will echo back a message received through a TCP connection and also write a file with that same message.

The code is instrumented creating a sample Business transactions everytime a new message arrives and is also instrumented to create a new backend each time a message is writtem to the file.

# How to compile de code

Download the SDK and unconpress it on the root folder of the repo.
Adjust the echoserv.c file to point to the right direcionts of the appdynamics.h file
Compile de code with GCC: gcc -L/home/ec2-user/C++_Echo/appd_sdk/appdynamics-cpp-sdk/lib -lappdynamics -lpthread -ldl -lrt -o tcp_echo echoserv.c helper.c

This will generate the file tcp_echo on the local directory

You than need to configure the LD_LIBRARY_PATH to point to the right place for the AppDynamis library:

LD_LIBRARY_PATH=<full path to the repository root>/appd_sdk/appdynamics-cpp-sdk/lib/
export LD_LIBRARY_PATH
./tcp_echo

This will bring the server up. From another terminal run the netcat command:

echo "This is a message" | nc <ip for the server> 2002 

This will send a message to the server. Do this several times, or use a script for that. After sometime you will see the flow map being created on appdynamics.

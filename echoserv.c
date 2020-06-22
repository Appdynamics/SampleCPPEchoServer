/*

  ECHOSERV.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo server.

*/


#include <string.h>
#include <unistd.h>
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
//#include <readline/readline.h>
#include "/home/ec2-user/C++_Echo/appd_sdk/appdynamics-cpp-sdk/include/appdynamics.h"

#include "helper.h"           /*  our own helper functions  */

#include <stdlib.h>
#include <stdio.h>


/*  Global constants  */

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */
    const char APP_NAME[] = "SampleC";
    const char TIER_NAME[] = "SampleCTier1";
    const char NODE_NAME[] = "SampleCNode1";
    const char CONTROLLER_HOST[] = "ec2-3-17-158-220.us-east-2.compute.amazonaws.com";
    const int CONTROLLER_PORT = 8090;
    const char CONTROLLER_ACCOUNT[] = "customer1";
    const char CONTROLLER_ACCESS_KEY[] = "aa565a62-9793-43b5-9521-14f7f10bb0bc";
    const int CONTROLLER_USE_SSL = 0;
    int result;

   FILE * fp;
   int i;
   // declare a backend, only once for this SDK instance
   const char backendOne[] = "first backend";
   appd_backend_declare(APPD_BACKEND_CACHE,  backendOne);
   result = appd_backend_set_identifying_property(backendOne, "SERVER POOL", "somce-cache-example");
   result = appd_backend_set_identifying_property(backendOne, "VENDOR", "Redis");

   result = appd_backend_prevent_agent_resolution(backendOne);
   result = appd_backend_add(backendOne);

struct appd_config* cfg = appd_config_init(); // appd_config_init() resets the configuration object and pass back an handle/pointer
appd_config_set_app_name(cfg, APP_NAME);
appd_config_set_tier_name(cfg, TIER_NAME);
appd_config_set_node_name(cfg, NODE_NAME);
appd_config_set_controller_host(cfg, CONTROLLER_HOST);
appd_config_set_controller_port(cfg, CONTROLLER_PORT);
appd_config_set_controller_account(cfg, CONTROLLER_ACCOUNT);
appd_config_set_controller_access_key(cfg, CONTROLLER_ACCESS_KEY);
appd_config_set_controller_use_ssl(cfg, CONTROLLER_USE_SSL); 

int initRC = appd_sdk_init(cfg);


    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
	port = strtol(argv[1], &endptr, 0);
	if ( *endptr ) {
	    fprintf(stderr, "ECHOSERV: Invalid port number.\n");
	    exit(EXIT_FAILURE);
	}
    }
    else if ( argc < 2 ) {
	port = ECHO_PORT;
    }
    else {
	fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
	exit(EXIT_FAILURE);
    }
    printf("Simple TCP server started...\n");
	
    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling bind()\n");
	exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling listen()\n");
	exit(EXIT_FAILURE);
    }

    
    /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {

	/*  Wait for a connection, then accept() it  */

        appd_bt_handle btHandle = appd_bt_begin("Checkout", NULL);

//        appd_bt_store(btHandle, my_bt_guid);

	if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    exit(EXIT_FAILURE);
	}


	/*  Retrieve an input line from the connected socket
	    then simply write it back to the same socket.     */
	
	//readline(conn_s, buffer, MAX_LINE-1);
	//writeline(conn_s, buffer, strlen(buffer));

	Readline(conn_s, buffer, MAX_LINE-1);
	printf("Message read from connection: %s\n", buffer);
	Writeline(conn_s, buffer, strlen(buffer));
appd_exitcall_handle ecHandle = appd_exitcall_begin(btHandle, backendOne);
result = appd_exitcall_set_details(ecHandle, "backend ONE");
   /* open the file for writing*/
   fp = fopen ("echo_write.txt","w");
 
       fprintf (fp, "This is line %d\n",i + 1);
 
   /* close the file*/  
   fclose (fp);
   appd_exitcall_end(ecHandle);


	/*  Close the connected socket  */
	int close_status = close(conn_s);
//	appd_bt_handle myBtHandle = appd_bt_get(my_bt_guid);

	appd_bt_end(btHandle);
 
	if ( close_status <0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    exit(EXIT_FAILURE);
	}
	else {
	    fprintf(stderr, "ECHOSERV: Connection successfully closed.\n");
	}
    }
}

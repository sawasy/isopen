/*
 *
 * isopen - A small program which connects to a port and reads the banner.
 *
 * Copyright (c) 2018, Matthew Sawasy <sawasy@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <ctype.h>
#include <errno.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

// Loop for checking if string is a number
bool isNumber(char number[])
{
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
        i = 1;
    for (; number[i] != 0; i++)
    {
        if (!isdigit(number[i]))
            return false;
    }
    return true;
}

// Display the help message.
// Returns 1 which is returned exit code.
int usage(char program[]) {

    printf("Usage: %s [OPTIONS] [<host>] [<port #>]\n", program);
    puts("Display remote network port banners.\n");
    puts("\thost\t\tSpecifies a host to contact over the network.");
    puts("\t\t\tIf not specified, localhost is used.\n");
    puts("\tport\t\tSpecifies a port number or service name to contact.");
    puts("\t\t\tIf not specified, the SSH port (22) is used.\n");
    puts("\t--help\t\tprint this help, then exit\n");
    puts("\t--version\tprint version number, then exit\n");
    return 1;

}

int main(int argc , char **argv)
{

    #define PROGRAM "isopen"
    #define LENGTH 20
    #define VERSION "0.9"

    int err, sock, port, length;
    struct hostent *host;
    struct sockaddr_in sa;
    char *endptr;
    char *raw_port;
    char recvdata[1000];
    char hostname[100] = "127.0.0.1";
    char ip_buffer[20];

    if ( argc != 3 && argc != 2 || (strcmp(argv[1], "--help") == 0) ) {
        return usage(PROGRAM);
    }
    if ( strcmp(argv[1], "--version") == 0 ) {
        printf("%s\n", VERSION);
        return 0;
    }
    if ( argc == 2 ) {
        raw_port = argv[1];
    }
    if ( argc == 3) {
        strcpy(hostname, argv[1]);
        raw_port = argv[2];
    }		
    if (isNumber ( raw_port )) {
        port = strtoimax(raw_port,&endptr,10);
    }			
    else {
        puts("Bad port number...\n\n");
        return usage(PROGRAM);
    }

    //Create a socket of type internet
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //Initialise the sockaddr_in structure
    strncpy((char*)&sa , "" , sizeof sa);

    sa.sin_family = AF_INET;

    // set port number for socket
    sa.sin_port = htons(port);

    // check if IP address
    int result = inet_pton(AF_INET, hostname, &(sa.sin_addr));

    if ( result == 1 ) {
        sa.sin_addr.s_addr = inet_addr(hostname);
    }

    // Doing hostname look up
    else { 
        host = gethostbyname(hostname);
        if( host != 0 ) {
            strncpy((char*)&sa.sin_addr , (char*)host->h_addr , sizeof sa.sin_addr);
        }

        else {
            herror(hostname);
            usage(PROGRAM);
            exit(2);
        }
    }

    inet_ntop(AF_INET, &(sa.sin_addr), ip_buffer, LENGTH);
    printf("Trying %s:%d... \n", ip_buffer, port);

    //Check whether socket created fine or not
    if(sock < 0) {
        perror("\nSocket");
        exit(1);
    }
    //Connect using that socket and sockaddr structure
    err = connect(sock , (struct sockaddr * )&sa , sizeof sa);

    //not connected
    if ( err <  0 ) {
        //printf("%s %-5d %s\n" , hostname , port, strerror(err));
        printf("Unable to connect to remote host %s:%d : Connection refused\n", ip_buffer, port);
    }
    //connected
    else {

        bzero( recvdata, 1000);
        // Send some HTTP info, so they respond and don't time out.
        write(sock,"GET /",strlen("GET /")+1);
        read(sock, recvdata, 1000);

        if ( (length = strlen(recvdata)) == 0) {
            printf ("Port %d is listening, but no data recieved.\n", port);
        }
        else {
            printf("Connected to %s:%d \n%s\n",hostname,port, recvdata);
        }
    }

    return(0);
}

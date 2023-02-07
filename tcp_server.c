/* A simple server in the internet domain using TCP
 * Answer the questions below in your writeup
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /* 1. What is argc and *argv[]?
     * argc is the number of arguments on the command line (including the program name itself) argv is an array of C-style strings,
     and we can note that the asterick, *, is a pointer. This means *argv[] is calling for values stored in memory 
     - source: https://www.cs.odu.edu/~zeil/cs330/f13/Public/faq/faq-htmlsu7.html
     */
    int sockfd, newsockfd, portno;
    /* 2. What is a UNIX file descriptor and file descriptor table?
     * A UNIX file descriptor is a way to allow for an open input/output stream: ...
     ... a file, a network connection, a pipe (a communication channel between processes)... 
     ... A file descriptor table stroes an array.
     - source: https://cs61.seas.harvard.edu/site/ref/file-descriptors/#gsc.tab=0
     */
    socklen_t clilen;

    struct sockaddr_in serv_addr, cli_addr;
    /* 3. What is a struct? What's the structure of sockaddr_in?
     * A struct (structure) is used to group several related variables into one place: sockaddr_in
     ... places serv_addr, cli_addr in one place
     source:  https://www.w3schools.com/cpp/cpp_structs.asp
     */
    
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /* 4. What are the input parameters and return value of socket()
     * they are: (AF_INET, SOCK_STREAM, 0);
     */
    
    if (sockfd < 0) 
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
             error("ERROR on binding");
    /* 5. What are the input parameters of bind() and listen()?
     * FOr bind, they are: (sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr))
     * For listen, they are: sockfd,5
     */
    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    while(1) {
        /* 6.  Why use while(1)? Based on the code below, what problems might occur if there are multiple simultaneous connections to handle?
        *While (1) means the loop will always be true. Is there are multiple simultaneous connections to handle,
         * then there could possibly be lag in communication between the server and client.
         * source:   https://www.tutorialspoint.com/difference-between-while-1-and-while-0-in-c-cplusplus
        */
        
	char buffer[256];
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    &clilen);
	/* 7. Research how the command fork() works. How can it be applied here to better handle multiple connections?
         * We use fork() to create a somewhat "child" copy of the original code; it works by calling the process
         behind the scene. It can be applied here to better handle multiple connections so somehow the server can 
         connect to multiple clients, without having too much traffic in just one server. we can make copies of it,
         though its all from the same parent.
         soruce: https://www.geeksforgeeks.org/fork-system-call/ 
         */
        
	if (newsockfd < 0) 
             error("ERROR on accept");
	bzero(buffer,256);
        
	n = read(newsockfd,buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Here is the message: hello %s\n",buffer);
        n = write(newsockfd,"I got your message",18);
        if (n < 0) 
            error("ERROR writing to socket");
        close(newsockfd);
    }
    close(sockfd);
    return 0; 
}
  
/* This program makes several system calls such as 'bind', and 'listen.' What exactly is a system call?
 * I think a system call utilzies functions that are in the librares which are defined above. these functions are 
  * already defined in the libraries
 */

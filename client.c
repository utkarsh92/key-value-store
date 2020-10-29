#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <signal.h>

int xscok;
char buffer[256];
void error(char *msg)
{
    perror(msg);
    exit(0);
}

void sighandler1(int x)
{
    write(STDOUT_FILENO,"\nthe program is interrupted, do you want to exit [Y/N] ",54);
    char ch = getchar();
    while (1)
    {
        if(ch=='y'|| ch=='y')
        {
            bzero(buffer,256);
            //close(xscok); 
            printf("socketclosed\n");
            //exit(3);
        }
        else
        {
            fflush(stdout);
            fflush(stdin);
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    
    signal(SIGINT,sighandler1);
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    xscok=sockfd;
    if (sockfd < 0) 
        error("ERROR opening socket");
    printf("%s",argv[1]);
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    while (1)
    {
    
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    }
    return 0;
}
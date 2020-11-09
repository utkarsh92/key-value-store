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
#include <bits/stdc++.h>
#include <sys/epoll.h>

//using namespace std;
int xscok;
char buffer[513];
char padd = '#';
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
            close(xscok); 
            printf("socketclosed\n");
            exit(3);
        }
        else
        {
            fflush(stdout);
            fflush(stdin);
            return;
        }
    }
}

char * encode(char status, char key[256], char value[256])
{
	char s[1], k[256], v[256];
	char *code = (char *)malloc(513);
	code[0] = '\0';

	memset(s, padd, 1);
	memset(k, padd, 256);
	memset(v, padd, 256);

	strncpy(s, &status, 1);
	strncpy(k, key, strlen(key));
	strncpy(v, value, strlen(value));

	strncat(code, s, 1);
	strncat(code, k, 256);
	strncat(code, v, 256);

	return code;
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    int res;
    char resp;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char pad[256];
    memset(pad,'#',256);
    
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
    bzero(buffer,513);
    bzero(pad,256);
    printf("To Get Value :Press 1\nTo Put KV pair: press 2\nTo Delete KV pair: press 3\n Response :");
    
    std::cin.clear();
    std::cin.sync();
    resp=getc(stdin);
    std::cin.clear();
    std::cin.sync();

    if(resp=='1')
    {
        int i=0;
        char k[256];
        printf("Enter Key: \n");
        char*t=NULL;
        size_t tl=0;
        fflush(stdin);
        i=read(0,k,256);
        getc(stdin);
        fflush(stdin);
        k[i-1]='\0';
        //k[i]='#';
        printf("%s \n %s",buffer,encode(resp,k,pad));
        strcpy(buffer,encode(resp,k,pad));
    }
    else if(resp=='2')
    {
        int i;
        printf("Enter Key : \n");
        char k[256];
        char*t=NULL;
        size_t tl=0;
        fflush(stdin);
        i=read(0,k,256);
        getc(stdin);
        k[i-1]='\0';
        printf("Enter Value : \n");
        char v[256];
        fflush(stdin);
        i=read(0,v,256);
        //getc(stdin);
        v[i-1]='\0';
        strcpy(buffer,encode(resp,k,v));   
    }
    else if(resp=='3')
    {
        int i;
        char k[256];
        printf("Enter Key: \n");
        char*t=NULL;
        fflush(stdin);
        i=read(0,k,256);
        getc(stdin);
        k[i-1]='\0';
        strcpy(buffer,encode(resp,k,pad));
    }
    else
    {
        resp='0';
        printf("Enter correct Response \n");
        continue;
    }
    fflush(stdin);
    //printf("%ld",strlen(buffer));

    //printf("Please enter the message: "); 
    
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,513);
    n = read(sockfd,buffer,513);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    }
    return 0;
}

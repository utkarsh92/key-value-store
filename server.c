#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


void error(char *msg)
{
    perror(msg);
    exit(1);
}

char * getvalue(char * key)
{
    char * value= "Updating Keyvalue Library Please will come back later"; /// call kvlibrary(key)
    //printf("here\n");
    value=strcat(value,"\0");
    //printf("value : %s\n",value);
    return value;
}

void* worker (void * arg)
{
    int n;
    char buffer[256], message[256];
    int newsockfd = (int) arg;
    while(1)
    {     
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) {
            break;
            error("ERROR reading from socket");
        }
        printf("Here is the message: %s\n",buffer);
        strcat(strcpy(message,"I got your message \n"),getvalue(buffer));
        n = write(newsockfd,message,strlen(message));
        if (n < 0) error("ERROR writing to socket");
    }
   //close(newsockfd);
    return NULL;
}

int server_init(int portno)
{
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    return sockfd;

}

int main(int argc,char * argv[])
{

    int sockfd, newsockfd, clilen;
    char buffer[256];
    struct sockaddr_in  cli_addr;
    int portno = atoi(argv[1]);

    ///
    long thread=0, thread_count=5;
	pthread_t * thread_handles;
    int err;
    pthread_mutex_t mutex;
    thread_handles=(pthread_t *) malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex,NULL);
    ///
   
    sockfd= server_init(portno);

    while(1)
    {
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        printf("new client request came : socketfd :%d\n",newsockfd);
        if (newsockfd < 0) 
            error("ERROR on accept");
        err=pthread_create(&thread_handles[thread],NULL,worker,(void *)newsockfd);
        thread++;
        if(thread==thread_count)
        {
            thread_count*=2; // replace by increment by no of threads given in config
        }
    }



  /*  
	thread_count=2;//strtol(argv[1],NULL,10);
	thread_handles=(pthread_t *) malloc(thread_count*sizeof(pthread_t));

	for(thread=0; thread<thread_count;thread++)
	{   
		err=pthread_create(&thread_handles[thread],NULL,worker,(void*)port);
		if(err)
		{
			error("Error: Thread not created");
		}
        port++;
	}
*/

    printf("reached here");
	for(thread=0; thread<thread_count;thread++)
	{
		err=pthread_join(thread_handles[thread],NULL);
		if(err)
		{
			perror("Error :");
		}
	}

    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


struct config
{
    int LISTENING_PORT_NO;
    char CACHE_REPLACEMENT[3];
    int CACHE_ENTRIES;                //no of keys
    int CLIENTS_PER_THREAD;          //number of clients that each worker thread handles)
    int THREAD_POOL_SIZE_INITIAL;      //no of worker threads to start out with;
    int THREAD_POOL_GROWTH;            //new threads to add each time you want to grow the pool

}typedef config;

config conf;

void read_config()
{
    FILE *file;
    
    int n=0,i=0;
    char ch;
    if((file=fopen("config","r"))!=NULL)
    {   
        
        while(n<6 || ch==EOF)
        {   
            char no[10];
            bzero(no,10);
            i=0;
            while(fgetc(file)!='=');
            while((ch=fgetc(file))==' ');
            while(ch!=' ')
            {
                no[i]=ch;
                i++;
                ch=fgetc(file);
            }
            no[i]='\0';
            //printf("%s\n",no);
            //n=atoi(no);
            //printf("%d\n",n);
            switch(n)
            {
                case 0:
                    conf.LISTENING_PORT_NO=atoi(no);
                    break;
                case 1:
                    strcpy(conf.CACHE_REPLACEMENT,no);
                    break;
                case 2:
                    conf.CACHE_ENTRIES=atoi(no);
                    break;
                case 3:
                    conf.CLIENTS_PER_THREAD=atoi(no);
                    break;
                case 4:
                    conf.THREAD_POOL_SIZE_INITIAL=atoi(no);
                    break;
                case 5:
                    conf.THREAD_POOL_GROWTH=atoi(no);
                    break;

            }
            n++;
        }
    }
    return ;
}


void error(char *msg)
{
    perror(msg);
    exit(1);
}

char * getvalue(char * key)
{
    char * value= "Updating Keyvalue Library Please come back later"; /// call kvlibrary(key)
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

int server_init()
{
    int portno=conf.LISTENING_PORT_NO;
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
    int portno ;//= atoi(argv[1]);

    ///
    long thread=0, thread_count;
	pthread_t * thread_handles;
    int err;
    pthread_mutex_t mutex;
    thread_handles=(pthread_t *) malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex,NULL);
    ///
   
    read_config();
    printf("%s",conf.CACHE_REPLACEMENT);
    printf("%d %d %d %d %d",conf.LISTENING_PORT_NO,conf.CACHE_ENTRIES,conf.CLIENTS_PER_THREAD,conf.THREAD_POOL_SIZE_INITIAL,conf.THREAD_POOL_GROWTH);
    portno=conf.LISTENING_PORT_NO;
    thread_count=conf.THREAD_POOL_SIZE_INITIAL;
    sockfd= server_init();

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
            thread_count+=conf.THREAD_POOL_GROWTH; // replace by increment by no of threads given in config
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



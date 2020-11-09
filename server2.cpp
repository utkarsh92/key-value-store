#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <bits/stdc++.h>

//#define MAXFDPEREPOLL 100
#define REQLIMIT  25  //5*5
using namespace std;

int sockx;

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
list<int> fdlist ;
pthread_mutex_t mutx;

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
    char * value1= "Updating Keyvalue Library Please come back later"; /// call kvlibrary(key)
    //printf("here\n");
    char * value=strcat(value1,"\0");
    //printf("value : %s\n",value);
    return value;
}



void * worker1( void * x)
{
    
    int tno=*(  reinterpret_cast <int *>(x));
    char* tn;
    int sockfd,curno=0,newfd;
    int fdbuffer[10];
    int i=0,err;
    char buffer[513], message[513];
    struct epoll_event events[conf.CLIENTS_PER_THREAD];
    int epfd=epoll_create1(0);
    static struct epoll_event ev;
    printf("In the thread %d\n",tno);
    while(1)
    {
        if(curno<5)//replace by conf.CLIENTS_PER_THREAD
        {
            pthread_mutex_lock(&mutx);
            if(!fdlist.empty())
            {               
                newfd=fdlist.back();
                fdlist.pop_back();
                
                ev.data.fd=newfd;
                ev.events= EPOLLIN;    
                epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
                curno++;
                printf("%d taking fd %d from list\n",tno,newfd);
            }
            pthread_mutex_unlock(&mutx);
        }   
        int n = epoll_wait (epfd, events, conf.CLIENTS_PER_THREAD, 1000);
        
        for(int i=0;i<n;i++)
            printf("thread : %d event fd : %d\n",tno,events[i].data.fd);
        
        for(int i=0;i<n;i++)
        {
            
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
	        {
              // error on this fd or the socket is not ready
	            fprintf (stderr, "epoll: closing connection\n");
	            close (events[i].data.fd);
                curno--;
	            continue;
	        }

            bzero(buffer,513);
        
            err = read(events[i].data.fd,buffer,513);
            if (err < 0) {
                error("ERROR reading from socket");
                break;
            }
            //printf("Here is the message: %s\n",buffer);
            printf("Got your Key: %s\n");
            strcat(strcpy(message,"I got your message \n"),getvalue(buffer));
            err = write(events[i].data.fd,message,strlen(message));
            if (err < 0) error("ERROR writing to socket");

        }
    }    
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

    int sockfd, newsockfd;
    socklen_t clilen;
    //char buffer[256];
    struct sockaddr_in  cli_addr;
    int portno ;//= atoi(argv[1]);

    ///
    long thread=0, thread_count=conf.THREAD_POOL_SIZE_INITIAL,active_threads=0;
	pthread_t * thread_handles;
    int err;
    if (pthread_mutex_init(&mutx,NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 0; 
    } 
    ///

    read_config();

    printf("%s",conf.CACHE_REPLACEMENT);
    printf("%d %d %d %d %d\n",conf.LISTENING_PORT_NO,conf.CACHE_ENTRIES,conf.CLIENTS_PER_THREAD,conf.THREAD_POOL_SIZE_INITIAL,conf.THREAD_POOL_GROWTH);
    portno=conf.LISTENING_PORT_NO;
    sockfd= server_init();
    int i=0,clientcount=0,growth=conf.THREAD_POOL_SIZE_INITIAL,toffset=0;
    int reqcount=0 ,reqlimit=REQLIMIT;
    thread_handles=(pthread_t *) malloc(growth*sizeof(pthread_t));
    while(1)
    {    
   
       for(;i<growth;i++)
       {
            void * x= reinterpret_cast<void *>(&i);
            if(err=pthread_create(&thread_handles[i],NULL,worker1,x))
                error("problem creating thread");   
            else
            {
                active_threads++;
                printf("$$ New Worker Thread created, Thread No. : %d\n",i);
            }
            
            
       }

       while(1)
       {
            if(listen(sockfd,5)==-1)
                continue;
            clilen = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            printf("##New Client Connection : fd : %d ##\n",newsockfd);
            if (newsockfd < 0) 
                error("ERROR on accept");
            clientcount=clientcount+1; //change1 for 5,10..
            reqcount++;
            pthread_mutex_lock(&mutx);
            fdlist.push_front(newsockfd);
            pthread_mutex_unlock(&mutx);
             
            if(reqcount==reqlimit)
            {
                //clientcount=0;
                reqcount=0;
                //reqlimit=100;
                growth+=conf.THREAD_POOL_GROWTH;
                break;
            }
            printf("details clientcount %d reqcount %d growth %d\n",clientcount,reqcount,growth);
       }
       //toffset+=5;///change2
       thread_handles=(pthread_t *) realloc((void *)thread_handles,growth*sizeof(pthread_t));
    }

	for(thread=0; thread<active_threads;thread++)
	{
		err=pthread_join(thread_handles[thread],NULL);
		if(err)
		{
			error("Error :");
		}
	}

    free(thread_handles);
    return 0;
}




#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <fcntl.h>
#include <time.h>
#include  <sys/syslog.h>
#include <mqueue.h>   /* mq_* functions */
#include <signal.h>
#include <errno.h>
#define PORT 8080 
#define SA struct sockaddr 
#define MSG_Q_NAME "/MY_MSGQ_3"
#define MAX_MSG_LEN     80


mqd_t msq_id;
  
// Function designed for chat between client and server. 
 void func(int sockfd) 
{ 
    char buff[MAX_MSG_LEN ]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
       
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents  

  	if(mq_send(msq_id, buff, strlen(buff)+1, 0) < 0)
    		{
      if(errno != EAGAIN)
      {
        perror ("Error on sending msg on MsgQ ");
        mq_close(msq_id);
        exit(1);
      }
    }
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) {  
            break; 
        } 
    } 
    mq_close(msq_id);
} 

// Driver function 
int main() 
{   
	pid_t pid, sid;
	int len, fd;
	const time_t timebuf;
	pid = fork(); // create a new process

	if (pid < 0) { // on error exit
		perror("fork");
	exit(EXIT_FAILURE);
	}

	if (pid > 0){  
		printf("Deamon PID: %d\n", pid);	
		exit(EXIT_SUCCESS); // parent process (exit)
	}
	sid = setsid(); // create a new session

	if (sid < 0) { // on error exit
		perror("setsid");
		exit(EXIT_FAILURE);
	}
	// make '/' the root directory
	if (chdir("/") < 0) { // on error exit
		perror("chdir");
		exit(EXIT_FAILURE);
	}
	umask(0);
	close(STDIN_FILENO);  // close standard input file descriptor
	close(STDOUT_FILENO); // close standard output file descriptor
	close(STDERR_FILENO); // close standard error file descriptor

	len = strlen(ctime(&timebuf));

	/* Message Queue*/
	struct mq_attr attr;
  memset(&attr, 0, sizeof attr);
  attr.mq_msgsize = 8192;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;

   msq_id = mq_open(MSG_Q_NAME, O_RDWR | O_CREAT | O_NONBLOCK,
                         0777, &attr);
  if(msq_id == (mqd_t) -1)
  {
     perror("Error on msg Q creation mq_open(): ");
    exit(1);
  }
    /* TCP/IP*/
    int sockfd, connfd, len_; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        exit(0); 
    } 
    else 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        exit(0); 
    } 

  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
 
        exit(0); 
    } 
  
    len_ = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len_); 
    if (connfd < 0) { 
 
        exit(0); 
    } 
  /*pthread_t msID,socketID;
  int rms = pthread_create(&msID, NULL, sendMessage, NULL);
  int rso = pthread_create(&socketID, NULL, func, connfd);
   if (rms|rso)
   {
        
         exit(0);
   }*/

 	 func(connfd) ;
    // After chatting close the socket 
    close(sockfd); 
} 


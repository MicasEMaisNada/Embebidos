//Header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <openssl/ssl.h>

#include <mqueue.h>   /* mq_* functions */
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include  <sys/syslog.h>


//Number of fields in channel
#define NFIELDS 1
//Write key
#define WRITE_KEY "93FSUE68PNHIKDJT"
//Read key
#define READ_KEY "BDJ0QE8EP5FWXBIR"
//Channel number
#define CHANNELN 1280253

//defines
#define URL_THINGSPEAK             "api.thingspeak.com"
#define PORT_THINGSPEAK            443
#define BEGIN_OF_HTTP_REQ_WRITE         "GET /update?api_key="
#define BEGIN_OF_HTTP_REQ_READ         "GET /channels"
#define END_OF_HTTP_REQ            "\r\n\r\n"
#define MAX_SIZE                   9999

#define THINGSPEAK_OFFLINE_ERROR     0
#define THINGSPEAK_CONNECTION_ERROR  -1
#define OPEN_SOCKET_ERROR            -2
#define OPEN_SSL_ERROR               -3
#define PARAMS_ERROR                 -4
#define SEND_OK                      -5   

#define MSG_Q_NAME "/MY_MSGQ_3"
#define MAX_MSG_LEN     80
mqd_t msq_id;
char enviar[MAX_MSG_LEN];
void RecvDataFromThingSpeak();
int main (void)
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
	float FieldValue;
	
	int i;
	char nbytes;

    	//Sets seed of random number generator
	srand(getpid());

	
	//Data to send
	FieldValue=8.0;


	//printf("Sending data to server:\n");
	/*for(i=0;i<NFIELDS; i++)
		printf("Field%d:%f\n",i+1,FieldValue);*/
	struct itimerval itv;
    	signal(SIGALRM,RecvDataFromThingSpeak);
    	itv.it_interval.tv_sec = 5;
    	itv.it_interval.tv_usec = 0;
    	itv.it_value.tv_sec = 5;
    	itv.it_value.tv_usec = 0;
    	setitimer(ITIMER_REAL,&itv,NULL);
	//RecvDataFromThingSpeak(CHANNELN, READ_KEY, sizeof(READ_KEY),response);
	//send data to channel.
	//SendDataToThingSpeak(NFIELDS, FieldValue,WRITE_KEY,sizeof(WRITE_KEY));

	//retrieve data from channel.
	/*nbytes=RecvDataFromThingSpeak(CHANNELN, READ_KEY, sizeof(READ_KEY),response);

	//Prints the response from server
	printf("Response from server: ");
	
	
	//printf("%c", nbytes);
	for(i=0;i<nbytes;i++)
		printf("%c",response[i]);
		
	printf("\n");	
	//tirar só o valor
	int k= 0;
	int mais_um =0;
	for(i=0; i<sizeof(response); i++){
		if(response[i] == '\"')
			k++;
		if(k == 9){
		
		if(mais_um==1)
		printf("%c",response[i]);
		
		mais_um=1;
		}
	}
	
	
	if(mq_send(msq_id, response, strlen(response)+1, 0) < 0){
      	if(errno != EAGAIN)
      	{
        	perror ("Error on sending msg on MsgQ ");
        	mq_close(msq_id);
        	exit(1);
      	}
    	}

	printf("\n");	
	mq_close(msq_id);*/
	while(1);
	mq_close(msq_id);
	
}

void RecvDataFromThingSpeak()
{
	char response[MAX_MSG_LEN];
	char anterior;
	int ChannelNo = CHANNELN;
	char * Key = READ_KEY;
	int SizeOfKey = sizeof(READ_KEY);
	int sockfd, portno;
	struct sockaddr_in serv_addr;
	struct hostent *ServerTCP;
	char ReqString[MAX_SIZE];
	char BeginOfHTTPReq[]=BEGIN_OF_HTTP_REQ_READ;
	char EndOfHTTPReq[]=END_OF_HTTP_REQ;

	SSL *conn;
	SSL_CTX *ssl_ctx;
	char bytes,total_bytes;


	SSL_load_error_strings ();
	SSL_library_init ();
	ssl_ctx = SSL_CTX_new (SSLv23_client_method ());


	//Setting up HTTP Req. string:
	bzero(&ReqString,sizeof(ReqString));
	//sprintf(ReqString,"%s/%d/fields/last?api_key=%s%s",BeginOfHTTPReq,ChannelNo,Key,EndOfHTTPReq);
	sprintf(ReqString,"%s/%d/fields/1/last.json?api_key=%s&results=2%s",BeginOfHTTPReq,ChannelNo,Key,EndOfHTTPReq);
	//printf("%s\n",ReqString);


	//Connecting to ThingSpeak and sending data:
	portno = PORT_THINGSPEAK;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// create an SSL connection and attach it to the socket
	conn = SSL_new(ssl_ctx);
	SSL_set_fd(conn, sockfd);

	//Step 1: opening a socket
	if (sockfd < 0) perror("socket");;
		//return OPEN_SOCKET_ERROR;

	//Step 2: check if ThingSpeak is online
	ServerTCP = gethostbyname(URL_THINGSPEAK);
	if (ServerTCP == NULL) perror("servertcp null");;
		//return THINGSPEAK_OFFLINE_ERROR;

	//Step 3: setting up TCP/IP socket structure
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)ServerTCP->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			ServerTCP->h_length);
	serv_addr.sin_port = htons(portno);



	//Step 4: connecting to ThingSpeak server (via HTTP port / port no. 80)
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		
		perror("connect");
		//return THINGSPEAK_CONNECTION_ERROR;
}


	// perform the SSL/TLS handshake with the server - when on the
	// server side, this would use SSL_accept()
	if (SSL_connect(conn)!= 1){
		perror("ssl connect");
		//return OPEN_SSL_ERROR ;
}

	//Step 5: sending data to ThingSpeak's channel
	SSL_write(conn,ReqString,strlen(ReqString));


	//Wait for response
	/* receive the response */
	bytes = 0;
	total_bytes=0;

	do{
		bytes = SSL_read(conn,response+bytes,MAX_SIZE);
		total_bytes+=bytes;
	}while(bytes);

	//Step 6: close TCP connection
	close(sockfd);

	SSL_CTX_free(ssl_ctx);
	SSL_free(conn);

	//All done!
	
	//Prints the response from server
	printf("Response from server: ");
	
	int i;
	//printf("%c", nbytes);
	//for(i=0;i<total_bytes;i++)
	//	printf("%c",response[i]);
		
	//printf("\n");	
	//tirar só o valor
	int k= 0;
	int mais_um =0;
	
	for(i=0; i<sizeof(response); i++){
		if(response[i] == '\"')
			k++;
		if(k == 9){
		
		if(mais_um==1)
		enviar[0] = response[i];
		
		mais_um=1;
		}
	}
	
	if((anterior != enviar[0]) && (enviar[0] != 0) ){
	if(mq_send(msq_id, enviar, strlen(enviar)+1, 0) < 0){
      	if(errno != EAGAIN)
      	{
        	perror ("Error on sending msg on MsgQ ");
        	mq_close(msq_id);
        	exit(1);
      	}}
      		anterior = enviar[0];}

}




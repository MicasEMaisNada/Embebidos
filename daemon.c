#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <fcntl.h>
#include <time.h>
#include  <sys/syslog.h>
#include <mqueue.h>   /* mq_* functions */
#include <signal.h>
#include <errno.h>

#define MSG_Q_NAME "/MY_MSGQ_3"
#define MAX_MSG_LEN     70
int main(int argc, char *argv[]){
	pid_t pid, sid;
	int len, fd;
	const time_t timebuf;

	pid = fork(); // create a new process

	if (pid < 0) { // on error exit
		syslog(LOG_ERR, "%s\n", "fork");
		exit(EXIT_FAILURE);
	}

	if (pid > 0){  
		printf("Deamon PID: %d\n", pid);	
		exit(EXIT_SUCCESS); // parent process (exit)
	}
	sid = setsid(); // create a new session

	if (sid < 0) { // on error exit
		syslog(LOG_ERR, "%s\n", "setsid");
		exit(EXIT_FAILURE);
	}
	// make '/' the root directory
	if (chdir("/") < 0) { // on error exit
		syslog(LOG_ERR, "%s\n", "chdir");
		exit(EXIT_FAILURE);
	}
	umask(0);
	close(STDIN_FILENO);  // close standard input file descriptor
	close(STDOUT_FILENO); // close standard output file descriptor
	close(STDERR_FILENO); // close standard error file descriptor

	len = strlen(ctime(&timebuf));
	  char msgcontent[MAX_MSG_LEN];
  struct mq_attr attr;
  memset(&attr, 0, sizeof attr);
  attr.mq_msgsize = 8192;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;

  mqd_t msq_id = mq_open(MSG_Q_NAME, O_RDWR | O_CREAT | O_NONBLOCK,
                         0777, &attr);
  if(msq_id == (mqd_t) -1)
  {
     perror("Error on msg Q creation mq_open(): ");
    exit(1);
  }

  // Write 5 msgs on message Q
  for(int i = 0; i < 5; ++i)
  {
  	snprintf(msgcontent, MAX_MSG_LEN, "My MSG %d",i);
    //stringstream s;
    //s << "My Msg " << i;

    if(mq_send(msq_id, msgcontent, strlen(msgcontent)+1, 0) < 0)
    {
      if(errno != EAGAIN)
      {
        perror ("Error on sending msg on MsgQ ");
        mq_close(msq_id);
        exit(1);
      }
    }
    else
    {
      printf( "Sent msg %d",i);
    }

    sleep(1); // Easily see the received message in reader
  }

  mq_close(msq_id);
	while (1) {
		char *buf = malloc(sizeof(char) + len + 1);
		if (buf == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		if ((fd = open("/var/log/example2.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
			perror("open");
			exit(EXIT_FAILURE);
		}
		time((time_t*)&timebuf);
		strncpy(buf, ctime(&timebuf), len + 1);
		write(fd, buf, len + 1);
		close(fd);
		//openlog("ESRG Deamon Test", LOG_PID , LOG_DAEMON);
		syslog(LOG_INFO, "%s\n", "Going to Sleep");
		sleep(15); // 15 seconds
	}
exit(EXIT_SUCCESS);		
}

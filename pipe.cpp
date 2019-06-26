#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <queue>
#include <vector>
#include <string>
#include <thread>
#include <functional>

#define MAXSIZE 10

#define MAX_BUF 10

using namespace std;

priority_queue<int, vector<int>, greater<int>> scheduler;

void encolarenpipe()
{
		
	int fd1;
	char *myfifo = "/tmp/myfifo";
	char buf[MAX_BUF];

	/* open, read, and display the message from the FIFO */
	fd1 = open(myfifo, O_RDONLY);
	read(fd1, buf, MAX_BUF);

	if (strlen(buf) > 0)
	{
		int buf1 = atoi(buf);
		printf("Received in named pipe: %s\n", buf);
		scheduler.push(buf1);
		//printf("TOP %d\n", scheduler.top());
	}
	sprintf(buf, "%s", "");
	close(fd1);
	
}

void die(char *s)
{
	perror(s);
	exit(1);
}

struct msgbuf1
{
	long mtype;
	char mtext[MAXSIZE];
};

int main()
{
	int fd[2], fd2[2], nbytes;
	pid_t childpid;
	char readbuffer[MAX_BUF];
	char rb[MAX_BUF];
	int c;
	pipe(fd);
	pipe(fd2);
	if ((childpid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}


	while (1)
	{
		
		if (childpid == 0)
		{

			thread e_p(encolarenpipe);
			e_p.join();
			close(fd[0]);
			close(fd2[1]);
			// Send "number" through the output side of pipe
			char buf[MAX_BUF];
			if (scheduler.size() > 0)
			{
				sprintf(buf, "%d", scheduler.top());
				write(fd[1], buf, (strlen(buf) + 1));
				//nbytes = read(fd2[0], rb, sizeof(rb));
				while(strlen(rb)!=2){
					nbytes = read(fd2[0], rb, sizeof(rb));
					
				}
			
				sprintf(rb, "%s", "");
				scheduler.pop();
			}
		}
		else
		{
			close(fd[1]);
			close(fd2[0]);
			// Read in a string from the pipe
			nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
			if (strlen(readbuffer) > 0)
			{
				printf("Received in unnamed pipe: %s\n", readbuffer);
				write(fd2[1], "ok", (strlen("ok")+1));
			} //codigo juango
				/*    int msqid;
        int msgflg = IPC_CREAT | 0666;
        key_t key;
        struct msgbuf sbuf;
        size_t buflen;

        key = 4321;

        if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
          die("msgget");

        //Message Type
        sbuf.mtype = 1;

//        printf("Enter a message to add to message queue : ");
//        scanf("%[^\n]",sbuf.mtext);
        //        getchar();
        sprintf(sbuf.mtext,"%s",readbuffer);//sbuf.mtext = readbuffer;
        buflen = strlen(sbuf.mtext) + 1 ;

        if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
        {
            printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
            
            die("msgsnd");
        }

        else
            printf("Message Sent\n");*/
		}

		//end juango

		//sleep(2);
		//}
	}

	return 0;
}

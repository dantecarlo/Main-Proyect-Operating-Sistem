#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXSIZE     10

#define MAX_BUF 10

void die(char *s)
{
  perror(s);
  exit(1);
}

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};


int main()
{
	int fd[2], nbytes;
	pid_t childpid;
	char readbuffer[MAX_BUF];
	int c;
	pipe(fd);
	if ((childpid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}

	while (1)
	{

		//sleep(1);

		if (childpid == 0)
		{ //printf("child\n");
			int fd1;
			char *myfifo = "/tmp/myfifoimpair";
			char buf[MAX_BUF];

			/* open, read, and display the message from the FIFO */
			fd1 = open(myfifo, O_RDONLY);
			read(fd1, buf, MAX_BUF);
			//			printf("parent process ha leido el mensaje\n");
			if (strlen(buf) > 0)
				printf("Received in named pipe: %s\n", buf);
			close(fd1);
			if (strlen(buf) > 0)
			{ //	printf("sent to parent\n");
				// Parent process closes up input side of pipe
				close(fd[0]);

				// Send "number" through the output side of pipe
				write(fd[1], buf, (strlen(buf) + 1));
				//        		exit(0);
			}
			sprintf(buf, "%s", "");
		}
		else
		{ //printf("parent\n");
			// Child process closes up output side of pipe
			close(fd[1]);
			// Read in a string from the pipe
			nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
			if (strlen(readbuffer) > 0)
				printf("Received in unnamed pipe: %s\n", readbuffer);
            //codigo juango
        int msqid;
        int msgflg = IPC_CREAT | 0666;
        key_t key;
        struct msgbuf sbuf;
        size_t buflen;

        key = 1234;

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
            printf("Message Sent\n");
		}

//end juango

		sleep(2);
	}
	return 0;
}

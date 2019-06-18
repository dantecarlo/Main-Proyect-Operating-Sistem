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

#define MAXSIZE 10

#define MAX_BUF 10

using namespace std;

class mycomp
{
public:
	bool operator()(char *a, char *b)
	{
		int n = stoi(a);
		int m = stoi(b);
		return a > b;
	}
};

priority_queue<char *, vector<char *>, mycomp> scheduler;

void encolarenpipe()
{
	int fd1;
	char *myfifo = "/tmp/myfifo";
	char buf[MAX_BUF];

	/* open, read, and display the message from the FIFO */
	fd1 = open(myfifo, O_RDONLY);
	read(fd1, buf, MAX_BUF);
	//			printf("parent process ha leido el mensaje\n");
	if (strlen(buf) > 0){
		printf("Received in named pipe: %s\n", buf);
		scheduler.push(buf);
	}
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
	int fd[2],fd2[2], nbytes;
	pid_t childpid;
	char readbuffer[MAX_BUF];
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
		{ //printf("child\n");

			thread e_p (encolarenpipe);
			e_p.join();

			if (!scheduler.empty())
			{
				close(fd[0]);
				close(fd2[1]);
				// Send "number" through the output side of pipe
				write(fd[1], scheduler.top(), (strlen(scheduler.top()) + 1));
				scheduler.pop();//        		exit(0);
			}
			//sprintf(buf, "%s", "");
		}
		else
		{ //printf("parent\n");
			// Child process closes up output side of pipe
			close(fd[1]);
			close(fd2[0]);
			// Read in a string from the pipe
			nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
			printf("nbytes %d: ", nbytes);
			if (strlen(readbuffer) > 0){
				printf("Received in unnamed pipe: %s\n", readbuffer);
			}//codigo juango
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
	}
	return 0;
}

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_BUF 10

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
			char *myfifo = "/tmp/myfifopair";
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
		}
		sleep(2);
	}
	return 0;
}

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <queue>
using namespace std;
queue<int> fifo; 
int GS=0;
void randomn()
{
	int numero = rand () % 100000;
	fifo.push (numero);
	printf("random number generated: %d\n", numero);
}

void sighandler(int signum)
{
	GS=signum;
	//printf("Caught signal %d\n", signum);	
	thread rn(randomn);
	rn.join();
}


void enviar(int signal)
{
	int fd;
	char send_[10];
	sprintf(send_, "%d", signal);
	char *myfifo = "/tmp/myfifo";
	mkfifo(myfifo, 0666);
	fd = open(myfifo, O_WRONLY);
	write(fd, send_, sizeof(send_));
	printf("number %s sent\n", send_);
	close(fd);
	unlink(myfifo);
}

void desencolar ()
{
	while (1)
	{
		
		if(fifo.size()>0){
		enviar(fifo.front());
		fifo.pop();
		}		
	}
}

int main()
{
	signal(2, sighandler);
	thread d(desencolar);
	d.join();

	return 0;
}

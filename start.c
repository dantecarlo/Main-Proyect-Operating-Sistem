#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int GS; // signal
void sighandler(int signum) {
	GS = signum;
	printf("Caught signal %d\n", GS);
}

/*int randomn()
{
	srand (time(NULL)); 
	int numero = rand () % 10 + 1;
	return numero;
}*/

void enviarp(int signal){
	int fd;
	char send_[10];
	sprintf(send_,"%d",signal);
	char * myfifo = "/tmp/myfifopair";
	mkfifo(myfifo, 0666);
	fd = open(myfifo, O_WRONLY);
	write(fd, send_, sizeof(send_));
	printf("pair number %s sent\n",send_);
	close(fd);
	unlink(myfifo);
}

void enviari(int signal){
	int fd;
	//int enviado = GS; 
	char send_[10];
	sprintf(send_,"%d",signal);
	char * myfifo = "/tmp/myfifoimpair";
	mkfifo(myfifo, 0666);
	fd = open(myfifo, O_WRONLY);
	write(fd, send_, sizeof(send_));
	printf("odd number %s sent\n",send_);
	close(fd);
	unlink(myfifo);
}

int main()
{
	//GS = 2;
	
	signal(2, sighandler);
	signal(4, sighandler);

	while(1){
		if(GS==2){

			int num = rand()%100000;
			printf("random number generated: %d\n", num);
			if(num%2==0)
				enviarp(num);
			else
				enviari(num);
			GS=0;
		}

			sleep(1);
	}


    return 0;
}

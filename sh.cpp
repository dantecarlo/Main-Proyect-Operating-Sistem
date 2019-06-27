#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stack>
#include <string>
#include <thread>
using namespace std;

#define MAXSIZE 10

stack<int> scheduler;
int msqid;
key_t key1;
struct msgbuf rcvbuffer;

void encolar()
{

    key1 = 4321;

    if ((msqid = msgget(key1, 0666)) < 0)
        die("msgget()");

    //Receive an answer of message type 1.
    if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
        die("msgrcv");
    int num = atoi(rcvbuffer.mtext);
    scheduler.push(num);
    printf("message recieved %s\n", rcvbuffer.mtext);
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

main()
{
    while (1)
    {

        thread e(encolar);

        //char c;
        int shmid;
        key_t key;
        char *shm, *s;
        /*
     * We'll name our shared memory segment
     * "5678".
     */
        key = 5679;

        /*
     * Create the segment.
     */
        if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        {
            perror("shmget");
            exit(1);
        }

        /*
     * Now we attach the segment to our data space.
     */
        if ((shm = (char *)shmat(shmid, NULL, 0)) == (char *)-1)
        {
            perror("shmat");
            exit(1);
        }

        /*
     * Now put some things into the memory for the
     * other process to read.
     */
        s = shm;

        int len = strlen(rcvbuffer.mtext);
        //        printf("len %d", len);
        for (int i = 0; i < len; i++)
            *s++ = rcvbuffer.mtext[i];
        *s = NULL;

        /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
        /*while (*shm != '*')
            sleep(1);*/
    }
    exit(0);
}

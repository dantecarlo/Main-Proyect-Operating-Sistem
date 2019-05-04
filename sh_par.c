#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define MAXSIZE 10

void die(char *s)
{
    perror(s);
    exit(1);
}

struct msgbuf
{
    long mtype;
    char mtext[MAXSIZE];
};

main()
{
    while (1)
    {

        int msqid;
        key_t key1;
        struct msgbuf rcvbuffer;

        key1 = 4321;

        if ((msqid = msgget(key1, 0666)) < 0)
            die("msgget()");

        //Receive an answer of message type 1.
        if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
            die("msgrcv");

        printf("message recieved %s\n", rcvbuffer.mtext);

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
        if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
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
        while (*shm != '*')
            sleep(1);
    }
    exit(0);
}

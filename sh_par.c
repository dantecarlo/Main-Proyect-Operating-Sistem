#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 10

void shm_server(char* num)
{
    printf("entra %s\n", num);
    char c;
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
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
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
    printf("%s\n", num);
    printf("primer numero %s\n", num);
    int len = strlen(num);
    printf("len %sd", len);
    for (int i=0; i< len; i++)
        *s++ = num[i];
    
    *s++;
    printf("adri aquiii :%s\n", *s);

    *s = NULL;

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while (*shm != '*')
        sleep(1);

    exit(0);
}


void die(char *s)
{
  perror(s);
  exit(1);
}


#define MAXSIZE     10

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

char* rc_msj()
{
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 4321;

    if ((msqid = msgget(key, 0666)) < 0)
      die("msgget()");


     //Receive an answer of message type 1.
    if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
      die("msgrcv");
    
    printf("%s\n", rcvbuffer.mtext);

    return rcvbuffer.mtext;
}

main()
{
    while(1)
    {
        // char num[MAXSIZE];
        // sprintf(num,"%s",rc_msj());
        // printf("adri aquiii x2:%s\n", num);
        shm_server(rc_msj());
        sleep(2);
    }
}
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <string>
#include <thread>
using namespace std;

#define SHMSZ 10
int shmid;
    key_t key;
    char *shm, *s;
void shm_client()
{
    /*
     * Now read what the server put in the memory.
     */

    FILE *fp;

    fp = fopen("numbers.txt", "a");
    bool b=0;
    for (s = shm; *s != NULL; s++){
        if (*s == '*'){
        b=1;
        break;
        }
        fputc(*s, fp);
    }
        
    //putchar(*s);
    //putchar('\n');
    if(b==0)
        fputc('\n', fp);

    fclose(fp);

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';
    

    //exit(0);
}

main()
{
    key = 5679;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = (char*)shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    while (1)
    {
        shm_client();
       
    }
}

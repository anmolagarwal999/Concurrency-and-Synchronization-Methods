#define _POSIX_C_SOURCE 199309L //required for clock
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>

int *shareMem(size_t sz)
{
    /*IPC_CREAT
              Create a new segment.  If this flag is not used, then shmget()
              will find the segment associated with key and check to see if
              the user has permission to access the segment.*/

    /*If a client wants to use a shared memory created with IPC_PRIVATE, 
    it must be a child process of the server, created after the parent
    has obtained the shared memory, so that the private key value can be passed to the child when it is created. */

    key_t mem_key = IPC_PRIVATE;
    int shm_id = shmget(mem_key, sz, IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror(" shmget error >");
        exit(1);
    }
    /*System call shmat() accepts a shared memory ID, shm_id, and attaches the indicated 
     shared memory to the program's address space. The returned value is a 
     pointer of type (void *) to the attached shared memory. Thus, casting is usually necessary.*/
    int *shm_ptr = (int *)shmat(shm_id, NULL, 0);

    if (shm_ptr == (void *)(-1))
    {
        perror(" shmat error ");
        exit(1);
    }

    return shm_ptr;
}

int main()
{
    int n, i, j, k;
    scanf("%d", &n);

    int *arr = shareMem(n);
    printf("Enter array elements\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    int pid1 = fork();
    if (pid1 == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            arr[i] = -1;
        }
        return 0;
    }

    int pid2 = fork();
    if (pid2 == 0)
    {
        for (int i = 2; i < n; i++)
        {
            printf("i is %d\n", i);
            arr[i] = -2;
        }
        return 0;
    }
    int wstatus;
    waitpid(pid1, &wstatus, 0);
    waitpid(pid2, &wstatus, 0);
    for (i = 0; i < n; i++)
    {
        printf("arr element is %d\n", arr[i]);
    }
}
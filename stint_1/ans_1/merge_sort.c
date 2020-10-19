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
#define part printf("--------------------------------\n");
typedef long long LL;
//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"
#define ANSI_RESET "\x1b[0m"

/////////////////////////////////////////////////////
int *org_arr, *thread_arr;
struct thread_data
{
    int *ptr_to_arr;
    int l, r;
};
int shm_id;
////////////////////////////////////////////////////////
void swap_elems(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}
int *get_shared_mem(size_t sz)
{
    /*IPC_CREAT
              Create a new segment.  If this flag is not used, then shmget()
              will find the segment associated with key and check to see if
              the user has permission to access the segment.*/

    /*If a client wants to use a shared memory created with IPC_PRIVATE, 
    it must be a child process of the server, created after the parent
    has obtained the shared memory, so that the private key value can be passed to the child when it is created. */
    printf("Size requested is %ld\n", sz);
    //exit(0);
    key_t mem_key = IPC_PRIVATE;
    shm_id = shmget(mem_key, sz, IPC_CREAT | 0666);
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

void merge_halves(int *arr, int l, int m, int r)
{
    int i, j, k;

    //l to m
    int sz1 = m - l + 1;

    //m+1 to r
    int sz2 = r - m;

    int left_arr[sz1], right_arr[sz2];

    for (i = 0; i < sz1; i++)
    {
        left_arr[i] = arr[l + i];
    }

    for (j = 0; j < sz2; j++)
    {
        right_arr[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;
    while (i < sz1 && j < sz2)
    {
        if (left_arr[i] <= right_arr[j])
        {
            arr[k] = left_arr[i];
            i++;
        }
        else
        {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < sz1)
    {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < sz2)
    {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

void merge_sort(int *arr, int l, int r)
{
    if (l < r)
    {
        //printf("l is %d and r is %d\n", l, r);
        int sz = r - l + 1;

        if (sz < 5)
        {
            int i = 0, idx = 0;

            for (i = l; i <= r; i++)
            {
                idx = i;
                for (int j = i + 1; j <= r; j++)
                {
                    if (arr[j] < arr[idx])
                    {
                        idx = j;
                    }
                }

                if (idx != i)
                {
                    swap_elems(&arr[i], &arr[idx]);
                }
            }

            // for(i=0;i<sz;i++)
            // {
            //     printf("After initial sorting, elements are %d\n",arr[i]);
            // }
            return;
        }

        int m = l + (r - l) / 2;

        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);

        merge_halves(arr, l, m, r);
    }
}

void *threaded_merge_sort(void *args)
{
    struct thread_data *ptr = (struct thread_data *)args;
    int l = ptr->l;
    int r = ptr->r;
    int *arr = ptr->ptr_to_arr;
    if (l < r)
    {
        //printf("l is %d and r is %d\n", l, r);
        int sz = r - l + 1;

        if (sz < 5)
        {
            int i = 0, idx = 0;

            for (i = l; i <= r; i++)
            {
                idx = i;
                for (int j = i + 1; j <= r; j++)
                {
                    if (arr[j] < arr[idx])
                    {
                        idx = j;
                    }
                }

                if (idx != i)
                {
                    swap_elems(&arr[i], &arr[idx]);
                }
            }

            // for(i=0;i<sz;i++)
            // {
            //     printf("After initial sorting, elements are %d\n",arr[i]);
            // }
            return NULL;
        }
        else
        {
            // printf(BRED "In threaded merge sort\n");
            // exit(-1);
            int m = l + (r - l) / 2;
            pthread_t tid1, tid2;

            struct thread_data a1;
            a1.l = l;
            a1.r = m;
            a1.ptr_to_arr = arr;
            pthread_create(&tid1, NULL, threaded_merge_sort, &a1);

            //sort right half array
            struct thread_data a2;
            a2.l = m + 1;
            a2.r = r;
            a2.ptr_to_arr = arr;
            pthread_create(&tid2, NULL, threaded_merge_sort, &a2);

            //wait for the two halves to get sorted
            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);

            merge_halves(arr, l, m, r);
            return NULL;
        }
        printf(BRED "Weird stuff here Miami\n" ANSI_RESET);
    }
}

void process_merge_sort(int *shared_arr, int l, int r)
{
    if (l < r)
    {
        //As long as array size is not one
        int sz = r - l + 1;

        if (sz < 5)
        {
            int i = 0, idx = 0;

            for (i = l; i <= r; i++)
            {
                idx = i;
                for (int j = i + 1; j <= r; j++)
                {
                    if (shared_arr[j] < shared_arr[idx])
                    {
                        idx = j;
                    }
                }

                if (idx != i)
                {
                    swap_elems(&shared_arr[i], &shared_arr[idx]);
                }
            }

            // for(i=0;i<sz;i++)
            // {
            //     printf("After initial sorting, elements are %d\n",arr[i]);
            // }
            return;
        }
        else
        {

            int m = l + (r - l) / 2;

            int pid1, pid2;

            pid1 = fork();
            if (pid1 == 0)
            {
                process_merge_sort(shared_arr, l, m);

                //Why there is a reason to use _exit() instead of exit() for this question ?
                //https://stackoverflow.com/a/3657687/6427607
                _exit(1);
            }
            else if (pid1 == -1)
            {
                perror("Fork_1: ");
                exit(-1);
            }
            else
            {

                pid2 = fork();
                if (pid2 == 0)
                {
                    process_merge_sort(shared_arr, m + 1, r);
                    _exit(1);
                }
                else if (pid2 == -1)
                {
                    perror("Fork_2: ");
                    exit(-1);
                }
                else
                {
                    int status;
                    waitpid(pid1, &status, 0);
                    waitpid(pid2, &status, 0);
                    merge_halves(shared_arr, l, m, r);
                }
            }
        }
    }
}
void print_arr(int *a, int n)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void copy_arr(int *arr, int *send_arr, int n, int id)
{
    for (int i = 0; i < n; i++)
    {
        printf("i is %d and id is %d\n", i, id);
        send_arr[i] = arr[i];
    }
}
int get_random_int(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}
int main()
{
    int n;
    printf("Enter value of n\n");
    scanf("%d", &n);
    // n=5000;
    /////////////////////////////////////////////////////////////
    //for process merge sort
    int *shared_arr = get_shared_mem((n + 1) * sizeof(int));

    printf("size allotted is %ld\n", sizeof(shared_arr));
    //  return 0;

    //original array
    org_arr = (int *)malloc(sizeof(int) * n);

    //making copy for threaded merge sort
    thread_arr = (int *)malloc(sizeof(int) * (n + 1));

    //////////////////////////////////////////////////////////////

    int arr_size = n;

    printf("Input n elements\n");
    for (int i = 0; i < n; i++)
    {
        //scanf("%d", &org_arr[i]);
        // printf("i is %d\n",i);
        org_arr[i] = get_random_int(1, 10000);
    }
    ///////////////////////////////////////////////////////////////
    copy_arr(org_arr, shared_arr, n, 1);
    copy_arr(org_arr, thread_arr, n, 2);
    //////////////////////////////////////////////////////////////////
    long double st, en;
    ////////////////////////////////////////////////////////////////////
    //Concurrent MERGE SORT
    struct timespec ts;
    part;
    printf("Before concurrent merge sort, contents are\n");
    print_arr(shared_arr, n);
    printf("Running concurrent_mergesort for n = %d\n", n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("%lld.%.9ld\n", (long long)ts.tv_sec, ts.tv_nsec);

    process_merge_sort(shared_arr, 0, n - 1);
    printf("After concurrent merge sort, contents are\n");
    print_arr(shared_arr, n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    en = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("%lld.%.9ld\n", (long long)ts.tv_sec, ts.tv_nsec);

    long double t1 = en - st;
    printf("time taken = %Lf seconds\n", t1);
    part;

    // Detach Shared Memory
    if (shmdt(shared_arr) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }
    ///////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    //Threaded MERGE SORT
    part;
    printf("Before THREADED merge sort, contents are\n");
    print_arr(thread_arr, n);
    printf("Running THREADED_mergesort for n = %d\n", n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("%lld.%.9ld\n", (long long)ts.tv_sec, ts.tv_nsec);

    struct thread_data send_data;
    send_data.l = 0;
    send_data.r = n - 1;
    send_data.ptr_to_arr = thread_arr;
    pthread_t tid;
    pthread_create(&tid, NULL, threaded_merge_sort, &send_data);
    pthread_join(tid, NULL);
    printf("After THREADED merge sort, contents are\n");
    print_arr(thread_arr, n);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    en = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("%lld.%.9ld\n", (long long)ts.tv_sec, ts.tv_nsec);

    long double t2 = en - st;
    printf("time taken = %Lf\n", t2);
    part;
    ///////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //Normal MERGE SORT
    part;
    printf("Before NORMAL merge sort, contents are\n");
    print_arr(org_arr, n);
    printf("Running NORMAL_mergesort for n = %d\n", n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("%lld.%.9ld\n", (long long)ts.tv_sec, ts.tv_nsec);

    merge_sort(org_arr, 0, n - 1);
    printf("After NORMAL merge sort, contents are\n");
    print_arr(org_arr, n);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    en = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("%lld.%.9ld\n", (long long)ts.tv_sec, ts.tv_nsec);

    long double t3 = en - st;
    printf("time taken = %Lf\n", t3);
    part;
    ///////////////////////////////////////////////////////////////////////

    //t1->process
    //t2->threaded
    //t3->normal

    printf("-------------Summary--------------\n");
    printf("Time for process merge sort is %Lf\n", t1);
    printf("Time for threaded merge sort is %Lf\n", t2);
    printf("Time for normal merge sort is %Lf\n", t3);

    printf("Normal mergesort take %Lf of the time taken by Process merge_sort\n", t3 / t1);
    printf("Normal mergesort take %Lf of the time taken by Threaded merge_sort\n", t3 / t2);
    printf("Threaded mergesort take %Lf of the time taken by Process merge_sort\n", t2 / t1);

    return 0;
}

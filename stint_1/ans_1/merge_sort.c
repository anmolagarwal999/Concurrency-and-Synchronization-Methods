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

void merge_halves(int arr[], LL l, LL m, LL r)
{
    LL i, j, k;

    //l to m
    LL sz1 = m - l + 1;

    //m+1 to r
    LL sz2 = r - m;

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

void merge_sort(int arr[], int l, int r)
{
    if (l < r)
    {
        //printf("l is %d and r is %d\n", l, r);
        LL sz = r - l + 1;

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

        LL m = l + (r - l) / 2;

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
        LL sz = r - l + 1;

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

            LL m = l + (r - l) / 2;
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
        LL sz = r - l + 1;

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

            LL m = l + (r - l) / 2;

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

void copy_arr(int *arr, int *send_arr, LL n)
{
    for (int i = 0; i < n; i++)
    {
        send_arr[i] = arr[i];
    }
}

int main()
{
    LL n;
    printf("Enter value of n\n");
    scanf("%lld", &n);
    /////////////////////////////////////////////////////////////
    //for process merge sort
    int *shared_arr = get_shared_mem(n + 1);

    //original array
    org_arr = (int *)malloc(sizeof(int) * n);

    //making copy for threaded merge sort
    thread_arr = (int *)malloc(sizeof(int) * n);

    //////////////////////////////////////////////////////////////

    int arr_size = n;
    printf("Input n elements\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &org_arr[i]);
    }
    ///////////////////////////////////////////////////////////////
    copy_arr(org_arr, shared_arr, n);
    copy_arr(org_arr, thread_arr, n);
    //////////////////////////////////////////////////////////////////
    long double st, en;
    ////////////////////////////////////////////////////////////////////
    //Concurrent MERGE SORT
    struct timespec ts;
    part;
    printf("Before concurrent merge sort, contents are\n");
    print_arr(shared_arr, n);
    printf("Running concurrent_mergesort for n = %lld\n", n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;

    process_merge_sort(shared_arr, 0, n - 1);
    printf("After concurrent merge sort, contents are\n");
    print_arr(shared_arr, n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    en = ts.tv_nsec / (1e9) + ts.tv_sec;
    long double t1 = en - st;
    printf("time taken = %Lf seconds\n", t1);
    part;
    ///////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    //Threaded MERGE SORT
    part;
    printf("Before THREADED merge sort, contents are\n");
    print_arr(thread_arr, n);
    printf("Running THREADED_mergesort for n = %lld\n", n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;
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
    long double t2 = en - st;
    printf("time taken = %Lf\n", t2);
    part;
    ///////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //Normal MERGE SORT
    part;
    printf("Before NORMAL merge sort, contents are\n");
    print_arr(org_arr, n);
    printf("Running NORMAL_mergesort for n = %lld\n", n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;
    merge_sort(org_arr, 0, n - 1);
    printf("After NORMAL merge sort, contents are\n");
    print_arr(org_arr, n);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    en = ts.tv_nsec / (1e9) + ts.tv_sec;
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

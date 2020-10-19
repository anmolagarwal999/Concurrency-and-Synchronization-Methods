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
#define part fprintf(stderr, "-----------------------------------------\n");
#define part2 fprintf(stderr, "====================================================\n");
#define part3 fprintf(stderr, "############################################################\n");
#define debug(x) printf("\n\"%s\" is: %d\n", #x, x);
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

int l_normal = 0, l_threaded = 0, l_process = 0;
long double t_normal, t_process, t_threaded;
int *shared_arr;

/////////////////////////////////////////////////////
int *org_arr;    //original array
int *thread_arr; //array concerned with thread
long double st, en;
struct timespec ts;
int n;

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
            l_normal++;
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
            l_threaded++;
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
        return NULL;
    }
    return NULL;
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

void initiate_threaded_merge_sort()
{

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

    t_threaded = en - st;
    printf("time taken = %Lf\n", t_threaded);
    part;
    ///////////////////////////////////////////////////////////////////////
}

int main()
{
    printf("Enter value of n\n");
    scanf("%d", &n);

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
    copy_arr(org_arr, thread_arr, n, 2);
    //////////////////////////////////////////////////////////////////

    initiate_threaded_merge_sort();




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

    t_normal = en - st;
    printf("time taken = %Lf\n", t_normal);
    part;
    ///////////////////////////////////////////////////////////////////////

    //t1->process
    //t2->threaded
    //t3->normal

    printf("Time for threaded merge sort is %Lf\n", t_threaded);
    printf("Time for normal merge sort is %Lf\n", t_normal);

    printf("Normal mergesort take %Lf of the time taken by Threaded merge_sort\n", t_normal / t_threaded);

    debug(l_threaded);
    debug(l_normal);

    return 0;
}

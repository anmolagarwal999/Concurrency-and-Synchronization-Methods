# Q1: Merge sort performance
- Anmol Agarwal, 2019101668

## Running the program

```bash
make
```
## Results

| n | Normal/Process_merge_sort | Normal/threaded_merge_sort |Threaded/Process_merge_sort |
| :---: | :---: | :---: |:---: |
| 16 | 0.033531 | 0.056632 |  0.592089 |
| 256 | 0.009413 |0.011030 | 0.853406  |
| 512 | 0.011753 | 0.014915 |0.788041 |
| 1024 | 0.008620 |  0.016880 | 0.510677 |
| 4096 | 0.266196  | 0.607295  | 0.438331 |

## Approach 1: Process merge sort
The parent process creates two child process and offloads left half of his job to the first child and the right half of the job to second child. This goes recursively and stops only when the job is to sort <5 elements.

```c=

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
```

## Approach 2: Threaded merge sort
Even though there is no concept of parent-child in threads and all threads of a process are siblings of each other, for visualization purposes, thread N, creates threads 2N+1 and 2(N+1) and offloads one half of its job to each one of them.  This goes recursively and stops only when the job is to sort <5 elements.

```c=
void *threaded_merge_sort(void *args)
{
    struct thread_data *ptr = (struct thread_data *)args;
    int l = ptr->l;
    int r = ptr->r;
    int *arr = ptr->ptr_to_arr;
    if (l < r)
    {
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

            return NULL;
        }
        else
        {
            // printf(BRED "In threaded merge sort\n");
            // exit(-1);
            int m = l + (r - l) / 2;
            pthread_t tid1, tid2;

            struct thread_data left_inp;
            left_inp.l = l;
            left_inp.r = m;
            left_inp.ptr_to_arr = arr;
            pthread_create(&tid1, NULL, threaded_merge_sort, &left_inp);

            struct thread_data right_inp;
            right_inp.l = m + 1;
            right_inp.r = r;
            right_inp.ptr_to_arr = arr;
            pthread_create(&tid2, NULL, threaded_merge_sort, &right_inp);

            //wait for the two halves to get sorted
            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);

            merge_halves(arr, l, m, r);
            return NULL;
        }
        return NULL;
    }
    return NULL;
}
```

## Approach 3: Normal merge sort
Based on divide and conquer
```c=
void merge_sort(int *arr, int l, int r)
{
    if (l < r)
    {
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

            return;
        }

        int m = l + (r - l) / 2;

        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);

        merge_halves(arr, l, m, r);
    }
}

```

# Observations and explanations
* Normal merge sort seems to work faster than Process merge sort even for large 'n'.Firstly,process forking adds significant time delay to overall performance due to time spent in duplicating the address space, PCB etc. We know that system calls (fork() in this case) add severe time delay. This overhead/process_creation multiplied by the number of processes created adds upto significant overhead. The cost of context switch is also very large and may explain the observaton.

* Normal merge sort seems to work faster than threaded merge sort even for large 'n'. Creation of multiple threads always come with overhead and in this experiment,it seems that the overhead is bigger than the actual savings/advantages of simultaneously performing several computations. Each thread has its own stack, TCB etc. Firstly, this overhead may be quite significant when multiplied by number of threads created.Also, after a certain number threads have been created,threads will spend most of its time communicating with each other rather than processing data. With the creation of each processing thread, comes an added cost in terms of an overhead. Also, there is no guarantee that all created threads run on different cores and they may end up competing with each other. 

* Another possible explanation for the poor performance for threaded and process merge sort is as follows: Consider that the threads/processes share the same cache.Now, if left_child loads on the left half to the cache. Now, if the right_child starts to access the right half of the array, there is a cache miss since cache is filled with left half of the array and now, the right half is copied to the cache. This inefficiency may go on and on and eventually leads to threaded/process merge sort losing to normal(sequential) merge sort.

* Although now always, I noticed that about 90% of the time, threaded merge sort was quicker than process merge sort. A possible explanation for this maybe that thread creation doesn't involve costly activity of duplicating the address space, separate PCB etc. 

Per process items             | Per thread items
------------------------------|-----------------
Address space                 | Program counter
Global variables              | Registers
Open files                    | Stack
Child processes               | State
Pending alarms                |
Signals and signal handlers   |
Accounting information        |

#include <stdio.h>
#include <stdlib.h>

void swap_elems(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

void merge_halves(int arr[], int l, int m, int r)
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

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
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
        printf("l is %d and r is %d\n",l,r);
        int sz = r - l + 1;

        if (sz < 5)
        {
            int i = 0, idx = 0;

            for (i = 0; i < sz; i++)
            {
                idx = i;
                for (int j = i + 1; j < sz; j++)
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

void printArray(int A[], int size)
{
    int i;
    int n=size;
    for (i = 0; i < n; i++)
        printf(" elem is %d ", A[i]);
    printf("\n");
}

int main()
{
    int n;
    scanf("%d", &n);
    printf("n is %d\n",n);
    int arr[n];
    for(int i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }

    int arr_size =n;

    printf("Given array is \n");
    printArray(arr, arr_size);

    merge_sort(arr, 0, arr_size - 1);

    printf("\nSorted array is \n");
    printArray(arr, arr_size);
    return 0;
}

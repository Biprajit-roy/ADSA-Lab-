#include <stdio.h>

// Function to swap two elements
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to find the median of three numbers (first, middle, last)
int medianOfThree(int arr[], int low, int high)
{
    int mid = (low + high) / 2;

    if (arr[low] > arr[mid])
        swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high])
        swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high])
        swap(&arr[mid], &arr[high]);

    // After sorting, arr[mid] is the median
    swap(&arr[mid], &arr[high - 1]); // Hide pivot at high-1
    return arr[high - 1];
}

// Partition function using median pivot
int partition(int arr[], int low, int high)
{
    int pivot = medianOfThree(arr, low, high);
    int i = low;
    int j = high - 1;

    while (1)
    {
        while (arr[++i] < pivot)
            ;
        while (arr[--j] > pivot)
            ;
        if (i < j)
            swap(&arr[i], &arr[j]);
        else
            break;
    }
    swap(&arr[i], &arr[high - 1]); // Restore pivot
    return i;
}

// QuickSort function
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

int main()
{
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    quickSort(arr, 0, n - 1);

    printf("Sorted array in ascending order:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}

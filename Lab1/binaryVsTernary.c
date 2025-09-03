#include <stdio.h>

// Binary Search
int binarySearch(int arr[], int left, int right, int key, int *comparisons)
{
    while (left <= right)
    {
        (*comparisons)++;
        int mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

// Ternary Search
int ternarySearch(int arr[], int left, int right, int key, int *comparisons)
{
    if (right >= left)
    {
        int mid1 = left + (right - left) / 3;
        int mid2 = right - (right - left) / 3;

        (*comparisons)++;
        if (arr[mid1] == key)
            return mid1;

        (*comparisons)++;
        if (arr[mid2] == key)
            return mid2;

        if (key < arr[mid1])
            return ternarySearch(arr, left, mid1 - 1, key, comparisons);
        else if (key > arr[mid2])
            return ternarySearch(arr, mid2 + 1, right, key, comparisons);
        else
            return ternarySearch(arr, mid1 + 1, mid2 - 1, key, comparisons);
    }
    return -1;
}

int main()
{
    int sizes[] = {10, 100, 1000, 10000, 100000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("| Array Size | Binary Search Comparisons | Ternary Search Comparisons |\n");

    for (int s = 0; s < numSizes; s++)
    {
        int n = sizes[s];
        int arr[n];

        // generate sorted array: 1, 2, 3, ..., n
        for (int i = 0; i < n; i++)
            arr[i] = i + 1;

        int key = arr[n / 2]; // search for middle element

        int binaryComparisons = 0, ternaryComparisons = 0;

        binarySearch(arr, 0, n - 1, key, &binaryComparisons);
        ternarySearch(arr, 0, n - 1, key, &ternaryComparisons);

        printf("| %9d | %25d | %28d |\n", n, binaryComparisons, ternaryComparisons);
    }

    return 0;
}

#include <stdio.h>

// Recursive Binary Search
int recursiveBinarySearch(int arr[], int low, int high, int key)
{
    if (low > high)
        return -1; // Key not found

    int mid = (low + high) / 2;

    if (arr[mid] == key)
        return mid;
    else if (key < arr[mid])
        return recursiveBinarySearch(arr, low, mid - 1, key);
    else
        return recursiveBinarySearch(arr, mid + 1, high, key);
}

// Iterative (Non-Recursive) Binary Search
int iterativeBinarySearch(int arr[], int n, int key)
{
    int low = 0, high = n - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        if (arr[mid] == key)
            return mid;
        else if (key < arr[mid])
            high = mid - 1;
        else
            low = mid + 1;
    }

    return -1; // Key not found
}

int main()
{
    int n, key, i, result;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d elements in sorted order:\n", n);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Enter the key to search: ");
    scanf("%d", &key);

    // Iterative Search
    result = iterativeBinarySearch(arr, n, key);
    if (result != -1)
        printf("Iterative Search: Key %d found at index %d\n", key, result);
    else
        printf("Iterative Search: Key %d not found\n", key);

    // Recursive Search
    result = recursiveBinarySearch(arr, 0, n - 1, key);
    if (result != -1)
        printf("Recursive Search: Key %d found at index %d\n", key, result);
    else
        printf("Recursive Search: Key %d not found\n", key);

    return 0;
}

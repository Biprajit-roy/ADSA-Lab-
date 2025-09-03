Q2. Binary vs Ternary Search:

Implemented a program to compare Binary Search and Ternary Search in terms of the number of comparisons made for different array sizes.
Used binary search function for Binary Search and ternary search function for Ternary Search. 

Binary search function divides the array into 2 halves at each step. Compares the key with the middle element, reduces the search space by half.(*comparisons)++ keeps track of the number of comparisons made. 

Ternary search function divides the array into 3 parts using two midpoints mid1 and mid2.Checks if the element is at mid1 or mid2If not found, searches in one of the three ranges. (*comparisons)++ counts how many times comparisons are made.

In the main function, defined a list of array sizes: {10, 100, 1000, 10000, 100000}.
For each size:
Generate a sorted array of numbers 1, 2, 3, ..., n.

Choose the middle element as the search key.

Run both Binary Search and Ternary Search on the array.

Count how many comparisons each method needed.

Print the results in a nice table format.


Both Binary Search and Ternary Search show logarithmic growth in the number of comparisons as the array size increases.

For every array size tested, Binary Search consistently required fewer comparisons than Ternary Search.

Although Ternary Search reduces the problem size to one-third at each step, it performs two comparisons per step, whereas Binary Search performs only one comparison per step.

As a result, the overhead of extra comparisons in Ternary Search makes it slower in practice than Binary Search.

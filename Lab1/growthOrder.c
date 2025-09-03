#include <stdio.h>
#include <math.h>
#include <string.h>

// Structure to store function name and value
typedef struct
{
    char name[20];
    double value;
} Func;

int main()
{
    // test values of n
    int n_values[] = {10, 50, 100, 500, 1000, 5000};
    int num_tests = sizeof(n_values) / sizeof(n_values[0]);

    for (int i = 0; i < num_tests; i++)
    {
        double n = n_values[i];

        // Array of functions
        Func funcs[12] = {
            {"1/n", 1.0 / n},
            {"log2(n)", log2(n)},
            {"12sqrt(n)", 12 * sqrt(n)},
            {"50sqrt(n)", 50 * sqrt(n)},
            {"n^0.51", pow(n, 0.51)},
            {"nlogn", n * log2(n)},
            {"100n^2+6n", 100 * n * n + 6 * n},
            {"n^2-324", n * n - 324},
            {"2n^3", 2 * pow(n, 3)},
            {"n^(logn)", pow(n, log2(n))},
            {"3^n", pow(3, n)},
            {"2^32*n", (double)(1ULL << 32) * n}};

        // Sort functions by value using bubble sort
        for (int j = 0; j < 12 - 1; j++)
        {
            for (int k = 0; k < 12 - j - 1; k++)
            {
                if (funcs[k].value > funcs[k + 1].value)
                {
                    Func temp = funcs[k];
                    funcs[k] = funcs[k + 1];
                    funcs[k + 1] = temp;
                }
            }
        }

        // Print results
        printf("\n=== For n = %.0f ===\n", n);
        printf("Increasing order of growth:\n");
        for (int j = 0; j < 12; j++)
        {
            printf("%-10s = %.6e\n", funcs[j].name, funcs[j].value);
        }
    }

    return 0;
}

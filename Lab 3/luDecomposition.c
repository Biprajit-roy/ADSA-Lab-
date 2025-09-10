#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 1e-9

void luDecomposition(int n, double A[n][n], double L[n][n], double U[n][n], int P[n])
{
    // Initialize permutation matrix P (as an array for row swaps)
    for (int i = 0; i < n; i++)
    {
        P[i] = i;
    }

    // Initialize L and U
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            L[i][j] = (i == j) ? 1 : 0; // L has 1s on diagonal
            U[i][j] = 0;
        }
    }

    // Gaussian elimination with partial pivoting
    for (int k = 0; k < n; k++)
    {
        // Find pivot (max element in column k, starting at row k)
        double maxVal = fabs(A[P[k]][k]);
        int maxRow = k;
        for (int i = k + 1; i < n; i++)
        {
            if (fabs(A[P[i]][k]) > maxVal)
            {
                maxVal = fabs(A[P[i]][k]);
                maxRow = i;
            }
        }

        // Swap rows in permutation matrix
        if (maxRow != k)
        {
            int temp = P[k];
            P[k] = P[maxRow];
            P[maxRow] = temp;
        }

        // Check for singular matrix
        if (fabs(A[P[k]][k]) < EPSILON)
        {
            printf("Matrix is singular or nearly singular!\n");
            exit(1);
        }

        // Elimination
        for (int i = k + 1; i < n; i++)
        {
            double multiplier = A[P[i]][k] / A[P[k]][k];
            L[P[i]][k] = multiplier;

            for (int j = k; j < n; j++)
            {
                A[P[i]][j] -= multiplier * A[P[k]][j];
            }
        }
    }

    // Build U from modified A
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            U[i][j] = A[P[i]][j];
        }
    }

    // Reorder L so it matches P
    double tempL[n][n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            tempL[i][j] = L[P[i]][j];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            L[i][j] = tempL[i][j];
}

void printMatrix(int n, double M[n][n])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%8.3f ", M[i][j]);
        printf("\n");
    }
}

int main()
{
    int n;
    printf("Enter the order of the square matrix: ");
    scanf("%d", &n);

    double A[n][n], L[n][n], U[n][n];
    int P[n];

    printf("Enter the elements of the matrix A (%dx%d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &A[i][j]);

    luDecomposition(n, A, L, U, P);

    printf("\nPermutation (row swaps) P:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", P[i] + 1); // +1 for 1-based row index
    printf("\n");

    printf("\nLower Triangular Matrix L:\n");
    printMatrix(n, L);

    printf("\nUpper Triangular Matrix U:\n");
    printMatrix(n, U);

    return 0;
}

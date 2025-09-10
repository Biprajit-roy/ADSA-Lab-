#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 1e-9

// Function to swap two rows in a matrix
void swapRows(double **M, int row1, int row2, int N)
{
    for (int i = 0; i < N; i++)
    {
        double temp = M[row1][i];
        M[row1][i] = M[row2][i];
        M[row2][i] = temp;
    }
}

// LU Decomposition with partial pivoting
int luDecomposition(double **A, double **L, double **U, int *P, int N)
{
    // Initialize permutation matrix P as identity
    for (int i = 0; i < N; i++)
        P[i] = i;

    for (int i = 0; i < N; i++)
    {
        // Partial pivoting: find the row with max pivot
        double maxVal = fabs(A[i][i]);
        int pivotRow = i;
        for (int k = i + 1; k < N; k++)
        {
            if (fabs(A[k][i]) > maxVal)
            {
                maxVal = fabs(A[k][i]);
                pivotRow = k;
            }
        }

        // Swap rows in A if needed
        if (pivotRow != i)
        {
            swapRows(A, i, pivotRow, N);
            int temp = P[i];
            P[i] = P[pivotRow];
            P[pivotRow] = temp;
        }

        // Check if pivot is still too small
        if (fabs(A[i][i]) < EPSILON)
        {
            printf("\nError: Matrix is singular or nearly singular.\n");
            return 0;
        }

        // Compute U
        for (int k = i; k < N; k++)
        {
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += (L[i][j] * U[j][k]);
            U[i][k] = A[i][k] - sum;
        }

        // Compute L
        for (int k = i; k < N; k++)
        {
            if (i == k)
                L[i][i] = 1;
            else
            {
                double sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (L[k][j] * U[j][i]);
                L[k][i] = (A[k][i] - sum) / U[i][i];
            }
        }
    }
    return 1;
}

// Forward substitution (Ly = Pb)
void forwardSubstitution(double **L, double *b, double *y, int *P, int N)
{
    for (int i = 0; i < N; i++)
    {
        double sum = 0;
        for (int j = 0; j < i; j++)
            sum += L[i][j] * y[j];
        y[i] = b[P[i]] - sum; // Use permuted b
    }
}

// Backward substitution (Ux = y)
void backwardSubstitution(double **U, double *y, double *x, int N)
{
    for (int i = N - 1; i >= 0; i--)
    {
        double sum = 0;
        for (int j = i + 1; j < N; j++)
            sum += U[i][j] * x[j];
        x[i] = (y[i] - sum) / U[i][i];
    }
}

int main()
{
    int N;
    printf("Enter the size of the matrix (N): ");
    scanf("%d", &N);

    double **A = (double **)malloc(N * sizeof(double *));
    double **L = (double **)malloc(N * sizeof(double *));
    double **U = (double **)malloc(N * sizeof(double *));
    double *b = (double *)malloc(N * sizeof(double));
    double *y = (double *)malloc(N * sizeof(double));
    double *x = (double *)malloc(N * sizeof(double));
    int *P = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        A[i] = (double *)malloc(N * sizeof(double));
        L[i] = (double *)calloc(N, sizeof(double));
        U[i] = (double *)calloc(N, sizeof(double));
    }

    printf("Enter the coefficients of matrix A (%dx%d):\n", N, N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%lf", &A[i][j]);

    printf("Enter the right-hand side vector b (%d values):\n", N);
    for (int i = 0; i < N; i++)
        scanf("%lf", &b[i]);

    // LU Decomposition with pivoting
    if (!luDecomposition(A, L, U, P, N))
    {
        // Free memory before exit
        for (int i = 0; i < N; i++)
        {
            free(A[i]);
            free(L[i]);
            free(U[i]);
        }
        free(A);
        free(L);
        free(U);
        free(b);
        free(y);
        free(x);
        free(P);
        return 1;
    }

    // Print L and U matrices
    printf("\nMatrix L (Lower Triangular):\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%8.4lf ", L[i][j]);
        printf("\n");
    }

    printf("\nMatrix U (Upper Triangular):\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%8.4lf ", U[i][j]);
        printf("\n");
    }

    printf("\nPermutation vector P:\n");
    for (int i = 0; i < N; i++)
        printf("%d ", P[i]);
    printf("\n");

    // Solve Ly = Pb (Forward Substitution)
    forwardSubstitution(L, b, y, P, N);

    // Solve Ux = y (Backward Substitution)
    backwardSubstitution(U, y, x, N);

    // Print solution
    printf("\nSolution vector x:\n");
    for (int i = 0; i < N; i++)
        printf("x[%d] = %lf\n", i, x[i]);

    // Free memory
    for (int i = 0; i < N; i++)
    {
        free(A[i]);
        free(L[i]);
        free(U[i]);
    }
    free(A);
    free(L);
    free(U);
    free(b);
    free(y);
    free(x);
    free(P);

    return 0;
}

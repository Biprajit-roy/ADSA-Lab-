#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to add two matrices
void addMatrix(int n, int A[][n], int B[][n], int C[][n])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

// Function to subtract two matrices
void subMatrix(int n, int A[][n], int B[][n], int C[][n])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Strassen algorithm
void strassen(int n, int A[][n], int B[][n], int C[][n])
{
    if (n == 1)
    {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    int A11[k][k], A12[k][k], A21[k][k], A22[k][k];
    int B11[k][k], B12[k][k], B21[k][k], B22[k][k];
    int C11[k][k], C12[k][k], C21[k][k], C22[k][k];
    int M1[k][k], M2[k][k], M3[k][k], M4[k][k], M5[k][k], M6[k][k], M7[k][k];
    int temp1[k][k], temp2[k][k];

    // Divide matrices
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // M1 = (A11 + A22) * (B11 + B22)
    addMatrix(k, A11, A22, temp1);
    addMatrix(k, B11, B22, temp2);
    strassen(k, temp1, temp2, M1);

    // M2 = (A21 + A22) * B11
    addMatrix(k, A21, A22, temp1);
    strassen(k, temp1, B11, M2);

    // M3 = A11 * (B12 - B22)
    subMatrix(k, B12, B22, temp1);
    strassen(k, A11, temp1, M3);

    // M4 = A22 * (B21 - B11)
    subMatrix(k, B21, B11, temp1);
    strassen(k, A22, temp1, M4);

    // M5 = (A11 + A12) * B22
    addMatrix(k, A11, A12, temp1);
    strassen(k, temp1, B22, M5);

    // M6 = (A21 - A11) * (B11 + B12)
    subMatrix(k, A21, A11, temp1);
    addMatrix(k, B11, B12, temp2);
    strassen(k, temp1, temp2, M6);

    // M7 = (A12 - A22) * (B21 + B22)
    subMatrix(k, A12, A22, temp1);
    addMatrix(k, B21, B22, temp2);
    strassen(k, temp1, temp2, M7);

    // C11 = M1 + M4 - M5 + M7
    addMatrix(k, M1, M4, temp1);
    subMatrix(k, temp1, M5, temp2);
    addMatrix(k, temp2, M7, C11);

    // C12 = M3 + M5
    addMatrix(k, M3, M5, C12);

    // C21 = M2 + M4
    addMatrix(k, M2, M4, C21);

    // C22 = M1 - M2 + M3 + M6
    subMatrix(k, M1, M2, temp1);
    addMatrix(k, temp1, M3, temp2);
    addMatrix(k, temp2, M6, C22);

    // Combine results
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }
}

// Find next power of 2
int nextPowerOf2(int n)
{
    int p = 1;
    while (p < n)
        p *= 2;
    return p;
}

int main()
{
    int n;
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);

    int np = nextPowerOf2(n);

    int A[np][np], B[np][np], C[np][np];

    // Initialize matrices with 0
    for (int i = 0; i < np; i++)
        for (int j = 0; j < np; j++)
        {
            A[i][j] = 0;
            B[i][j] = 0;
            C[i][j] = 0;
        }

    printf("Enter elements of Matrix A (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of Matrix B (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &B[i][j]);

    strassen(np, A, B, C);

    printf("Resultant Matrix after Strassen Multiplication:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    return 0;
}

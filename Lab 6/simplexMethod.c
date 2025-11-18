#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXM 10 // Maximum number of constraints
#define MAXN 10 // Maximum number of variables

// Function prototypes
void print_tableau(double tableau[MAXM + 1][MAXN + MAXM + 2], int m, int n, int basis[MAXM]);
int find_entering_variable(double tableau[MAXM + 1][MAXN + MAXM + 2], int n, int m);
int find_leaving_variable(double tableau[MAXM + 1][MAXN + MAXM + 2], int m, int n, int pivot_col);
void pivot_operation(double tableau[MAXM + 1][MAXN + MAXM + 2], int m, int n, int pivot_row, int pivot_col, int basis[MAXM]);

int main()
{
    int m, n;
    double tableau[MAXM + 1][MAXN + MAXM + 2]; // simplex tableau
    int basis[MAXM];                           // store basic variable indices

    printf("Enter number of variables: ");
    scanf("%d", &n);
    printf("Enter number of constraints: ");
    scanf("%d", &m);

    printf("\nEnter coefficients of constraints (A matrix):\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%lf", &tableau[i][j]);
        }
    }

    printf("\nEnter RHS values (b vector):\n");
    for (int i = 0; i < m; i++)
    {
        scanf("%lf", &tableau[i][n + m + 1 - 1]); // last column is RHS
    }

    printf("\nEnter coefficients of objective function (Z = c1*x1 + c2*x2 + ...):\n");
    for (int j = 0; j < n; j++)
    {
        scanf("%lf", &tableau[m][j]);
        tableau[m][j] *= -1; // Convert to standard form (Z-row negative)
    }

    // Add slack variables
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            tableau[i][n + j] = (i == j) ? 1 : 0;
        }
        basis[i] = n + i; // slack variable index
    }

    // Set the rest of the tableau to 0 in Z-row
    for (int j = n; j < n + m; j++)
        tableau[m][j] = 0;
    tableau[m][n + m + 1 - 1] = 0;

    printf("\nInitial Simplex Tableau:\n");
    print_tableau(tableau, m, n, basis);

    while (1)
    {
        int pivot_col = find_entering_variable(tableau, n, m);
        if (pivot_col == -1)
        {
            printf("\nOptimal solution found!\n");
            break;
        }

        int pivot_row = find_leaving_variable(tableau, m, n, pivot_col);
        if (pivot_row == -1)
        {
            printf("\nUnbounded solution detected!\n");
            return 0;
        }

        pivot_operation(tableau, m, n, pivot_row, pivot_col, basis);
        print_tableau(tableau, m, n, basis);
    }

    printf("\nOptimal Solution:\n");
    for (int i = 0; i < m; i++)
    {
        printf("x%d = %.3lf\n", basis[i] + 1, tableau[i][n + m + 1 - 1]);
    }

    printf("Zmax = %.3lf\n", tableau[m][n + m + 1 - 1]);

    return 0;
}

// ---------------- Helper Functions ----------------

void print_tableau(double tableau[MAXM + 1][MAXN + MAXM + 2], int m, int n, int basis[MAXM])
{
    printf("\nCurrent Simplex Tableau:\n");
    printf("-----------------------------------------------------\n");
    printf("Basis |");
    for (int j = 0; j < n + m; j++)
        printf(" x%-3d", j + 1);
    printf(" |  RHS\n");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < m; i++)
    {
        printf("x%-4d|", basis[i] + 1);
        for (int j = 0; j < n + m; j++)
            printf("%5.2lf", tableau[i][j]);
        printf(" | %5.2lf\n", tableau[i][n + m + 1 - 1]);
    }

    printf("-----------------------------------------------------\nZ     |");
    for (int j = 0; j < n + m; j++)
        printf("%5.2lf", tableau[m][j]);
    printf(" | %5.2lf\n", tableau[m][n + m + 1 - 1]);
    printf("-----------------------------------------------------\n");
}

int find_entering_variable(double tableau[MAXM + 1][MAXN + MAXM + 2], int n, int m)
{
    int pivot_col = -1;
    double min = 0;
    for (int j = 0; j < n + m; j++)
    {
        if (tableau[m][j] < min)
        {
            min = tableau[m][j];
            pivot_col = j;
        }
    }
    return pivot_col;
}

int find_leaving_variable(double tableau[MAXM + 1][MAXN + MAXM + 2], int m, int n, int pivot_col)
{
    double min_ratio = 1e9;
    int pivot_row = -1;
    for (int i = 0; i < m; i++)
    {
        double aij = tableau[i][pivot_col];
        double bi = tableau[i][n + m + 1 - 1];
        if (aij > 0)
        {
            double ratio = bi / aij;
            if (ratio < min_ratio)
            {
                min_ratio = ratio;
                pivot_row = i;
            }
        }
    }
    return pivot_row;
}

void pivot_operation(double tableau[MAXM + 1][MAXN + MAXM + 2], int m, int n, int pivot_row, int pivot_col, int basis[MAXM])
{
    double pivot = tableau[pivot_row][pivot_col];

    // Normalize pivot row
    for (int j = 0; j <= n + m; j++)
        tableau[pivot_row][j] /= pivot;

    // Update RHS
    tableau[pivot_row][n + m + 1 - 1] /= pivot;

    // Eliminate other rows
    for (int i = 0; i <= m; i++)
    {
        if (i == pivot_row)
            continue;
        double factor = tableau[i][pivot_col];
        for (int j = 0; j <= n + m; j++)
            tableau[i][j] -= factor * tableau[pivot_row][j];
        tableau[i][n + m + 1 - 1] -= factor * tableau[pivot_row][n + m + 1 - 1];
    }

    basis[pivot_row] = pivot_col; // update basic variable
}

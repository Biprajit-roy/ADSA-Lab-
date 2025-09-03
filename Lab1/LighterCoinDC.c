#include <stdio.h>

int weigh(int coins[], int start1, int end1, int start2, int end2)
{

    int sum1 = 0, sum2 = 0;

    for (int i = start1; i <= end1; i++)
        sum1 += coins[i];
    for (int i = start2; i <= end2; i++)
        sum2 += coins[i];

    if (sum1 < sum2)
        return -1;
    if (sum1 > sum2)
        return 1;

    return 0;
}

int findDefective(int coins[], int start, int end)
{

    int n = end - start + 1;

    if (n == 1)
        return start;

    int mid = start + (n / 2) - 1;
    int leftStart = start, leftEnd = mid;
    int rightStart = mid + 1, rightEnd = end;

    if (n % 2 == 0)
    {
        int result = weigh(coins, leftStart, leftEnd, rightStart, rightEnd);
        if (result == -1)
            return findDefective(coins, leftStart, leftEnd);
        else if (result == 1)
            return findDefective(coins, rightStart, rightEnd);
        else
            return -1;
    }

    else
    {
        int extra = end;
        int result = weigh(coins, leftStart, leftEnd, rightStart, rightEnd - 1);

        if (result == -1)
            return findDefective(coins, leftStart, leftEnd);
        else if (result == 1)
            return findDefective(coins, rightStart, rightEnd - 1);
        else
        {
            if (coins[extra] < coins[start])
                return extra;
            else
                return -1;
        }
    }
}

int main()
{
    int n;
    printf("Enter number of coins: ");
    scanf("%d", &n);

    int coins[n];

    printf("Enter weights of %d coins:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &coins[i]);

    int defectiveIndex = findDefective(coins, 0, n - 1);

    if (defectiveIndex == -1)
        printf("No defective coin found. All coins are of equal weight.\n");
    else
        printf("Defective (lighter) coin found at index %d (0-based).\n", defectiveIndex);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct FibNode
{
    int key;
    int degree;
    int mark;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
} FibNode;

typedef struct FibHeap
{
    int n;
    FibNode *min;
} FibHeap;

// ---------- Utility Functions ----------
FibNode *create_node(int key)
{
    FibNode *node = (FibNode *)malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->mark = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    return node;
}

FibHeap *make_fib_heap()
{
    FibHeap *H = (FibHeap *)malloc(sizeof(FibHeap));
    H->n = 0;
    H->min = NULL;
    return H;
}

// ---------- Insert ----------
void fib_heap_insert(FibHeap *H, int key)
{
    FibNode *x = create_node(key);

    if (H->min == NULL)
    {
        H->min = x;
    }
    else
    {
        x->right = H->min->right;
        x->left = H->min;
        H->min->right->left = x;
        H->min->right = x;

        if (x->key < H->min->key)
            H->min = x;
    }
    H->n++;
}

// ---------- Link Two Trees ----------
void fib_heap_link(FibHeap *H, FibNode *y, FibNode *x)
{
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;

    if (x->child == NULL)
    {
        x->child = y;
        y->left = y->right = y;
    }
    else
    {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = 0;
}

// ---------- Consolidation ----------
void fib_heap_consolidate(FibHeap *H)
{
    int D = 50;
    FibNode *A[50] = {NULL};

    FibNode *start = H->min;
    if (start == NULL)
        return;

    FibNode *w = start;
    int roots = 0;

    do
    {
        roots++;
        w = w->right;
    } while (w != start);

    w = start;
    while (roots--)
    {
        FibNode *x = w;
        int d = x->degree;
        w = w->right;

        while (A[d] != NULL)
        {
            FibNode *y = A[d];
            if (x->key > y->key)
            {
                FibNode *t = x;
                x = y;
                y = t;
            }
            fib_heap_link(H, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }

    H->min = NULL;
    for (int i = 0; i < 50; i++)
    {
        if (A[i] != NULL)
        {
            if (H->min == NULL)
            {
                H->min = A[i];
                H->min->left = H->min->right = H->min;
            }
            else
            {
                A[i]->left = H->min;
                A[i]->right = H->min->right;
                H->min->right->left = A[i];
                H->min->right = A[i];

                if (A[i]->key < H->min->key)
                    H->min = A[i];
            }
        }
    }
}

// ---------- Extract Minimum ----------
FibNode *fib_heap_extract_min(FibHeap *H)
{
    FibNode *z = H->min;
    if (z != NULL)
    {
        if (z->child != NULL)
        {
            FibNode *x = z->child;
            do
            {
                x->parent = NULL;
                x = x->right;
            } while (x != z->child);

            FibNode *zleft = z->left;
            FibNode *childleft = z->child->left;

            zleft->right = z->child;
            z->child->left = zleft;

            childleft->right = z->right;
            z->right->left = childleft;
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
        {
            H->min = NULL;
        }
        else
        {
            H->min = z->right;
            fib_heap_consolidate(H);
        }
        H->n--;
    }
    return z;
}

// ---------- Decrease Key ----------
void fib_heap_cut(FibHeap *H, FibNode *x, FibNode *y)
{
    if (x->right == x)
        y->child = NULL;
    else
    {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x)
            y->child = x->right;
    }

    y->degree--;

    x->left = H->min;
    x->right = H->min->right;
    H->min->right->left = x;
    H->min->right = x;

    x->parent = NULL;
    x->mark = 0;
}

void fib_heap_cascading_cut(FibHeap *H, FibNode *y)
{
    FibNode *z = y->parent;
    if (z != NULL)
    {
        if (y->mark == 0)
            y->mark = 1;
        else
        {
            fib_heap_cut(H, y, z);
            fib_heap_cascading_cut(H, z);
        }
    }
}

void fib_heap_decrease_key(FibHeap *H, FibNode *x, int k)
{
    if (k > x->key)
    {
        printf("New key is greater than current key!\n");
        return;
    }

    x->key = k;

    FibNode *y = x->parent;
    if (y != NULL && x->key < y->key)
    {
        fib_heap_cut(H, x, y);
        fib_heap_cascading_cut(H, y);
    }

    if (x->key < H->min->key)
        H->min = x;
}

// ---------- Delete ----------
void fib_heap_delete(FibHeap *H, FibNode *x)
{
    fib_heap_decrease_key(H, x, INT_MIN);
    fib_heap_extract_min(H);
}

// ---------- Display (root list only) ----------
void display(FibHeap *H)
{
    if (H->min == NULL)
    {
        printf("Heap is empty.\n");
        return;
    }

    printf("Root list: ");
    FibNode *x = H->min;
    do
    {
        printf("%d ", x->key);
        x = x->right;
    } while (x != H->min);
    printf("\n");
}

// ---------- Menu ----------
int main()
{
    FibHeap *H = make_fib_heap();
    int choice, key, new_key;

    while (1)
    {
        printf("\n---- Fibonacci Heap Menu ----\n");
        printf("1. Insert\n");
        printf("2. Get Minimum\n");
        printf("3. Extract Minimum\n");
        printf("4. Decrease Key\n");
        printf("5. Display Root List\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter key to insert: ");
            scanf("%d", &key);
            fib_heap_insert(H, key);
            break;

        case 2:
            if (H->min)
                printf("Minimum = %d\n", H->min->key);
            else
                printf("Heap is empty!\n");
            break;

        case 3:
        {
            FibNode *m = fib_heap_extract_min(H);
            if (m)
                printf("Extracted Min = %d\n", m->key);
            else
                printf("Heap is empty!\n");
            break;
        }

        case 4:
            printf("Enter current key: ");
            scanf("%d", &key);

            printf("Enter new smaller key: ");
            scanf("%d", &new_key);

            // Search for node in root list (simple linear search demo)
            FibNode *x = H->min;

            if (x == NULL)
            {
                printf("Heap empty.\n");
                break;
            }

            int found = 0;
            do
            {
                if (x->key == key)
                {
                    found = 1;
                    break;
                }
                x = x->right;
            } while (x != H->min);

            if (!found)
            {
                printf("Key not found (search limited to root list).\n");
                break;
            }

            fib_heap_decrease_key(H, x, new_key);
            printf("Key decreased.\n");
            break;

        case 5:
            display(H);
            break;

        case 6:
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }
}

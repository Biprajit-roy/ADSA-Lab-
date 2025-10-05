#include <stdio.h>
#include <stdlib.h>

#define MIN_DEGREE 2

typedef struct BTreeNode
{
    int *keys;
    int t;
    struct BTreeNode **C;
    int n;
    int leaf;
} BTreeNode;

// Function prototypes
BTreeNode *createNode(int t, int leaf);
void traverse(BTreeNode *root);
void printTree(BTreeNode *root, int level);
BTreeNode *insert(BTreeNode *root, int k);
void insertNonFull(BTreeNode *node, int k);
void splitChild(BTreeNode *parent, int i, BTreeNode *child);
BTreeNode *deleteNode(BTreeNode *root, int k);
int findKey(BTreeNode *node, int k);
void removeFromLeaf(BTreeNode *node, int idx);
void removeFromNonLeaf(BTreeNode *node, int idx);
int getPredecessor(BTreeNode *node, int idx);
int getSuccessor(BTreeNode *node, int idx);
void fill(BTreeNode *node, int idx);
void borrowFromPrev(BTreeNode *node, int idx);
void borrowFromNext(BTreeNode *node, int idx);
void merge(BTreeNode *node, int idx);

// Create a new B-Tree node
BTreeNode *createNode(int t, int leaf)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (int *)malloc(sizeof(int) * (2 * t - 1));
    node->C = (BTreeNode **)malloc(sizeof(BTreeNode *) * (2 * t));
    node->n = 0;
    return node;
}

// Traverse tree in-order
void traverse(BTreeNode *root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->n; i++)
        {
            if (!root->leaf)
                traverse(root->C[i]);
            printf("%d ", root->keys[i]);
        }
        if (!root->leaf)
            traverse(root->C[i]);
    }
}

// Print tree visually
void printTree(BTreeNode *root, int level)
{
    if (root != NULL)
    {
        printf("Level %d : ", level);
        for (int i = 0; i < root->n; i++)
            printf("%d ", root->keys[i]);
        printf("\n");
        if (!root->leaf)
        {
            for (int i = 0; i <= root->n; i++)
                printTree(root->C[i], level + 1);
        }
    }
}

// Insert key
BTreeNode *insert(BTreeNode *root, int k)
{
    if (root == NULL)
    {
        root = createNode(MIN_DEGREE, 1);
        root->keys[0] = k;
        root->n = 1;
        return root;
    }
    if (root->n == 2 * MIN_DEGREE - 1)
    {
        BTreeNode *s = createNode(MIN_DEGREE, 0);
        s->C[0] = root;
        splitChild(s, 0, root);
        int i = 0;
        if (s->keys[0] < k)
            i++;
        insertNonFull(s->C[i], k);
        return s;
    }
    else
    {
        insertNonFull(root, k);
        return root;
    }
}

void insertNonFull(BTreeNode *node, int k)
{
    int i = node->n - 1;
    if (node->leaf)
    {
        while (i >= 0 && node->keys[i] > k)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = k;
        node->n++;
    }
    else
    {
        while (i >= 0 && node->keys[i] > k)
            i--;
        i++;
        if (node->C[i]->n == 2 * MIN_DEGREE - 1)
        {
            splitChild(node, i, node->C[i]);
            if (node->keys[i] < k)
                i++;
        }
        insertNonFull(node->C[i], k);
    }
}

void splitChild(BTreeNode *parent, int i, BTreeNode *y)
{
    BTreeNode *z = createNode(y->t, y->leaf);
    z->n = MIN_DEGREE - 1;
    for (int j = 0; j < MIN_DEGREE - 1; j++)
        z->keys[j] = y->keys[j + MIN_DEGREE];
    if (!y->leaf)
    {
        for (int j = 0; j < MIN_DEGREE; j++)
            z->C[j] = y->C[j + MIN_DEGREE];
    }
    y->n = MIN_DEGREE - 1;
    for (int j = parent->n; j >= i + 1; j--)
        parent->C[j + 1] = parent->C[j];
    parent->C[i + 1] = z;
    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[i] = y->keys[MIN_DEGREE - 1];
    parent->n++;
}

// --- Delete functions ---
int findKey(BTreeNode *node, int k)
{
    int idx = 0;
    while (idx < node->n && node->keys[idx] < k)
        idx++;
    return idx;
}

BTreeNode *deleteNode(BTreeNode *root, int k)
{
    if (!root)
        return root;
    int idx = findKey(root, k);

    if (idx < root->n && root->keys[idx] == k)
    {
        if (root->leaf)
            removeFromLeaf(root, idx);
        else
            removeFromNonLeaf(root, idx);
    }
    else
    {
        if (root->leaf)
        {
            printf("Key %d not found\n", k);
            return root;
        }
        int flag = (idx == root->n) ? 1 : 0;
        if (root->C[idx]->n < MIN_DEGREE)
            fill(root, idx);
        if (flag && idx > root->n)
            root->C[idx - 1] = deleteNode(root->C[idx - 1], k);
        else
            root->C[idx] = deleteNode(root->C[idx], k);
    }
    if (root->n == 0)
    {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];
        free(tmp->keys);
        free(tmp->C);
        free(tmp);
    }
    return root;
}

void removeFromLeaf(BTreeNode *node, int idx)
{
    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];
    node->n--;
}

void removeFromNonLeaf(BTreeNode *node, int idx)
{
    int k = node->keys[idx];
    if (node->C[idx]->n >= MIN_DEGREE)
    {
        int pred = getPredecessor(node, idx);
        node->keys[idx] = pred;
        node->C[idx] = deleteNode(node->C[idx], pred);
    }
    else if (node->C[idx + 1]->n >= MIN_DEGREE)
    {
        int succ = getSuccessor(node, idx);
        node->keys[idx] = succ;
        node->C[idx + 1] = deleteNode(node->C[idx + 1], succ);
    }
    else
    {
        merge(node, idx);
        node->C[idx] = deleteNode(node->C[idx], k);
    }
}

int getPredecessor(BTreeNode *node, int idx)
{
    BTreeNode *cur = node->C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

int getSuccessor(BTreeNode *node, int idx)
{
    BTreeNode *cur = node->C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

void fill(BTreeNode *node, int idx)
{
    if (idx != 0 && node->C[idx - 1]->n >= MIN_DEGREE)
        borrowFromPrev(node, idx);
    else if (idx != node->n && node->C[idx + 1]->n >= MIN_DEGREE)
        borrowFromNext(node, idx);
    else
    {
        if (idx != node->n)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

void borrowFromPrev(BTreeNode *node, int idx)
{
    BTreeNode *child = node->C[idx];
    BTreeNode *sibling = node->C[idx - 1];
    for (int i = child->n - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];
    if (!child->leaf)
        for (int i = child->n; i >= 0; i--)
            child->C[i + 1] = child->C[i];
    child->keys[0] = node->keys[idx - 1];
    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];
    node->keys[idx - 1] = sibling->keys[sibling->n - 1];
    child->n += 1;
    sibling->n -= 1;
}

void borrowFromNext(BTreeNode *node, int idx)
{
    BTreeNode *child = node->C[idx];
    BTreeNode *sibling = node->C[idx + 1];
    child->keys[child->n] = node->keys[idx];
    if (!child->leaf)
        child->C[child->n + 1] = sibling->C[0];
    node->keys[idx] = sibling->keys[0];
    for (int i = 1; i < sibling->n; i++)
        sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->leaf)
        for (int i = 1; i <= sibling->n; i++)
            sibling->C[i - 1] = sibling->C[i];
    child->n += 1;
    sibling->n -= 1;
}

void merge(BTreeNode *node, int idx)
{
    BTreeNode *child = node->C[idx];
    BTreeNode *sibling = node->C[idx + 1];
    child->keys[MIN_DEGREE - 1] = node->keys[idx];
    for (int i = 0; i < sibling->n; i++)
        child->keys[i + MIN_DEGREE] = sibling->keys[i];
    if (!child->leaf)
        for (int i = 0; i <= sibling->n; i++)
            child->C[i + MIN_DEGREE] = sibling->C[i];
    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];
    for (int i = idx + 2; i <= node->n; i++)
        node->C[i - 1] = node->C[i];
    child->n += sibling->n + 1;
    node->n--;
    free(sibling->keys);
    free(sibling->C);
    free(sibling);
}

// Main
int main()
{
    BTreeNode *root = NULL;
    int choice, key;

    while (1)
    {
        printf("\n1.Insert 2.Delete 3.Traverse 4.Print Tree 5.Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter key to insert: ");
            scanf("%d", &key);
            root = insert(root, key);
            break;
        case 2:
            printf("Enter key to delete: ");
            scanf("%d", &key);
            root = deleteNode(root, key);
            break;
        case 3:
            printf("Inorder traversal: ");
            traverse(root);
            printf("\n");
            break;
        case 4:
            printf("B-Tree structure:\n");
            printTree(root, 0);
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid choice\n");
        }
    }
}

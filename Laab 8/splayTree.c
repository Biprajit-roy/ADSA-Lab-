#include <stdio.h>
#include <stdlib.h>

// ---------------------- Node Structure ----------------------

typedef struct Node
{
    int key;
    struct Node *left, *right;
} Node;

// ---------------------- Utility: Create Node ----------------------

Node *createNode(int key)
{
    Node *node = (Node *)malloc(sizeof(Node)); // FIXED
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

// ---------------------- Right Rotate ----------------------

Node *rightRotate(Node *x)
{
    Node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// ---------------------- Left Rotate ----------------------

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// ---------------------- Splay Operation ----------------------

Node *splay(Node *root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    // Key lies in left subtree
    if (key < root->key)
    {
        if (root->left == NULL)
            return root;

        // Zig-Zig (Left Left)
        if (key < root->left->key)
        {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key)
        {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }
        return (root->left == NULL) ? root : rightRotate(root);
    }

    // Key lies in right subtree
    else
    {
        if (root->right == NULL)
            return root;

        // Zig-Zig (Right Right)
        if (key > root->right->key)
        {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }
        // Zig-Zag (Right Left)
        else if (key < root->right->key)
        {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        return (root->right == NULL) ? root : leftRotate(root);
    }
}

// ---------------------- Insert ----------------------

Node *insert(Node *root, int key)
{
    if (root == NULL)
        return createNode(key);

    root = splay(root, key);

    if (root->key == key)
        return root; // No duplicates

    Node *newNode = createNode(key);

    if (key < root->key)
    {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    }
    else
    {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    return newNode;
}

// ---------------------- Search ----------------------

Node *search(Node *root, int key)
{
    return splay(root, key);
}

// ---------------------- Delete ----------------------

Node *delete(Node *root, int key)
{
    if (root == NULL)
        return NULL;

    root = splay(root, key);

    if (root->key != key)
        return root; // Key not found

    Node *temp;
    if (root->left == NULL)
    {
        temp = root->right;
    }
    else
    {
        temp = splay(root->left, key);
        temp->right = root->right;
    }

    free(root);
    return temp;
}

// ---------------------- Inorder Traversal ----------------------

void inorder(Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

// ---------------------- MAIN (Menu-Driven) ----------------------

int main()
{
    Node *root = NULL;
    int choice, key;

    while (1)
    {
        printf("\n--- Splay Tree Menu ---\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Inorder Traversal\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter key to insert: ");
            scanf("%d", &key);
            root = insert(root, key);
            break;

        case 2:
            printf("Enter key to search: ");
            scanf("%d", &key);
            root = search(root, key);
            if (root && root->key == key)
                printf("Found and splayed to root.\n");
            else
                printf("Key not found.\n");
            break;

        case 3:
            printf("Enter key to delete: ");
            scanf("%d", &key);
            root = delete(root, key);
            printf("Deleted if it existed.\n");
            break;

        case 4:
            printf("Inorder: ");
            inorder(root);
            printf("\n");
            break;

        case 5:
            exit(0);

        default:
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

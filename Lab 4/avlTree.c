#include <stdio.h>
#include <stdlib.h>

// AVL Tree Node
typedef struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

// Function to get the height of a node
int height(Node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to get maximum of two integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Create a new node
Node *createNode(int key)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1; // New node is initially added at leaf
    return node;
}

// Right rotate
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    // Rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x; // New root
}

// Left rotate
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // Rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y; // New root
}

// Get balance factor of a node
int getBalance(Node *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Insert a node into AVL tree
Node *insert(Node *node, int key)
{
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Duplicate keys not allowed

    // Update height
    node->height = 1 + max(height(node->left), height(node->right));

    // Get balance factor
    int balance = getBalance(node);

    // Balance the tree
    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Find node with minimum key
Node *minValueNode(Node *node)
{
    Node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Delete a node from AVL tree
Node *deleteNode(Node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else
    {
        // Node with one or no child
        if ((root->left == NULL) || (root->right == NULL))
        {
            Node *temp = root->left ? root->left : root->right;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp; // Copy the contents
            free(temp);
        }
        else
        {
            // Node with two children
            Node *temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    // Update height
    root->height = 1 + max(height(root->left), height(root->right));

    // Balance the tree
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Inorder traversal
void inorder(Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

// Main function
int main()
{
    Node *root = NULL;
    int choice, key;

    while (1)
    {
        printf("\n1. Insert\n2. Delete\n3. Display Inorder\n4. Exit\n");
        printf("Enter your choice: ");
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
            inorder(root);
            printf("\n");
            break;
        case 4:
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

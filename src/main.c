#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct node
{
    char *word;
    struct node *leftchilde, *rightchilde;
} node;
node *predecessor;
node *successor;
int iffind = 0;
int count = 0;
node *newnode(char *word)
{
    node *temp = (node *)malloc(sizeof(node));
    temp->word = (char *)malloc(sizeof(char) * strlen(word));
    strcpy(temp->word, word);
    temp->leftchilde = temp->rightchilde = NULL;
    return temp;
}
int getsize(node *root)
{
    if (root == NULL)
        return 0;
    else
        return (getsize(root->leftchilde) + 1 + getsize(root->rightchilde));
}
int getheight(node *root)
{
    if (root == NULL)
        return 0;
    else
    {
        int lheight = getheight(root->leftchilde);
        int rheight = getheight(root->rightchilde);
        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}
node *insert(node *root, char *word)
{
    if (root == NULL)
        return newnode(word);
    if (strcasecmp(word, root->word) < 0)
        root->leftchilde = insert(root->leftchilde, word);
    else if (strcasecmp(word, root->word) > 0)
        root->rightchilde = insert(root->rightchilde, word);
    return root;
}

node *search(node *root, char *word)
{
    node *temp = root;
    while (temp != NULL)
    {
        if (strcasecmp(word, temp->word) == 0)
        {
            iffind = 1;
            return temp;
        }
        else if (strcasecmp(word, temp->word) < 0)
        {
            if (temp->leftchilde != NULL)
            {
                temp = temp->leftchilde;
            }
            else
                return temp;
        }
        else if (strcasecmp(word, temp->word) > 0)
        {
            if (temp->rightchilde != NULL)
            {
                temp = temp->rightchilde;
            }
            else
                return temp;
        }
    }
    return temp;
}
void findpreandsuc(node *root, node *leaf)
{
    if (iffind == 0)
    {
        if (root == leaf)
        {
            iffind = 1;
        }
        else if (root != NULL)
        {
            predecessor = root;
        }
    }
    else if (iffind == 1)
    {
        if (root != NULL)
        {
            successor = root;
            iffind = 2;
        }
    }
    if (root)
    {
        findpreandsuc(root->leftchilde, leaf);
        if (iffind == 0)
            predecessor = root;
        if (iffind == 1)
        {
            if (root != leaf)
            {
                successor = root;
                iffind = 2;
            }
        }
        findpreandsuc(root->rightchilde, leaf);
    }
    if (predecessor == successor)
    {
        predecessor = NULL;
    }
}
void print_inorder(node *root)
{
    if (root)
    {
        print_inorder(root->leftchilde);
        printf("%s\t", root->word);
        print_inorder(root->rightchilde);
    }
}
int main()
{
    node *root = NULL;
    FILE *fp;
    fp = fopen("EN-US-Dictionary.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    else
    {
        printf("File opened successfully\n");
    }
    printf("--------------------------\n");
    while (!feof(fp))
    {
        char *word = (char *)malloc(sizeof(char) * 50);
        fscanf(fp, "%s", word);
        root = insert(root, word);
    }
    fclose(fp);
    printf("Size = %d\n", getsize(root));
    printf("--------------------------\n");
    printf("Height = %d\n", getheight(root));
    printf("--------------------------\n");
    char string[100];
    printf("Enter the string to be searched\n");
    gets(string);
    char buffer[100];
    int i = 0;
    int j = 0;
    while (string[i] != '\0')
    {
        j = 0;
        while (string[i] != ' ' && string[i] != '\0')
        {
            buffer[j] = string[i];
            i++;
            j++;
        }
        i++;
        while (j < i)
            buffer[j++] = '\0';
        node *leaf = search(root, buffer);
        if (iffind == 1)
        {
            printf("%s-The word is correct\n", buffer);
            iffind = 0;
        }
        else
        {
            printf("%s-The word is incorrect suggestions are:", buffer);
            findpreandsuc(root, leaf);
            printf("%s, ", leaf->word);
            if (predecessor != NULL)
            {
                printf("%s, ", predecessor->word);
            }
            if (successor != NULL)
            {
                printf("%s\n", successor->word);
            }
            iffind = 0;
            predecessor = NULL;
            successor = NULL;
        }
    }
}

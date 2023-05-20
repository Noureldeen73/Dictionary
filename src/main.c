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
    if (strcasecmp(word, root->word) > 0)
        root->rightchilde = insert(root->rightchilde, word);
    else if (strcasecmp(word, root->word) < 0)
        root->leftchilde = insert(root->leftchilde, word);
    return root;
}

node *search(node *root, char *word)
{
    node *temp = root;
    while (temp != NULL)
    {
        if (strcasecmp(word, temp->word) == 0)
        {
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
    if (root == NULL)
        return;
    if (strcasecmp(leaf->word, root->word) < 0)
    {
        successor = root;
        findpreandsuc(root->leftchilde, leaf);
    }
    else if (strcasecmp(leaf->word, root->word) > 0)
    {
        predecessor = root;
        findpreandsuc(root->rightchilde, leaf);
    }
    else
    {
        if (root->leftchilde != NULL)
        {
            node *temp = root->leftchilde;
            while (temp->rightchilde)
            {
                temp = temp->rightchilde;
            }
            predecessor = temp;
        }
        if (root->rightchilde != NULL)
        {
            node *temp = root->rightchilde;
            while (temp->leftchilde)
            {
                temp = temp->leftchilde;
            }
            successor = temp;
        }
    }
}
void print_inorder(node *root, FILE *fp1)
{
    if (root)
    {
        print_inorder(root->leftchilde, fp1);
        fprintf(fp1, "%s\n", root->word);
        print_inorder(root->rightchilde, fp1);
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
    while (string[i] != '\0' && string[i] != '\n')
    {
        j = 0;
        if (string[i] == ' ')
        {
            i++;
            continue;
        }
        while (string[i] != ' ' && string[i] != '\0' && string[i] != '\n')
        {
            buffer[j] = string[i];
            i++;
            j++;
        }
        buffer[j++] = '\0';
        node *leaf = search(root, buffer);
        if (strcasecmp(leaf->word, buffer) == 0)
        {
            printf("%s-The word is correct\n", buffer);
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
            predecessor = NULL;
            successor = NULL;
        }
    }
}

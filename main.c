//
// Created by gauss on 10/07/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define TAM 256
#define MAX_FREQUENCY UINT_MAX

typedef struct node
{
    struct node *next;
    struct node *prev;
    struct node *left;
    struct node *right;
    unsigned char character;
    unsigned int frequency;
} node_t;

typedef struct frequency_list
{
    struct node *head;
    struct node *tail;
    int size;
} list_t;

typedef struct frequency_tree
{
    struct node *root;
} huffmen_tree_t;

unsigned int *alloc_frequency_table();
void initialize_frequency_table(unsigned int *frequency_table);
void fill_frequency_table(unsigned int *frequency_table, const char *file_name);
void display_frequency_table(unsigned int *frequency_table);
void destroy_frequency_table(unsigned int *frequency_table);

list_t *alloc_frequency_list();
void insert_frequency_list(list_t *frequency_list, node_t *new_node);
node_t *remove_frequency_list(list_t *frequency_list);
void fill_frequency_list(list_t *frequency_list, const unsigned int *frequency_table);
void destroy_frequency_list(list_t *frequency_list);
void display_frequency_list(list_t *frequency_list);

huffmen_tree_t *alloc_huffmen_tree_t();
node_t *huffmen_tree_build(list_t *frequency_list);
void display_huffmen_tree(node_t *root);
int tree_height(node_t *root, int height);
void destroy_huffmen_tree(node_t *root);

char ** alloc_dictionary(int height);
void build_dictionary(char **dictionary, node_t *root, char *str);
void display_dictionary(char **dictionary);
void destroy_dictionary(char **dictionary);

int main()
{
    unsigned int *frequency_table;
    list_t *frequency_list;
    huffmen_tree_t *huffmen_tree;
    char ** dictionary;
    char *file_name;

    file_name = calloc(256, sizeof(char));
    file_name = "Teste.txt";
    //-------------------------- Frequency Table
    frequency_table = alloc_frequency_table();
    initialize_frequency_table(frequency_table);
    fill_frequency_table(frequency_table, file_name);
    display_frequency_table(frequency_table);

    //-------------------------- Frequency List
    frequency_list = alloc_frequency_list();
    fill_frequency_list(frequency_list, frequency_table);
    display_frequency_list(frequency_list);

    //-------------------------- Huffmen Tree
    huffmen_tree = alloc_huffmen_tree_t();
    printf("\nHuffmen Tree:\n");
    huffmen_tree->root = huffmen_tree_build(frequency_list);
    display_huffmen_tree(huffmen_tree->root);

    //-------------------------- Dictionary
    int h = tree_height(huffmen_tree->root, 0);
    dictionary = alloc_dictionary(h);
    build_dictionary(dictionary, huffmen_tree->root, "");
    display_dictionary(dictionary);

    // ------------------------- Free memory

    destroy_frequency_table(frequency_table);
    destroy_frequency_list(frequency_list);
    //destroy_huffmen_tree(huffmen_tree->root);
    destroy_dictionary(dictionary);

    return 0;
}

unsigned int *alloc_frequency_table()
{
    unsigned int *frequency_table = (unsigned int *)malloc(sizeof(unsigned int) * TAM);
    if (frequency_table){ return frequency_table; }
    else
    {
        printf("\n\tError allocating frequency_table\n");
        return nullptr;
    }
}
void initialize_frequency_table(unsigned int *frequency_table)
{
    if (!frequency_table)
    {
        printf("\n\tfrequency_table is NULL\n");
        return;
    }
    for (unsigned int i = 0; i < TAM; i++){ frequency_table[i] = 0; }
}
void fill_frequency_table(unsigned int *frequency_table, const char *file_name)
{
    if (!frequency_table)
    {
        printf("\n\tfrequency_table is NULL\n");
        return;
    }
    if (!file_name)
    {
        printf("\n\tfile_name is NULL\n");
        return;
    }
    FILE *f = fopen(file_name, "rb");
    if (f)
    {
        unsigned char byte;
        while (fread(&byte, sizeof(unsigned char), 1, f))
        {
            frequency_table[byte]++;
        }
        fclose(f);
    }else
    {
        printf("\n\tError opening file in \'fill_frequency_table\'\n");
    }
}
void display_frequency_table(unsigned int *frequency_table)
{
    if (!frequency_table){ return; }
    printf("\n\tDisplay Frequency Table:\n");
    for (unsigned int i = 0; i < TAM; i++)
    {
        if (frequency_table[i] > 0)
        {
            printf("<%c> : %u\n", i, frequency_table[i]);
        }
    }
}
void destroy_frequency_table(unsigned int *frequency_table)
{
    free(frequency_table);
}

list_t *alloc_frequency_list()
{
    list_t *frequency_list = (list_t *)malloc(sizeof(list_t));
    if (frequency_list)
    {
        frequency_list->head = nullptr;
        frequency_list->tail = nullptr;
        frequency_list->size = 0;
        return frequency_list;
    }else
    {
        printf("\n\tError allocating frequency_list\n");
        return nullptr;
    }
}
void insert_frequency_list(list_t *frequency_list, node_t *new_node)
{
    if (frequency_list && new_node) {
        if (frequency_list->size <= 0) {
            new_node->next = nullptr;
            new_node->prev = nullptr;
            frequency_list->head = new_node;
            frequency_list->tail = new_node;
        }else if (frequency_list->head->frequency > new_node->frequency) {
            new_node->next = frequency_list->head;
            new_node->prev = nullptr;
            frequency_list->head->prev = new_node;
            frequency_list->head = new_node;
        }else {
            node_t *current = frequency_list->head;
            while (current->next && current->next->frequency < new_node->frequency) {
                current = current->next;
            }
            if (!current->next) {
                new_node->next = nullptr;
                new_node->prev = frequency_list->tail;
                frequency_list->tail->next = new_node;
                frequency_list->tail = new_node;
            }else {
                new_node->next = current->next;
                new_node->prev = current;
                current->next = new_node;
                current->next->prev = new_node;
            }
        }
        frequency_list->size++;
    }
}
node_t *remove_frequency_list(list_t *frequency_list)
{
    if (!frequency_list)
    {
        printf("\n\tFrequency_List is NULL in \'remove_frequency_list\'\n");
        return nullptr;
    }
    if (frequency_list->size <= 0)
    {
        printf("Frequency_List is empty in \'remove_frequency_list\'\n");
        return nullptr;
    }
    node_t *temp = frequency_list->head;
    frequency_list->head = frequency_list->head->next;
    frequency_list->size--;
    temp->next = nullptr;
    return temp;
}
void fill_frequency_list(list_t *frequency_list, const unsigned int *frequency_table)
{
    if (!frequency_table)
    {
        printf("\n\tfrequency_table is NULL in \'fill_frequency_list\'\n");
        return;
    }
    if (!frequency_list)
    {
        printf("\n\tfrequency_list is NULL in \'fill_frequency_list\'\n");
        return;
    }
    for (unsigned int i = 0; i < TAM; i++)
    {
        if (frequency_table[i] > 0)
        {
            node_t *new_node = (node_t *)malloc(sizeof(node_t));
            new_node->left = nullptr;
            new_node->right = nullptr;
            new_node->frequency = frequency_table[i];
            new_node->character = (char) i;
            insert_frequency_list(frequency_list, new_node);
        }
    }
}
void destroy_frequency_list(list_t *frequency_list)
{
    if (!frequency_list){ return; }
    node_t *y = nullptr;
    node_t *x = frequency_list->head;
    while (x != nullptr)
    {
        y = x;
        x = x->next;
        free(y);
    }
    free(frequency_list);
}
void display_frequency_list(list_t *frequency_list)
{
    printf("\n\tDisplay Frequency List | Size of list <%d> :\n", frequency_list->size);
    node_t *temp = frequency_list->head;
    while (temp != NULL)
    {
        printf("<%c> : %u\n", temp->character, temp->frequency);
        temp = temp->next;
    }
}

huffmen_tree_t *alloc_huffmen_tree_t() {
    huffmen_tree_t *newTree = (huffmen_tree_t *)malloc(sizeof(huffmen_tree_t));
    if (newTree) {
        newTree->root = nullptr;
        return newTree;
    }else {
        return nullptr;
    }
}
node_t *huffmen_tree_build(list_t *frequency_list) {
    node_t *left, *right, *newNode;
    while (frequency_list->size > 1) {
        left = remove_frequency_list(frequency_list);
        right = remove_frequency_list(frequency_list);
        newNode = (node_t * )malloc(sizeof(node_t));
        newNode->character = '\0';
        newNode->frequency = 0;

        if (left) { newNode->frequency += left->frequency; }
        if (right) { newNode->frequency += right->frequency; }

        newNode->left = left;
        newNode->right = right;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        insert_frequency_list(frequency_list, newNode);
    }
    return frequency_list->head;
}
void display_huffmen_tree(node_t *root) {
    if (!root) return;
    display_huffmen_tree(root->left);
    printf("<%c> : %d\n", root->character, root->frequency);
    display_huffmen_tree(root->right);
}
int tree_height(node_t *root, int height) {
    if (!root) return height;
    int left_height = tree_height(root->left, height) + 1;
    int right_height = tree_height(root->right, height) + 1;
    return left_height >= right_height ? left_height : right_height;
}
void destroy_huffmen_tree(node_t *root) {
    if (!root) return;
    destroy_huffmen_tree(root->left);
    destroy_huffmen_tree(root->right);
    free(root);
}
char ** alloc_dictionary(int height) {
    char ** dictionary = (char **)malloc(sizeof(char *) * TAM);
    for (int i = 0; i < TAM; i++) {
        dictionary[i] = calloc(height + 1, sizeof(char));
    }
    return dictionary;
}
void build_dictionary(char **dictionary, node_t *root, char *str) {
    if (!root) return;
    if (root->left == nullptr && root->right == nullptr) {
        dictionary[root->character] = str;
    }
    char *left_str = calloc(strlen(str) + 1, sizeof(char));
    strcpy(left_str, str);
    strcat(left_str, "0");
    char *right_str = calloc(strlen(str) + 1, sizeof(char));
    strcpy(right_str, str);
    strcat(left_str, "1");
    build_dictionary(dictionary, root->left, left_str);
    build_dictionary(dictionary, root->right, right_str);
}
void display_dictionary(char **dictionary) {
    printf("\n\tDisplaying dictionary \n");
    for (int i = 0; i < TAM; i++) {
        if ((strcmp(dictionary[i], "") != 0)) {
            printf("<%c> : %s\n", i, dictionary[i]);
        }
    }
}
void destroy_dictionary(char **dictionary) {
    if (!dictionary) {return;}
    for (int i = 0; i < TAM; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
}
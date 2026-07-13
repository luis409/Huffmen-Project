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
void fill_frequency_list(list_t *frequency_list, unsigned int *frequency_table);
void destroy_frequency_list(list_t *frequency_list);
void display_frequency_list(list_t *frequency_list);

huffmen_tree_t *alloc_huffmen_tree_t();
void build_huffmen_tree_t(huffmen_tree_t *huffmen_tree, list_t *frequency_list);
void destroy_huffmen_tree_t(huffmen_tree_t *huffmen_tree);
void display_huffmen_tree_t(node_t *root);

char ** alloc_dictionary(int height);
int huffmen_tree_height(node_t *root, int height);
void build_huffmen_tree_str(char ** dictionary, huffmen_tree_t *huffmen_tree, int height);
void fill_dictionary(char **dictionary, node_t *root, char *str, int h);
void display_dictionary(char **dictionary);
void destroy_dictionary(char **dictionary);

char *code_string(char **dictionary, char *file_name);

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
    build_huffmen_tree_t(huffmen_tree, frequency_list);
    printf("\n\tDisplay Huffmen Tree:\n");
    display_huffmen_tree_t(huffmen_tree->root);

    //-------------------------- Dictionary
    int h = huffmen_tree_height(huffmen_tree->root, 0);
    printf("\n\tHuffmen Tree height: %d\n", h);
    dictionary = alloc_dictionary(h);
    build_huffmen_tree_str(dictionary, huffmen_tree, h);
    display_dictionary(dictionary);

    //-------------------------- Code String
    printf("\n\tCode String:\n");
    file_name = code_string(dictionary, file_name);

    // ------------------------- Free memory

    destroy_frequency_table(frequency_table);
    destroy_frequency_list(frequency_list);
    destroy_huffmen_tree_t(huffmen_tree);
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
        return NULL;
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
        frequency_list->head = NULL;
        frequency_list->tail = NULL;
        frequency_list->size = 0;
        return frequency_list;
    }else
    {
        printf("\n\tError allocating frequency_list\n");
        return NULL;
    }
}
void insert_frequency_list(list_t *frequency_list, node_t *new_node)
{
    if (frequency_list)
    {
        if (new_node)
        {
            if (frequency_list->size == 0)
            {
                new_node->next = NULL;
                new_node->prev = NULL;
                frequency_list->head = new_node;
                frequency_list->tail = new_node;
                //frequency_list->size++;
            }
            else
            {
                node_t *temp = frequency_list->head;
                while (temp != NULL && temp->frequency > new_node->frequency)
                {
                    temp = temp->next;
                }
                if (temp == NULL)
                {
                    new_node->next = NULL;
                    new_node->prev = frequency_list->tail;
                    frequency_list->tail->next = new_node;
                    frequency_list->tail = new_node;
                    //frequency_list->size++;
                }else
                {
                    if (temp->prev == NULL)
                    {
                        new_node->prev = NULL;
                        new_node->next = frequency_list->head;
                        frequency_list->head->prev = new_node;
                        frequency_list->head = new_node;
                    }else
                    {
                        new_node->prev = temp->prev;
                        new_node->next = temp;
                        temp->prev->next = new_node;
                        temp->prev = new_node;
                    }
                    //frequency_list->size++;
                }
            }
            frequency_list->size++;
        }else{ printf("\n\tnew_node is NULL in insert_frequency_list\n"); }
    }else{ printf("\n\tfrequency_list is NULL in insert_frequency_list\n"); }
}
node_t *remove_frequency_list(list_t *frequency_list)
{
    if (!frequency_list)
    {
        printf("\n\tFrequency_List is NULL in \'remove_frequency_list\'\n");
        return NULL;
    }
    if (frequency_list->size <= 0)
    {
        printf("Frequency_List is empty in \'remove_frequency_list\'\n");
        return NULL;
    }
    node_t *temp = frequency_list->head;
    frequency_list->head = frequency_list->head->next;
    frequency_list->size--;
    temp->next = NULL;
    return temp;
}
void fill_frequency_list(list_t *frequency_list, unsigned int *frequency_table)
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
            new_node->left = NULL;
            new_node->right = NULL;
            new_node->frequency = frequency_table[i];
            new_node->character = (char) i;
            insert_frequency_list(frequency_list, new_node);
        }
    }
}
void destroy_frequency_list(list_t *frequency_list)
{
    if (!frequency_list){ return; }
    node_t *y = NULL;
    node_t *x = frequency_list->head;
    while (x != NULL)
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

huffmen_tree_t *alloc_huffmen_tree_t()
{
    huffmen_tree_t *tree = (huffmen_tree_t *)malloc(sizeof(huffmen_tree_t));
    if (tree)
    {
        tree->root = NULL;;
        return tree;
    }else
    {
        printf("\n\tError memory alocation in \'alloc_huffmen_tree_t\'\n");
        return NULL;
    }
}
void build_huffmen_tree_t(huffmen_tree_t *huffmen_tree, list_t *frequency_list)
{
    if (!huffmen_tree) {
        printf("\n\tErro huffmen_tree is NULL in \'build_huffmen_tree_t\'\n");
        return;
    }
    if (!frequency_list) {
        printf("\n\tFrequency_List is NULL in \'build_huffmen_tree_t\'\n");
        return;
    }
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->frequency = UINT_MAX;
    new_node->character = (char) '\0';
    insert_frequency_list(frequency_list, new_node);

    //int n = frequency_list->size - 1;
    huffmen_tree->root = frequency_list->head;

    node_t *x = huffmen_tree->root;
    node_t *y = frequency_list->head->next;

    while (y != NULL && x != NULL) {
        if (!x->left && !x->right) {
            x->left = y;
            y = y->next;
        }else if (x->left && !x->right) {
            x->right = y;
            y = y->next;
        }else if (x->right && !x->left) {
            x->left = y;
            y = y->next;
        }else {
            x = x->next;
        }
    }
}
void destroy_huffmen_tree_t(huffmen_tree_t *huffmen_tree) {
    if (!huffmen_tree){ return; }
    free(huffmen_tree);
}
void display_huffmen_tree_t(node_t *root) {
    if (root == NULL){ return; }
    printf("<%c> : %u\n", root->character, root->frequency);
    display_huffmen_tree_t(root->left);
    display_huffmen_tree_t(root->right);
}

char ** alloc_dictionary(int height) {
    char ** dictionary = (char **)malloc(sizeof(char *) * TAM);
    if (dictionary) {
        for (int i = 0; i < TAM; i++) {
            dictionary[i] = calloc(height + 1, sizeof(char));
        }
        return dictionary;
    }else {
        printf("\n\tError allocation memory failed in \'alloc_dictionary\'\n");
        return nullptr;
    }
}
int huffmen_tree_height(node_t *root, int height) {
    if (root == NULL){ return height; }
    int left_height = huffmen_tree_height(root->left, height) + 1;
    int right_height = huffmen_tree_height(root->right, height) + 1;
    return left_height > right_height ? left_height : right_height;
}
void build_huffmen_tree_str(char ** dictionary, huffmen_tree_t *huffmen_tree, int height) {
    fill_dictionary(dictionary, huffmen_tree->root->left, "0", height);
    fill_dictionary(dictionary, huffmen_tree->root->right, "1", height);
}
void fill_dictionary(char **dictionary, node_t *root, char *str, int h) {
    if (root == NULL) { return; }
    if (dictionary == NULL) {
        printf("\n\tError dictionary is NULL in \'fill_dictionary\'\n");
        return;
    }
    unsigned char idx = (unsigned char) root->character;
    strcpy(dictionary[idx], str);

    char *leftStr = calloc(h + 1, sizeof(char));
    strcpy(leftStr, str);
    char *rightStr = calloc(h + 1, sizeof(char));
    strcpy(rightStr, str);

    strcat(leftStr, "0");
    fill_dictionary(dictionary, root->left, leftStr, h);
    strcat(rightStr, "1");
    fill_dictionary(dictionary, root->right, rightStr, h);
}
void display_dictionary(char **dictionary) {
    if (!dictionary) {
        printf("\n\tDictionary is NULL in \'display_dictionary\'\n");
        return;
    }
    printf("\n\tDisplay Dictionary :\n");
    for (int i = 0; i < TAM; i++) {
        if (strcmp(dictionary[i], "") != 0) {
            printf("<%c> : %s\n", i, dictionary[i]);
        }
    }
}
void destroy_dictionary(char **dictionary) {
    if (!dictionary){ return; }
    for (int i = 0; i < TAM; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
}

char *code_string(char **dictionary, char *file_name) {
    FILE *f = fopen(file_name, "rb");
    if (f) {
        int n = strlen(file_name);
        char *newFileName = calloc((n + 5), sizeof(char));
        strcpy(newFileName, file_name);
        strcat(newFileName, ".cod");
        FILE *fw = fopen(newFileName, "w");
        if (fw) {
            unsigned char byte;
            while (fread(&byte, sizeof(unsigned char), 1, f)) {
                printf("%s", dictionary[byte % 256]);
                fprintf(fw, "%s", dictionary[byte % 256]);
            }printf("\n");
            fclose(fw);
            fclose(f);
            return newFileName;
        }else {
            printf("File_name read error in \'code_string\' block one\n");
            return nullptr;
        }
    }else {
        printf("File_name read error in \'code_string\'\n");
        return nullptr;
    }
}
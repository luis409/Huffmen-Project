//
// Created by gauss on 10/07/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TAM 256
#define MAX_FREQUENCY UINT_MAX

typedef struct node
{
    struct node *next;
    struct node *prev;
    struct node *left;
    struct node *right;
    char character;
    unsigned int frequency;
} node_t;

typedef struct frequency_list
{
    struct node *head;
    struct node *tail;
    int size;
} list_t;

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

int main()
{
    unsigned int *frequency_table;
    list_t *frequency_list;

    //-------------------------- Frequency Table
    frequency_table = alloc_frequency_table();
    initialize_frequency_table(frequency_table);
    fill_frequency_table(frequency_table, "test_1.txt");
    display_frequency_table(frequency_table);

    //-------------------------- Frequency List
    frequency_list = alloc_frequency_list();
    fill_frequency_list(frequency_list, frequency_table);
    display_frequency_list(frequency_list);

    destroy_frequency_table(frequency_table);
    destroy_frequency_list(frequency_list);
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
                while (temp != NULL && temp->frequency >= new_node->frequency)
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
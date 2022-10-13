#include "list.h"

typedef struct node
{
    struct node* prev;
    void* data;
    struct node* next;
} node;

typedef struct info
{
    node* head;
    int size;
    node* tail;
} info;

void* createList()
{
    info* list = calloc(1, sizeof(info));
    return list;
}

void freeList(void* listtofree)
{
    info* list = listtofree;
    node* current = list->head;
    while (current)
    {
        node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

void* createNode(void* data)
{
    node* n = calloc(1, sizeof(node));
    n->data = data;
    return n;
}

void insertAtTail(void* listtoadd, void* nodetoadd)
{
    info* list = listtoadd;
    node* n = nodetoadd;
    if (!list->head)
    {
        list->head = n;
        list->tail = n;
    }
    else
    {
        list->tail->next = n;
        n->prev = list->tail;
        list->tail = n;
    }
    list->size++;
}

void removeNode(void* listtoremove, void* nodetoremove)
{
    info* list = listtoremove;
    node* n = nodetoremove;
    if (!n->prev && !n->next)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else if (!n->prev)
    {
        list->head = n->next;
        n->next->prev = NULL;
    }
    else if (!n->next)
    {
        list->tail = n->prev;
        n->prev->next = NULL;
    }
    else
    {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    list->size--;
    free(n);
}

/*>>>>>>>>>>All sets<<<<<<<<<<*/

void setHead(void* listtoset, void* nodetoset)
{
    info* list = listtoset;
    list->head = nodetoset;
}

/*>>>>>>>>>>All gets<<<<<<<<<<*/

void* getPrev(void* nodetoget)
{
    node* n = nodetoget;
    return n->prev;
}

void* getData(void* nodetoget)
{
    node* n = nodetoget;
    return n->data;
}

void* getNext(void* nodetoget)
{
    node* n = nodetoget;
    return n->next;
}

void* getHead(void* listtoget)
{
    info* l = listtoget;
    return l->head;
}

int getSize(void* list)
{
    info* l = list;
    return l->size;
}

void* getTail(void* list)
{
    info* l = list;
    return l->tail;
}
#include "list.h"

typedef struct node
{
    void* data;
    struct node* next;
} node;

typedef struct info
{
    node* head;
    int size;
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
    }
    else
    {
        node* current = list->head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = n;
    }
    list->size++;
}

void removeNode(void* listtoremove, void* nodetoremove)
{
    info* list = listtoremove;
    node* n = nodetoremove;
    if (list->head == n)
    {
        list->head = n->next;
    }
    else
    {
        node* current = list->head;
        while (current->next != n)
        {
            current = current->next;
        }
        current->next = n->next;
    }
    free(n);
    list->size--;
}

/*>>>>>>>>>>All sets<<<<<<<<<<*/

void setHead(void* listtoset, void* nodetoset)
{
    info* list = listtoset;
    list->head = nodetoset;
}

/*>>>>>>>>>>All gets<<<<<<<<<<*/

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

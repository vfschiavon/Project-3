#include "libs.h"

#ifndef LIST_H
#define LIST_H

typedef void* Lista;

void* createList();

void freeList(void* listtofree);

void* createNode(void* data);

void insertAtTail(void* listtoadd, void* nodetoadd);

void removeNode(void* listtoadd, void* nodetoremove);

/*>>>>>>>>>>All sets<<<<<<<<<<*/

void setHead(void* listtoset, void* nodetoset);

/*>>>>>>>>>>All gets<<<<<<<<<<*/

void* getPrev(void* nodetoget);

void* getData(void* nodetoget);

void* getNext(void* nodetoget);

void* getHead(void* listtoget);

int getSize(void* listtoget);

void* getTail(void* listtoget);

#endif
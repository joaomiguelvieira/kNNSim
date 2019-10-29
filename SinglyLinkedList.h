#ifndef __SINGLY_LINKED_LIST_HEADER__
#define __SINGLY_LINKED_LIST_HEADER__

#include <stdlib.h>
#include <assert.h>

typedef struct SinglyLinkedListNode_ {
  void *object;
  struct SinglyLinkedListNode_ *nextNode;
} SinglyLinkedListNode;

typedef struct SinglyLinkedList_ {
  SinglyLinkedListNode *head, *tail;
  int numberNodes;
  void (*deleteObject)(void *);
} SinglyLinkedList;

SinglyLinkedList *newSinglyLinkedList(void (* deleteObject)(void *));
void deleteSinglyLinkedList(SinglyLinkedList *singlyLinkedList);

void insertObjectIntoSinglyLinkedListAtHead(SinglyLinkedList *singlyLinkedList, void *object);
void insertObjectIntoSinglyLinkedListAtTail(SinglyLinkedList *singlyLinkedList, void *object);

SinglyLinkedListNode *getHeadSinglyLinkedList(SinglyLinkedList *singlyLinkedList);
SinglyLinkedListNode *getTailSinglyLinkedList(SinglyLinkedList *singlyLinkedList);
SinglyLinkedListNode *getNextNodeSinglyLinkedList(SinglyLinkedListNode *singlyLinkedListNode);

void *getObjectFromSinglyLinkedListNode(SinglyLinkedListNode *singlyLinkedListNode);

int getLengthSinglyLinkedList(SinglyLinkedList *singlyLinkedList);

#endif

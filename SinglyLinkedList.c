#include "SinglyLinkedList.h"

SinglyLinkedListNode *newSinglyLinkedListNode(void *object) {
  SinglyLinkedListNode *singlyLinkedListNode = (SinglyLinkedListNode *) malloc(sizeof(SinglyLinkedListNode));
  assert(singlyLinkedListNode != NULL);

  singlyLinkedListNode->object = object;

  return singlyLinkedListNode;
}

void deleteSinglyLinkedListNode(SinglyLinkedListNode *singlyLinkedListNode, void (* deleteObject)(void *)) {
  deleteObject(singlyLinkedListNode->object);
  free(singlyLinkedListNode);
}

SinglyLinkedList *newSinglyLinkedList(void (* deleteObject)(void *)) {
  SinglyLinkedList *singlyLinkedList = (SinglyLinkedList *) malloc(sizeof(SinglyLinkedList));
  assert(singlyLinkedList != NULL);

  singlyLinkedList->head = NULL;
  singlyLinkedList->tail = NULL;
  singlyLinkedList->numberNodes = 0;
  singlyLinkedList->deleteObject = deleteObject;

  return singlyLinkedList;
}

void deleteSinglyLinkedList(SinglyLinkedList *singlyLinkedList) {
  while (singlyLinkedList->head) {
    SinglyLinkedListNode *newHead = singlyLinkedList->head->nextNode;
    deleteSinglyLinkedListNode(singlyLinkedList->head, singlyLinkedList->deleteObject);
    singlyLinkedList->head = newHead;
  }

  free(singlyLinkedList);
}

void insertObjectIntoSinglyLinkedListAtHead(SinglyLinkedList *singlyLinkedList, void *object) {
  SinglyLinkedListNode *node = newSinglyLinkedListNode(object);

  node->nextNode = singlyLinkedList->head;
  singlyLinkedList->head = node;

  if (!(singlyLinkedList->tail))
    singlyLinkedList->tail = node;

  singlyLinkedList->numberNodes++;
}

void insertObjectIntoSinglyLinkedListAtTail(SinglyLinkedList *singlyLinkedList, void *object) {
  SinglyLinkedListNode *node = newSinglyLinkedListNode(object);

  node->nextNode = NULL;

  if (!(singlyLinkedList->tail)) {
    singlyLinkedList->head = node;
    singlyLinkedList->tail = node;
  }
  else {
    singlyLinkedList->tail->nextNode = node;
    singlyLinkedList->tail = node;
  }

  singlyLinkedList->numberNodes++;
}

SinglyLinkedListNode *getHeadSinglyLinkedList(SinglyLinkedList *singlyLinkedList) {
  return singlyLinkedList->head;
}

SinglyLinkedListNode *getNextNodeSinglyLinkedList(SinglyLinkedListNode *singlyLinkedListNode) {
  return singlyLinkedListNode->nextNode;
}

void *getObjectFromSinglyLinkedListNode(SinglyLinkedListNode *singlyLinkedListNode) {
  return singlyLinkedListNode->object;
}

int getLengthSinglyLinkedList(SinglyLinkedList *singlyLinkedList) {
  return singlyLinkedList->numberNodes;
}

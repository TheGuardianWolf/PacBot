#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#import <stddef.h>

typedef struct {
	void* item;
    LLNode* next;
    LLNode* prev;
} LLNode;

typedef struct {
    LLNode* first;
    LLNode* last;
    size_t size;
} LinkedList;

LinkedList* linked_list_create();

// LIFO
void linked_list_push(const LinkedList* list, void* item);

void* linked_list_pop(const LinkedList* list);

// FIFO
void linked_list_add(const LinkedList* list, void* item);

void* linked_list_remove(const LinkedList* list);

#endif /* LINKED_LIST_H */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

struct LLNode {
	void* item;
    struct LLNode* next;
    struct LLNode* prev;
};

typedef struct LLNode LLNode;

typedef struct {
    LLNode* first;
    LLNode* last;
    size_t size;
} LinkedList;

LinkedList* linked_list_create();

// LIFO
void linked_list_push(LinkedList* list, void* item);

void* linked_list_pop(LinkedList* list);

void* linked_list_peek_stack(LinkedList* list);

// FIFO
void linked_list_add(LinkedList* list, void* item);

void* linked_list_remove(LinkedList* list);

void* linked_list_peek_queue(LinkedList* list);

void linked_list_destroy(LinkedList* list);

#endif /* LINKED_LIST_H */

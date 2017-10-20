#include <stdlib.h>
#include <stddef.h>
#include "linked_list.h"

LinkedList* linked_list_create() {
	LinkedList* list = malloc(sizeof(LinkedList));
	if (list != NULL) {
		list->first = NULL;
		list->last = NULL;
		list->size = 0;
		return list;
	}
	return NULL;
}

// LIFO
void linked_list_push(LinkedList* list, void* item) {
	LLNode* node = malloc(sizeof(LLNode));
	if (node != NULL) {
		node->item = item;
		if (list->size == 0) {
			list->last = node;
		}
		else {
			list->first->prev = node;
		}
		node->next = list->first;
		node->prev = NULL;
		list->first = node;
		list->size++;
	}
}

void* linked_list_pop(LinkedList* list) {
	return linked_list_remove(list);
}

// void* linked_list_peek_stack(LinkedList* list) {
// 	if (list->size > 0) {
// 		return list->last->item;
// 	}
// 	return NULL;
// }

// FIFO
void linked_list_add(LinkedList* list, void* item) {
	LLNode* node = malloc(sizeof(LLNode));
	if (node != NULL) {
		node->item = item;
		if (list->size == 0) {
			list->first = node;
		}
		else {
			list->last->next = node;
		}
		node->next = NULL;
		node->prev = list->last;
		list->last = node;
		list->size++;
	}
}

void* linked_list_remove(LinkedList* list) {
    if (list->size > 0) {
    	LLNode* node = list->first;
    	list->size--;
    	if (list->size == 0) {
    		list->first = NULL;
    		list->last = NULL;
    	}
    	else {
    		list->first = node->next;
    	}
    	
    	void* item = node->item;
    	free(node);
    	
    	return item;
    }
    return NULL;
}

void* linked_list_peek(LinkedList* list) {
	if (list->size > 0) {
		return list->first->item;
	}
	return NULL;
}

void linked_list_destroy(LinkedList* list) {
	LLNode* node;
	while(list->size > 0) {
		node = list->first;
		list->first = node->next;
		free(node);
		list->size--;
	}
	free(list);
}

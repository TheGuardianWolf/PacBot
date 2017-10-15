#include <stdlib.h>

LinkedList* linked_list_create() {
	LinkedList list = malloc(sizeof(LinkedList));
	if (list != NULL) {
		list->first = NULL;
		list->last = NULL;
		list->size = 0;
		return list;
	}
	return NULL;
}

// LIFO
void linked_list_push(const LinkedList* list, void* item) {
	LLNode* node = malloc(sizeof(LLNode*));
	if (node != NULL) {
		node->item = item;
		list->first->prev = node;
		node->next = list->first;
		node->prev = NULL;
		list->first = node;
		if (list->size == 0) {
			list->last = node;
		}
		list->size++;
	}
}

void* linked_list_pop(const LinkedList* list) {
	return linked_list_remove(list);
}

void* linked_list_peek_stack(const LinkedList* list) {
	if (list->size > 0) {
		return list->last->item;
	}
	return NULL;
}

// FIFO
void linked_list_add(const LinkedList* list, void* item) {
	LLNode* node = malloc(sizeof(LLNode*));
	if (node != NULL) {
		node->item = item;
		list->last->next = node;
		node->next = NULL;
		node->prev = list->last;
		list->last = node;
		if (list->size == 0) {
			list->first = node;
		}
		list->size++;
	}
}

void* linked_list_remove(const LinkedList* list) {
	LLNode* node = list->first;
	node->next->prev = NULL;
	list->first = node->next;
	void* item = node->item;
	free(node);
	list->size--;
	if (list->size == 0) {
		list->last = NULL;
	}
	return item;	
}

void* linked_list_peek_queue(const LinkedList* list) {
	if (list->size > 0) {
		return list->first->item;
	}
	return NULL;
}

void linked_list_destroy(const LinkedList* list) {
	LLNode* node, next_node;
	while(list->size > 0) {
		node = list->first;
		next_node = node->next;
		free(node);
		list->size--;
	}
	free(list);
}

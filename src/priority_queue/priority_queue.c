#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "priority_queue.h"

Node* pq_enqueue(Node** a_head, void* a_value, int (*cmp_fn)(const void*, const void*)) {
	// Find the node just before where we want to insert it.
	Node** a_curr = a_head; // Start at the address of the head node.
	for(; *a_curr != NULL && cmp_fn != NULL && cmp_fn(a_value, (*a_curr) -> a_value) >= 0; a_curr = &(*a_curr) -> next);
	// After the address of the head node, we store the address of the head node's next value. 
	// Then the next node's next value. And so on.

	// Create new node -- allocate and initialize (using a compound literal).
	Node* new_node = malloc(sizeof(*new_node));
	*new_node = (Node) { .a_value = a_value, .next = *a_curr };

	// Place it in the list.
	// - This conveniently works even if the new node is the new head node.
	*a_curr = new_node;

	return new_node;
}

Node* pq_dequeue(Node** a_head) {
	if(*a_head == NULL) {
		return NULL;
	}
	Node* removed_node = *a_head; 	// Store address to the head.
	*a_head = (*a_head) -> next; 	// New head is now whatever came after the old head.
	removed_node -> next = NULL;	// Removed node must be a valid list of size 1.
	return removed_node;
}

Node* stack_push(Node** a_head, void* a_value) {
	/*
	Node* new_node = malloc(sizeof(*new_node));
	*new_node = (Node) { .a_value = a_value, .next = *a_head };
	*a_head = new_node;
	return *a_head;
	*/
	return pq_enqueue(a_head, a_value, NULL);
}

Node* stack_pop(Node** a_head) {
	return pq_dequeue(a_head);
}

void destroy_list(Node** a_head, void (*destroy_value_fn)(void*)) {
	if(*a_head == NULL) {
		return;
	}
	destroy_list(&((*a_head) -> next), destroy_value_fn); // Remove the head from the list.
	if(destroy_value_fn != NULL) {
		destroy_value_fn((*a_head) -> a_value);           // Deallocate memory for payload, if needed.
	}
	free(*a_head);                                        // Deallocate memory for the node itself.
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */

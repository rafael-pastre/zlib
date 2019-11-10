#include "zstrmultiset.h"

// Creates a empty multiset
StrMultiSet* str_multiset_create(){
	StrMultiSet* str_multiset = NULL;
	str_multiset = malloc(sizeof(StrMultiSet));
	if(!str_multiset)
		return NULL;
	*str_multiset = NULL;
	return str_multiset;
}

// Counts the number of different strings on the multiset
int str_multiset_count(StrMultiSet* str_multiset){
	StrMultiSetNode* node = *str_multiset;
	int i = 0;
	
	while(node != NULL){
		i++;
		node = node->next;
	}
	
	return i;
}

// Insert string into multiset
int str_multiset_insert(StrMultiSet* str_multiset, const char* str){
	StrMultiSetNode* node = *str_multiset;

	// Verfies if there is an existent element
	while(node != NULL){
		if(!strcmp(node->str, str)){
			node->count++;
			return 1;
		}
		node = node->next;
	}
	
	// If the element is a new one insert at the begining
	node = *str_multiset;
	node = malloc(sizeof(StrMultiSetNode));
	strcpy((node)->str, str);
	node->count = 1;
	node->next = *str_multiset;
	*str_multiset = node;
	
	return 1;
}

//TO-DO
int str_multiset_remove(StrMultiSet* str_multiset, const char* str){
	StrMultiSetNode* node = *str_multiset;
	StrMultiSetNode* prev = NULL;

	// Verfies if there is an existent element
	while(node != NULL){
		if(!strcmp(node->str, str)){
			// Decrement node count and verifies if all elemnts were removed
			if(--(node->count) == 0){
				if(prev == NULL){
					// Remove 1 element
					*str_multiset = node->next;
					free(node);
				}
				else{
					// Other elements
					prev->next = node->next;
					free(node);
				}
			}
			return 1;
		}
		prev = node;
		node = node->next;
	}
	
	return 0;
}

void print_str_multiset(StrMultiSet* str_multiset){
	StrMultiSetNode* node = *str_multiset;
	int i = 0;
	
	printf("Map : %p -> %p, numer of elements = %d\n", str_multiset, *str_multiset, str_multiset_count(str_multiset));
	
	while(node != NULL){
		printf("| elem %d (%p): str = %s, count = %d, next = %p\n", i++, node, node->str, node->count, node->next);
		node = node->next;
	}
	
	printf("|--------\n\n");
}
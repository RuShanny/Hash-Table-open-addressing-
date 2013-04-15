#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include "hash.h"

iterator SetToBegin(hash_table* hash_info){
  iterator itbegin;
	ptr_iterator ptr_begin = &itbegin;
	ptr_begin->hashtab_ptr = hash_info;
	ptr_begin->current = hash_info->hash_ptr;
	return itbegin;
}

iterator SetToEnd(hash_table* hash_info) {	
	iterator itend;
	ptr_iterator ptr_end = &itend;
	ptr_end->hashtab_ptr = hash_info;
	ptr_end->current = &(hash_info->hash_ptr[hash_info->size-1]);
	return itend;
}

char** Value(iterator it) {
	return it.current;
}

iterator Next(iterator it) {
	if (it.current == &((it.hashtab_ptr)->hash_ptr[(it.hashtab_ptr)->size-1]))
		printf("Got to the end of hashtable: can not go further");
	else it.current++;
	return it;
}

iterator Back(iterator it) {
	if (it.current == (it.hashtab_ptr)->hash_ptr)
		printf("Got to the beginning of hashtable: can not go further");
	else it.current--;
	return it;
}

int CheckBegin(iterator it) {
	if (it.current == (it.hashtab_ptr)->hash_ptr) return 1;
	else return 0;
}

int CheckEnd(iterator it) {
	if (it.current == &((it.hashtab_ptr)->hash_ptr[(it.hashtab_ptr)->size-1])) return 1;
	else return 0;
}

void CleanMemory(hash_table* hash_info) {
	iterator it;
	char** i;
	for ( it = SetToBegin(hash_info); !CheckEnd(it) ; it = Next(it) )  {
		i = Value(it);
		free(*i);
	}
	free(it.hashtab_ptr->hash_ptr);
	free(it.hashtab_ptr->deleted_ptr);
}

void CreateHash(hash_table* hash_info) {
	hash_info->hash_ptr = (char**)calloc(hash_info->size, sizeof(char*));
	hash_info->deleted_ptr = (int*)malloc(hash_info->size*sizeof(int));
	if ( (hash_info->hash_ptr == NULL)||(hash_info->deleted_ptr == NULL) ) {
		printf("Failed in memory allocation\n");
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	int i;
	for (i = 0; i < hash_info->size; i++) 
		hash_info->deleted_ptr[i] = 1;
}

hash_table* ResizeHash(hash_table* hash_info) {
	int i;
	hash_info->size = hash_info->size*2;
	char** tmp = hash_info->hash_ptr;
	int* tmp_deleted = hash_info->deleted_ptr;
	CreateHash(hash_info);
	for (i = 0; i < ((hash_info->size)/2); i++) 
		if (tmp[i] != NULL) hash_info->hash_ptr[i] = tmp[i];
	for (i = 0; i < ((hash_info->size)/2); i++) 
		if (tmp_deleted[i] != 1) hash_info->deleted_ptr[i] = tmp_deleted[i];
	free(tmp);
	free(tmp_deleted);
	return hash_info;
}

//hash(S)  =  S[0]  +  S[1] * P  +  S[2] * P^2  +  S[3] * P^3  +  ...  +  S[N] * P^N
unsigned long HashFunction1 (char *str){
	const int p = 31;
	size_t i;
	long int len = strlen(str);
	unsigned long hash = 0;
	int p_pow = 1;
	if (str[0] == '\0') {
		return 1;
	}
	for (i = 0; i < len; i++ ) {
		hash += (str[i] - '1' + 1) * p_pow;
		p_pow *= p;
	}
	return abs(hash);
}
 
unsigned long HashFunction2 (char* str) {
	unsigned long hash2 = strlen(str);
	if (str[0] == '\0') {
		return 3;
	}
	return hash2;
}


// if a collision occured while adding new element, call for the second hash_function 
// and then try to put an element to the cell with number = (hash1 + hash2 * time) mod size_of_hashtable,
// where time shows how many times occured collision for this current element 

int AddElement(char* str, hash_table* hash_info) {
	int i, time = 0;
	int flag = 0;
	unsigned long hash1 = HashFunction1(str), hash2 = HashFunction2(str);
	int hash = (hash1 + hash2*time) % hash_info->size;
	for (i = 0; i < hash_info->size; i++) {
		if (( hash_info->hash_ptr[hash] == NULL ) || ( hash_info->deleted_ptr[hash] )) {
			hash_info->hash_ptr[hash] = (char *)malloc(sizeof(char)*(strlen(str)+1));
			if ( hash_info->hash_ptr[hash] == NULL ) {
				printf("Failed in memory allocation\n");
				printf("%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			hash_info->deleted_ptr[hash] = 0;
			strcpy(hash_info->hash_ptr[hash], str);
			flag = 1;
			break;
		} else { 
			if ( strcmp(hash_info->hash_ptr[hash], str) == 0 ) {
				printf("This element is already in hash_table\n");
				return 0;			
			}
			time++;
			hash = (hash1 + hash2*time) % hash_info->size; 
		} 
	} 
	if (flag == 0) {
		hash_info = ResizeHash(hash_info);
		int res = AddElement(str, hash_info);
		if (res == 0) return 0;	
	}
	return 0;	
}

char* SearchElement (char* str, hash_table* hash_info) {
	int i, time = 0;
	unsigned long hash1 = HashFunction1(str), hash2 = HashFunction2(str);
	int hash = (hash1 + hash2 * time) % (hash_info->size);
	for (i = 0; i < hash_info->size; i++) {
		if ( hash_info->hash_ptr[hash] != NULL ) {
			if (( strncmp(hash_info->hash_ptr[hash], str, strlen(str)) == 0 ) && ( hash_info->deleted_ptr[hash] != 1 )) {
			return hash_info->hash_ptr[hash];
			}
		} else { 
			time++;
			hash = (hash1 + hash2*time) % (hash_info->size); 
		} 
	} 
	return NULL;
}
 

int Remove(char* str, hash_table* hash_info) {
	int i, time = 0;
	unsigned long hash1 = HashFunction1(str), hash2 = HashFunction2(str);
	int hash = (hash1 + hash2 * time) % (hash_info->size);
	for (i = 0; i < hash_info->size; i++) {
		if ( hash_info->hash_ptr[hash] != NULL ) {
			if ( strncmp(hash_info->hash_ptr[hash], str, strlen(str)) == 0 ) {
				hash_info->deleted_ptr[hash] = 1;
				free(hash_info->hash_ptr[hash]);
				hash_info->hash_ptr[hash] = NULL;
				return 1;  // Object was removed
			}
		} else { 
			time++;
			hash = (hash1 + hash2*time) % (hash_info->size); 
		} 
	} 
	return 0;	//if we reached this line it means that we failed in removing: object wasn't found   
}


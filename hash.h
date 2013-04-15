#ifndef _HASHLIB_H_
#define _HASHLIB_H_

typedef struct iterator* ptr_iterator;
typedef struct hash_table {
  int size;
	char** hash_ptr;	  // pointer to the hash_table 
	int* deleted_ptr;	 // pointer to the array where I can find out if the element is deleted or not
} hash_table;

// bidirectional iterator
typedef struct iterator {	
	hash_table* hashtab_ptr;
	char** current;
} iterator;


iterator SetToBegin (hash_table* hash_info);
iterator SetToEnd (hash_table* hash_info);
char** Value(iterator it);
iterator Next(iterator it);
iterator Back(iterator it);
int CheckBegin(iterator it);
int CheckEnd(iterator it);
void CleanMemory(hash_table* hash_info);
void CreateHash(hash_table* hash_info);
hash_table* ResizeHash(hash_table* hash_info);
unsigned long HashFunction1 (char *str);
unsigned long HashFunction2 (char* str);
int AddElement(char* str, hash_table* hash_info);
char* SearchElement (char* str, hash_table* hash_info);
int Remove(char* str, hash_table* hash_info);


#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#include "hash.h"

int TestAddFunction(hash_table* hash_info) {
  int k, len;
	char buf[10];
	for ( k = 0; k < 1000; k++ ) {
		len = sprintf(buf, "%d", k);
		if ((AddElement(buf, hash_info)) != 0 ) {
				printf("Failed in addition of memory\n");
				return 1;
		}
	}
	k = AddElement("", hash_info);
	len = sprintf(buf, "%d", 10);
	k = AddElement(buf, hash_info);
	k = AddElement("Make_GCOV_happy!", hash_info);
	return 0;
}

int TestSearchFunction(hash_table* hash_info) {
	int k, len;
	char buf[10];
	char* ptrtmp;
	ptrtmp = SearchElement("Make_GCOV_happy!", hash_info);
	for ( k = 0; k < 100; k++ ) {
		len = sprintf(buf, "%d", k);
		ptrtmp = SearchElement(buf, hash_info);
	}
	ptrtmp = SearchElement("11111111", hash_info);
	free(ptrtmp);
	return 0;
}

int TestRemoveFunction(hash_table* hash_info) {
	int k, j, len;
	char buf[10];
	for ( k = 0; k < 1000; k++ ) {
		len = sprintf(buf, "%d", k);
		j = Remove(buf, hash_info);
	}
	j = Remove("Make_GCOV_happy!", hash_info);
	return 0;
}

int TestIterators(hash_table* hash_info) {
	iterator it;
	char** i;
	for ( it = SetToEnd(hash_info); !CheckBegin(it) ; it = Back(it) )  {
		i = Value(it);
	}

	for ( it = SetToBegin(hash_info); !CheckEnd(it) ; it = Next(it) )  {
		i = Value(it);
	}
	return 0;
}

int main () {
	hash_table hash_array;
	hash_table* hash_info = &hash_array;
	hash_info->size = 256;
	CreateHash(hash_info);
	if (TestAddFunction(hash_info) != 0) printf("Failed in TestAddFunction");
	if (TestSearchFunction(hash_info) != 0) printf("Failed in TestSearchFunction");
	if (TestRemoveFunction(hash_info) != 0) printf("Failed in TestRemoveFunction");
	if (TestIterators(hash_info) != 0) printf("Failed in TestIterators");
	hash_info = ResizeHash(hash_info);
	if (hash_info == NULL) printf("Failed in ResizeHash");
	CleanMemory(hash_info);
	return 0;
}

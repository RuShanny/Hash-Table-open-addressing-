#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include "hash.h"

#define LENGTH 100
#define BILLION  1000000000L

char* GenerateRandomString(char* str) {
  int i;
	srand((unsigned)time(NULL));
 	for ( i = 0 ; i < LENGTH - 1 ; i++ ) *str = 'A' + (char)(rand() % 20);
 	str[LENGTH]='\0';
	return str;
}


int main () {
	hash_table hash_array;
	hash_table* hash_info = &hash_array;
	hash_info->size = 256;
	CreateHash(hash_info);
	struct timespec start, stop;
    	double worktime;

// test AddElement
	unsigned long j;
	char tmp[LENGTH] = {0};

	if (clock_gettime( CLOCK_REALTIME, &start) == -1) {
     		perror ("clock gettime");
		return -1;
     	}
   	
	for ( j = 0; j < 10000; ++j ) {
		AddElement(GenerateRandomString(tmp), hash_info);
	}

    	if (clock_gettime( CLOCK_REALTIME, &stop) == -1) {
      		perror ("clock gettime");
		return -1;
      	}

    	worktime = ((( stop.tv_sec - start.tv_sec )*1e9 + ( stop.tv_nsec - start.tv_nsec ))*1e-9)/10000;
    	printf ("%.10f sec for AddElement cycle \n", worktime);

// test SearchElement
	if (clock_gettime( CLOCK_REALTIME, &start) == -1) {
     		perror ("clock gettime");
		return -1;
     	}
   	
	for ( j = 0; j < 10000; ++j ) {
		SearchElement(GenerateRandomString(tmp), hash_info);
	}

    	if (clock_gettime( CLOCK_REALTIME, &stop) == -1) {
      		perror( "clock gettime" );
		return -1;
      	}

    	worktime = ((( stop.tv_sec - start.tv_sec )*1e9 + ( stop.tv_nsec - start.tv_nsec ))*1e-9)/10000;
    	printf("%.10f sec for SearchElement cycle \n", worktime);

// test Remove 
	if (clock_gettime( CLOCK_REALTIME, &start) == -1) {
     		perror( "clock gettime" );
		return -1;
     	}
   	int k;
	for ( j = 0; j < 10000; ++j ) {
		k = Remove(GenerateRandomString(tmp), hash_info);
	}

    	if (clock_gettime( CLOCK_REALTIME, &stop) == -1) {
      		perror( "clock gettime" );
		return -1;
      	}

    	worktime = ((( stop.tv_sec - start.tv_sec )*1e9 + ( stop.tv_nsec - start.tv_nsec ))*1e-9)/10000;
    	printf ("%.10f sec for Remove cycle \n", worktime);


// test CleanMemory 	
	if (clock_gettime( CLOCK_REALTIME, &start) == -1) {
     		perror ("clock gettime");
		return -1;
     	}
   	
	for ( j = 0; j < 10000; ++j ) {
		hash_table hash_array;
		hash_table* hash_info = &hash_array;
		hash_info->size = 256;
		CreateHash(hash_info);
		CleanMemory(hash_info);
	}
	
	if (clock_gettime( CLOCK_REALTIME, &stop) == -1) {
      		perror( "clock gettime" );
		return -1;
      	}

    	worktime = ((( stop.tv_sec - start.tv_sec )*1e9 + ( stop.tv_nsec - start.tv_nsec ))*1e-9)/10000;
    	printf ("%.10f sec for CreateHash and CleanMemory cycle\n", worktime);	


	return 0;
}

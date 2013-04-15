all : func_val perf_stat

clean :
  rm -f *.o *.so *.gc* func perf

func_val : func_cov
	valgrind --tool=memcheck ./func 2>&1 | grep 'ERROR SUMMARY: 0 errors from 0 contexts'
	valgrind --tool=memcheck ./func 2>&1 | grep 'All heap blocks were freed -- no leaks are possible'
	valgrind --tool=helgrind ./func 2>&1 | grep 'ERROR SUMMARY: 0 errors from 0 contexts'

func_cov : func
	valgrind --tool=memcheck ./func
	gcov hash.c | grep 'Lines executed' | grep '100.00'

func : func.c libhash-cov.so
	gcc --coverage -g -Wall -o func func.c -Wl,-rpath . -L. -lhash-cov

perf_stat : perf
	./perf

perf : perf.c libhash.so
	gcc -O3 -Wall -lrt  -o perf perf.c -Wl,-rpath . -L. -lhash

libhash.so : hash.h hash.c
	gcc -O3 -c -Wall -Werror -fpic hash.c
	gcc -O3 -shared -o libhash.so hash.o


libhash-cov.so : hash.h hash.c
	gcc --coverage -c -g -Wall -Werror -fpic hash.c
	gcc --coverage -shared -g -o libhash-cov.so hash.o


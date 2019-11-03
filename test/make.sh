    gcc -o test-concurrent-set test-concurrent-set.c ../src/set.c ../src/concurrent-set.c  ../src/compare-int.c ../src/hash-int.c -I../src/ -lpthread

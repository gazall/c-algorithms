all:
    gcc -o test-current test-current-set.c ../src/set.c ../src/current-set.c  ../src/compare-int.c ../src/hash-int.c -I../src/ -lpthread

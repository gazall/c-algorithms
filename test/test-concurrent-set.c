#include "concurrent-set.h"
#include "hash-int.h"
#include "compare-int.h"
#include <unistd.h>
#include <stdio.h>

int data[6] = {1, 2, 3, 4, 5, 100};

/*
 *reader1 and reader2 should come into the rdlock in the same time
 */
void reader_do(void *arg) {
    int *t = (int *)arg;
    fprintf(stderr, "reader come, %i\n", *t);
    sleep(5);
}

void *reader1(void *arg) {
    int reader_id = 1;
    ConCurrentSet *set = (ConCurrentSet *)arg;
    concurrent_set_query_and_do(set, &data[0], reader_do, &reader_id);
}

void *reader2(void *arg) {
    int reader_id = 2;
    ConCurrentSet *set = (ConCurrentSet *)arg;
    concurrent_set_query_and_do(set, &data[1], reader_do, &reader_id);
}

void *reader3(void *arg) {
    int reader_id = 3;
    ConCurrentSet *set = (ConCurrentSet *)arg;
    concurrent_set_query_and_do(set, &data[5], reader_do, &reader_id);
}

void *writer1(void *arg) {
    ConCurrentSet *set = (ConCurrentSet *)arg;

    fprintf(stderr, "before writer 1\n");
    concurrent_set_insert(set, &data[2]);
    fprintf(stderr, "after writer 1\n");
}

void *writer2(void *arg) {
    ConCurrentSet *set = (ConCurrentSet *)arg;

    fprintf(stderr, "before writer 2\n");
    concurrent_set_remove(set, &data[0]);
    fprintf(stderr, "after writer 2\n");
}

void run_test() {
    ConCurrentSet *set = concurrent_set_new(int_hash, int_equal);
    if (set == NULL) {
        fprintf(stdout, "new set failed\n");
        return ;
    }
    fprintf(stdout, "new set ok\n");

    concurrent_set_insert(set, &data[0]);
    concurrent_set_insert(set, &data[1]);

    pthread_t tid[5];
    pthread_create(&tid[0], NULL, reader1, (void *)set);    
    pthread_create(&tid[1], NULL, reader2, (void *)set);    
    pthread_create(&tid[2], NULL, reader3, (void *)set);    

    sleep(1);
    //writer shoud come into wrlock after reader execute
    pthread_create(&tid[3], NULL, writer1, (void *)set);    
    pthread_create(&tid[4], NULL, writer2, (void *)set);    

    sleep(5);
    int r1 = concurrent_set_query(set, &data[0]);
    int r2 = concurrent_set_query(set, &data[1]);
    int r3 = concurrent_set_query(set, &data[2]);
    fprintf(stdout, "r1 = %d, r2 = %d, r3 = %d.\n", 
                        r1, r2, r3);

    int i = 0;
    for (; i < 5; i++) {
        pthread_join(tid[i], NULL);
    }
    concurrent_set_free(set);

    fprintf(stdout, "run test finish.\n");
}

int main() {
    run_test();
}

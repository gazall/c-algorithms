#ifndef ALGORITHM_concurrent_set_H
#define ALGORITHM_concurrent_set_H

#include "set.h"
#include <pthread.h>

typedef struct _ConCurrentSet {
    Set *set;
    pthread_rwlock_t lock;
}ConCurrentSet;

typedef void (*concurrent_set_query_do)(void *);

ConCurrentSet *concurrent_set_new(SetHashFunc hash_func, SetEqualFunc equal_func);
void concurrent_set_free(ConCurrentSet *set);
int concurrent_set_insert(ConCurrentSet *set, SetValue data);
int concurrent_set_query(ConCurrentSet *set, SetValue data);
void concurrent_set_query_and_do(ConCurrentSet *set, SetValue data, concurrent_set_query_do func, void *args);
int concurrent_set_remove(ConCurrentSet *set, SetValue data);


#endif

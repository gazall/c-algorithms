#ifndef ALGORITHM_CURRENT_SET_H
#define ALGORITHM_CURRENT_SET_H

#include "set.h"
#include <pthread.h>

typedef struct _ConCurrentSet {
    Set *set;
    pthread_rwlock_t lock;
}ConCurrentSet;

typedef void (*current_set_query_do)(void *);

ConCurrentSet *current_set_new(SetHashFunc hash_func, SetEqualFunc equal_func);
void current_set_free(ConCurrentSet *set);
int current_set_insert(ConCurrentSet *set, SetValue data);
int current_set_query(ConCurrentSet *set, SetValue data);
void current_set_query_and_do(ConCurrentSet *set, SetValue data, current_set_query_do func, void *args);
int current_set_remove(ConCurrentSet *set, SetValue data);


#endif

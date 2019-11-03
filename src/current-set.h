#ifndef ALGORITHM_CURRENT_SET_H
#define ALGORITHM_CURRENT_SET_H

#include "set.h"
#include <pthread.h>

typedef struct _CurrentSet {
    Set *set;
    pthread_rwlock_t lock;
}CurrentSet;

typedef void (*current_set_query_do)(void *);

CurrentSet *current_set_new(SetHashFunc hash_func, SetEqualFunc equal_func);
void current_set_free(CurrentSet *set);
int current_set_insert(CurrentSet *set, SetValue data);
int current_set_query(CurrentSet *set, SetValue data);
void current_set_query_and_do(CurrentSet *set, SetValue data, current_set_query_do func, void *args);
int current_set_remove(CurrentSet *set, SetValue data);


#endif

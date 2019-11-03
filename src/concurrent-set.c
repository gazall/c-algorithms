/*

Copyright (c) 2005-2008, Simon Howard

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 */

#include "concurrent-set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ConCurrentSet *current_set_new(SetHashFunc hash_func, SetEqualFunc equal_func) {
    ConCurrentSet *cs = (ConCurrentSet *)malloc(sizeof(ConCurrentSet)); 
    if (NULL == cs) {
        return NULL;
    }
    cs->set = set_new(hash_func, equal_func);
    if (NULL == cs->set) {
        return NULL;
    }
    int ret = pthread_rwlock_init(&cs->lock, NULL);
    if (ret != 0) {
        fprintf(stderr, "pthread_rwlock_init failed.");
        return NULL;
    }

    return cs;
}

void current_set_free(ConCurrentSet *set) {
    if (NULL == set) {
        return ;
    }

    set_free(set->set);
    pthread_rwlock_destroy(&set->lock);
    free(set);
}

int current_set_insert(ConCurrentSet *set, SetValue data) {
    int ret = 0;
    
    pthread_rwlock_wrlock(&set->lock);
    ret = set_insert(set->set, data); 
    pthread_rwlock_unlock(&set->lock);

    return ret;
}

/*
return 1, found. 0, not found.
*/
int current_set_query(ConCurrentSet *set, SetValue data) {
    int ret;

    pthread_rwlock_rdlock(&set->lock);
    ret = set_query(set->set, data);
    pthread_rwlock_unlock(&set->lock);

    return ret;
}

void current_set_query_and_do(ConCurrentSet *set, SetValue data, current_set_query_do func, void *args) {
    if (NULL == func) {
        return ;
    }

    pthread_rwlock_rdlock(&set->lock);
    int ret = set_query(set->set, data);
    if (ret != 0) { //found
        func(args);
    }
    pthread_rwlock_unlock(&set->lock);
}

int current_set_remove(ConCurrentSet *set, SetValue data) {
    int ret = 0;

    pthread_rwlock_wrlock(&set->lock);
    ret = set_remove(set->set, data);
    pthread_rwlock_unlock(&set->lock);

    return ret;
}



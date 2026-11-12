#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct elem {
    char *key;
    void *value;
    struct elem *next;
} elem_t;

typedef struct hashtable {
    elem_t **elems;
    size_t capacity;
    size_t count;
} hashtable_t;

hashtable_t *hashtable_create(size_t capacity);
bool hashtable_insert(hashtable_t *table, const char *key, void *value);
void *hashtable_search(hashtable_t *table, const char *key);
void hashtable_free(hashtable_t *table);

#endif

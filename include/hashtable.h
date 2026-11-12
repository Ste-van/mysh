#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>

typedef void (*hashtable_free_fn)(void *value);
typedef void (*hashtable_iter_fn)(const char *key, void *value, void *ctx);

typedef struct elem {
    char *key;
    void *value;
    struct elem *next;
} elem_t;

typedef struct hashtable {
    elem_t **elems;
    size_t capacity;
    size_t count;
    hashtable_free_fn free_value;
} hashtable_t;

hashtable_t *hashtable_create(size_t capacity, hashtable_free_fn free_value);
bool hashtable_insert(hashtable_t *table, const char *key, void *value);
bool hashtable_remove(hashtable_t *table, const char *key);
void *hashtable_search(hashtable_t *table, const char *key);
void hashtable_clear(hashtable_t *table);
void hashtable_foreach(hashtable_t *table, hashtable_iter_fn fn, void *ctx);
void hashtable_free(hashtable_t *table);

#endif

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>

/****************************************************************************************
 *
 *  WARNING: Gemini suggested me to use a hashtable for the built-in commands.
 *           Then I spent 4 hours implementing this hashtable just to hear
 *           from Gemini that I was supposed to use a static array for built-ins.
 *
 *           Then I spent 2 hours during lectures refactoring the implementation so
 *           that it serves the real purpose of a hashtable in a shell.
 *
 *           The refactoring was done in a moment of rage (I hate wasting time)
 *           so I'm not real sure about the correctness of the implementation.
 *
 *           I will surely revise this code when i will need it.
 *
 *  The hashtable will be used to:
 *  - manage shell/environment variables
 *  - hash built-in command
 *  - aliases
 *  - shell functions
 *
*****************************************************************************************/

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

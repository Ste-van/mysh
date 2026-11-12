#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

static size_t hash_djb2(const char *str) {
    size_t hash = 5381;
    int c;

    c = *str;
    while (c != '\0') {
        // hash = (hash * 33) + c;
        hash = ((hash << 5) + hash) + c; //bit-wise optimization
        str++;
        c = *str;
    }

    return hash;
}

hashtable_t *hashtable_create(size_t capacity) {
    hashtable_t *table = malloc(sizeof(hashtable_t));
    if (table == NULL)
        return NULL;

    table->elems = calloc(capacity, sizeof(elem_t *));
    if (table->elems == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;

    return table;
}

bool hashtable_insert(hashtable_t *table, const char *key, void *value) {

    if (table == NULL || key == NULL)
        return false;

    size_t hash = hash_djb2(key);
    size_t index = hash % table->capacity;

    // Case 1: the element is not in the table
    elem_t *elem = table->elems[index];
    while (elem != NULL) {
        if (strcmp(elem->key, key) == 0) {
            elem->value = value;
            return true;
        }

        elem = elem->next;
    }

    // Case 2: element not found, create new elem
    elem_t *new_elem = malloc(sizeof(elem_t));
    if (new_elem == NULL) 
        return false;

    new_elem->key = strdup(key);
    if (new_elem->key == NULL) {
        free(new_elem);
        return false;
    }

    new_elem->value = value;

    new_elem->next = table->elems[index];
    table->elems[index] = new_elem;

    table->count++;

    return true;
}

void *hashtable_search(hashtable_t *table, const char *key) {
    if (table == NULL || key == NULL)
        return NULL;

    size_t hash = hash_djb2(key);
    size_t index = hash % table->capacity;

    elem_t *elem = table->elems[index];

    while (elem != NULL) {
        if (strcmp(elem->key, key) == 0) {
            return elem->value;
        }

        elem = elem -> next;
    }

    return NULL;
}

void hashtable_free(hashtable_t *table) {
    if (table == NULL)
        return;

    for (size_t i = 0; i < table->capacity; i++) {
        elem_t *current = table->elems[i];

        while (current != NULL) {
            elem_t *next = current->next;

            free(current->key);
            free(current);

            current = next;
        }
    }

    free(table->elems);
    free(table);
}

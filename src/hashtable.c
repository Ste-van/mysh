#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hashtable.h"

static size_t hash_djb2(const char *str) {
    size_t hash = 5381;
    const unsigned char *c = (const unsigned char *)str;

    while (*c != '\0') {
        // hash = (hash * 33) + c;
        hash = ((hash << 5) + hash) + *c; //bit-wise optimization
        c++;
    }

    return hash;
}

hashtable_t *hashtable_create(size_t capacity, hashtable_free_fn free_value) {
    if (capacity < 8) {
        capacity = 8;
    }

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
    table->free_value = free_value;

    return table;
}

static bool hashtable_resize(hashtable_t *table, size_t new_capacity) {
    elem_t **elems_copy = calloc(new_capacity, sizeof(elem_t *));
    if (elems_copy == NULL)
        return false;

    for (size_t i = 0; i < table->capacity; i++) {
        elem_t *tmp = table->elems[i];
        while (tmp != NULL) {
            elem_t *next = tmp->next;
            size_t new_idx = hash_djb2(tmp->key) % new_capacity;
            tmp->next = elems_copy[new_idx];
            elems_copy[new_idx] = tmp;
            tmp = next;
        }
    }

    free(table->elems);
    table->elems = elems_copy;
    table->capacity = new_capacity;
    return true;
}

bool hashtable_insert(hashtable_t *table, const char *key, void *value) {
    if (table == NULL || key == NULL)
        return false;

    size_t index = hash_djb2(key) % table->capacity;

    // Case 1: the element is in the table
    elem_t *elem = table->elems[index];
    while (elem != NULL) {
        if (strcmp(elem->key, key) == 0) {
            if (elem->value != value && table->free_value != NULL)
                table->free_value(elem->value);
            elem->value = value;
            return true;
        }

        elem = elem->next;
    }

    // if load factor > 0.75 -> resize table (duplicate capacity)
    if (table->count >= table->capacity / 4 * 3) {
        if (table->capacity <= SIZE_MAX / 2) {
            if (hashtable_resize(table, table->capacity * 2))
                index = hash_djb2(key) % table->capacity;
        }
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

    size_t index = hash_djb2(key) % table->capacity;

    elem_t *elem = table->elems[index];

    while (elem != NULL) {
        if (strcmp(elem->key, key) == 0) {
            return elem->value;
        }

        elem = elem -> next;
    }

    return NULL;
}

bool hashtable_remove(hashtable_t *table, const char *key) {
    if (table == NULL || key == NULL)
        return false;

    size_t index = hash_djb2(key) % table->capacity;
    elem_t **link = &table->elems[index];

    while (*link != NULL) {
        elem_t *tmp = *link;
        if (strcmp(tmp->key, key) == 0) {
            *link = tmp->next;
            if (table->free_value != NULL)
                table->free_value(tmp->value);
            free(tmp->key);
            free(tmp);
            table->count--;
            return true;
        }
        link = &tmp->next;
    }

    return false;
}

void hashtable_clear(hashtable_t *table) {
    if (table == NULL)
        return;

    for (size_t i = 0; i < table->capacity; i++) {
        elem_t *elem = table->elems[i];
        while (elem != NULL) {
            elem_t *next = elem->next;
            if (table->free_value != NULL)
                table->free_value(elem->value);
            free(elem->key);
            free(elem);
            elem = next;
        }
        table->elems[i] = NULL;
    }
    table->count = 0;
}

void hashtable_foreach(hashtable_t *table, hashtable_iter_fn fn, void *ctx) {
    if (table == NULL || fn == NULL)
        return;

    for (size_t i = 0; i < table->capacity; i++) {
        elem_t *elem = table->elems[i];
        while(elem != NULL) {
            fn(elem->key, elem->value, ctx);
            elem = elem->next;
        }
    }
}

void hashtable_free(hashtable_t *table) {
    if (table == NULL)
        return;
    hashtable_clear(table);
    free(table->elems);
    free(table);
}

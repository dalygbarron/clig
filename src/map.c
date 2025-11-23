#include "../include/ds.h"
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAP_INIT_SIZE 16
#define BUCKET_SIZE(m) (sizeof(char const *) + m->element_size)

struct clig_map_t_ {
    uint8_t *buffer;
    size_t element_size;
    size_t n;
    size_t capacity;
};

static size_t hash_func(char const *value) {
    size_t width = CHAR_BIT * sizeof(size_t);
    size_t h = 0;
    for (int i = 0; value[i]; i++) {
        h = (h << 9) | (h & (0b111111111 << (width - 9)) >> (width - 9));
        h = h ^ value[i];
    }
    return h;
}

static void insert_bucket(clig_map_t *m, char const *key, void *value) {
    // we assume that the key we receive here can be held onto.
    size_t index = hash_func(key) % m->capacity;
    size_t bucket_size = sizeof(char const *) + m->element_size;
    for (int i = index; i < index + m->capacity; i++) {
        index = i % m->capacity;
        char const *local_key = (char const *)(m->buffer + BUCKET_SIZE(m) * i);
        if (local_key == NULL) {
            memcpy(m->buffer + BUCKET_SIZE(m) * i, &key, sizeof(char const *));
            memcpy(
                m->buffer + BUCKET_SIZE(m) * i + sizeof(char const *),
                value,
                sizeof(char const *)
            );
            return;
        }
    }
    // map should never be full.
    assert(0);
}

static void grow_map(clig_map_t *m) {
    size_t old_capacity = m->capacity;
    uint8_t *old_buffer = m->buffer;
    m->capacity *= 2;
    m->buffer = malloc(BUCKET_SIZE(m) * m->capacity);
    for (int i = 0; i < old_capacity; i++) {
        char const *key = (char const *)(old_buffer + i * BUCKET_SIZE(m));
        if (!key) continue;
        void *value = &old_buffer[i * BUCKET_SIZE(m) + sizeof(char const *)];
        insert_bucket(m, key, value);
    }
}

clig_map_t *clig_map_make(size_t element_size) {
    clig_map_t *m = malloc(sizeof(clig_map_t));
    m->element_size = element_size;
    m->n = 0;
    m->buffer = malloc((sizeof(char const *) + element_size) * MAP_INIT_SIZE);
    m->capacity = MAP_INIT_SIZE;
    return m;
}

void clig_map_insert(clig_map_t *m, char const *key, void const *src) {
    if (m->n >= m->capacity / 4 * 3) grow_map(m);
    // TODO: before insert we need to consider growing and rehashing because
    // doing so afterwards would be stupid.
    size_t index = hash_func(key) % m->capacity;
    for (int i = index; i < index + m->capacity; i++) {
        index = i % m->capacity;
        // if (m->buckets[index].key == NULL) {
        //     bucket_insert(m->buckets + index, key, src);
        // }
    }
    // map should never be full.
    assert(0);
}

int clig_map_get(clig_map_t *m, char const *key, void *dst) {
    return 0;
}

size_t clig_map_size(clig_map_t const *m) {
    return m->n;
}

void clig_map_free(clig_map_t *m) {
    free(m->buffer);
    free(m);
}
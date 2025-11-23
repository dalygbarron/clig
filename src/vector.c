#include "../include/ds.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BUF_INIT_SIZE 32

struct clig_vector_t_ {
    uint8_t *buffer;
    size_t element_size;
    size_t n;
    size_t capacity;
};

static void remake_buffer(clig_vector_t *v) {
    void *new_buffer = malloc(v->element_size * v->capacity);
    if (new_buffer == NULL) abort();
    memcpy(new_buffer, v->buffer, v->element_size * v->n);
    free(v->buffer);
    v->buffer = new_buffer;
}

static void check_size(clig_vector_t *v) {
    // if size is equal to capacity, we double capacity.
    // if size is a quarter or less of capacity and capacity is greater than the
    // minimum we halve it.
    size_t current_capacity = v->capacity;
    if (v->n == v->capacity) {
        v->capacity *= 2;
    } else if (v->n <= v->capacity / 4 && v->capacity > BUF_INIT_SIZE) {
        v->capacity /= 2;
    }
    if (v->capacity != current_capacity) remake_buffer(v);
}

static void move_item(clig_vector_t *v, size_t from, size_t to) {
    memcpy(
        v->buffer + v->element_size * to,
        v->buffer + v->element_size * from,
        v->element_size
    );
}

clig_vector_t *clig_vector_make(size_t element_size) {
    clig_vector_t *v = (clig_vector_t *)malloc(sizeof(clig_vector_t));
    if (v == NULL) return NULL;
    v->buffer = malloc(element_size * BUF_INIT_SIZE);
    if (v->buffer == NULL) {
        free(v);
        return NULL;
    }
    v->element_size = element_size;
    v->n = 0;
    v->capacity = BUF_INIT_SIZE;
    return v;
}

void clig_vector_insert(clig_vector_t *v, void const *item, size_t index) {
    assert(v != NULL);
    assert(item != NULL);
    assert(index <= v->n);
    if (index != v->n) move_item(v, index, v->n);
    memcpy(
        v->buffer + v->element_size * index,
        item,
        v->element_size
    );
    v->n++;
    check_size(v);
}

void clig_vector_remove(clig_vector_t *v, size_t index) {
    assert(v != NULL);
    assert(index < v->n);
    if (index != v->n - 1) move_item(v, v->n - 1, index);
    v->n--;
    check_size(v);
}

void clig_vector_push(clig_vector_t *v, void const *item) {
    assert(v != NULL);
    assert(item != NULL);
    memcpy(
        v->buffer + v->n * v->element_size,
        item,
        v->element_size
    );
    v->n++;
    check_size(v);
}

void clig_vector_pop(clig_vector_t *v, void *dst) {
    assert(v != NULL);
    assert(v->n > 0);
    if (dst != NULL) {
        memcpy(
            dst,
            v->buffer + (v->n - 1) * v->element_size,
            v->element_size
        );
    }
    v->n--;
    check_size(v);
}

void clig_vector_peek(clig_vector_t *v, size_t index, void *dst) {
    assert(v != NULL);
    assert(index < v->n);
    memcpy(
        dst,
        v->buffer + index * v->element_size,
        v->element_size
    );
}

size_t clig_vector_size(clig_vector_t const *v) {
    assert(v != NULL);
    return v->n;
}

void clig_vector_trim(clig_vector_t *v, size_t margin) {
    assert(v != NULL);
    if (margin < 1) margin = 1;
    v->capacity = v->n + margin;
    if (v->capacity < BUF_INIT_SIZE) v->capacity = BUF_INIT_SIZE;
    remake_buffer(v);
}

void *clig_vector_get_buffer(clig_vector_t *v) {
    assert(v != NULL);
    return v->buffer;
}

void clig_vector_free(clig_vector_t *v) {
    assert(v != NULL);
    free(v->buffer);
    free(v);
}
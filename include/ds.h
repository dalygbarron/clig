// ds.h
// Kind of irrelevant but the implementation needs some data structures so I
// figure I might as well make them available to consumers of the library as
// well.

#pragma once

#include <stddef.h>

typedef struct clig_vector_t_ clig_vector_t;
typedef struct clig_map_t_ clig_map_t;

// vector functions.
clig_vector_t *clig_vector_make(size_t element_size);
void clig_vector_insert(clig_vector_t *v, void const *item, size_t index);
void clig_vector_remove(clig_vector_t *v, size_t index);
void clig_vector_push(clig_vector_t *v, void const *item);
void clig_vector_pop(clig_vector_t *v, void *dst);
void clig_vector_peek(clig_vector_t *v, size_t index, void *dst);
size_t clig_vector_size(clig_vector_t const *v);
void clig_vector_trim(clig_vector_t *v, size_t margin);
void *clig_vector_get_buffer(clig_vector_t *v);
void clig_vector_free(clig_vector_t *v);

// map functions.
clig_map_t *clig_map_make(size_t element_size);
void clig_map_insert(clig_map_t *m, char const *key, void const *src);
int clig_map_get(clig_map_t *m, char const *key, void *dst);
size_t clig_map_size(clig_map_t const *m);
void clig_map_free(clig_map_t *m);
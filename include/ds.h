// ds.h
// Kind of irrelevant but the implementation needs some data structures so I
// figure I might as well make them available to consumers of the library as
// well.

#pragma once

#include <stddef.h>



typedef struct _clig_vector_t clig_vector_t;

clig_vector_t *clig_make_vector(size_t element_size);
void clig_vector_insert(clig_vector_t *v, void *item, size_t index);
void clig_vector_remove(clig_vector_t *v, size_t index);
void clig_vector_push(clig_vector_t *v, void *item);
void clig_vector_pop(clig_vector_t *v, void *dst);
void clig_vector_peek(clig_vector_t *v, size_t index, void *dst);
size_t clig_vector_size(clig_vector_t *v);
void clig_vector_trim(clig_vector_t *v, size_t margin);
void *clig_vector_get_buffer(clig_vector_t *v);
void clig_vector_free(clig_vector_t *v);
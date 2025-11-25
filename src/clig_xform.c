#include "../include/clig.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define CACHE_SIZE 4

// The cache consists of a fixed size array of elements with string keys, void
// pointers to data, and a doubly linked list which keeps track of the relative
// age of items in the cache, if newer pointer is NULL then it is the newest
// object and if older pointer is NULL it is oldest.
// On the actual xform object which manages the cache we also keep track of the
// oldest and newest item so that we can find them easily.
// The only important operation on the cache is trying to find something,
// which also involves deleting the oldest thing if there isn't enough space,
// and updating the sequence of oldness.
// So our first step is trying to find it. If that works, we update it to be
// the newest and then return it, and if not, we find the oldest, free it,
// insert the data, and then use that same operation to make it the newest.
// How we update something to be the newest:
//   - if already newest, abort.
//   - Make old newest point to this object as newer.
//   - wire up old pointers on found object to remove it, respecting NULLs.
//   - if this object was oldest make it's newer the oldest now.
//   - Give found object new pointers for older and newer being NULL.
//   - set cache pointer for newest to this object.
// The specificity of this ordering is just to make sure the needed pointers
// are there when we need them.

typedef struct cache_bucket_t_ cache_bucket_t;

struct cache_bucket_t_ {
    char *key;
    void *data;
    cache_bucket_t *older;
    cache_bucket_t *newer;
};

struct clig_xform_t_{
    clig_xform_func make_func;
    clig_xform_free_func free_func;
    cache_bucket_t *oldest;
    cache_bucket_t *newest;
    cache_bucket_t buckets[CACHE_SIZE];
};

static void make_newest(clig_xform_t *x, cache_bucket_t *b) {
    if (x->newest == b) return;
    // if it's not the newest it should always have something newer.
    assert(b->newer);
    x->newest->newer = b;
    b->newer->older = b->older;
    if (b->older == NULL) x->oldest = b->newer;
    else b->older->newer = b->newer;
    b->newer = NULL;
    b->older = x->newest;
    x->newest = b;
}

clig_xform_t *clig_xform_make(
    clig_xform_func xform_func,
    clig_xform_free_func free_func
) {
    clig_xform_t *x = malloc(sizeof(clig_xform_t));
    if (!x) return NULL;
    x->make_func = xform_func;
    x->free_func = free_func;
    x->newest = x->buckets;
    x->oldest = x->buckets + CACHE_SIZE - 1;
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache_bucket_t *b = x->buckets + i;
        b->key = NULL;
        b->data = NULL;
        if (i > 0) b->newer = x->buckets + i - 1;
        else b->newer = NULL;
        if (i < CACHE_SIZE - 1) b->older = x->buckets + i + 1;
        else b->older = NULL;
    }
    return x;
}

void clig_xform_free(clig_xform_t *x) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache_bucket_t *b = x->buckets + i;
        if (b->key) free(b->key);
        if (b->data) x->free_func(b->data);
    }
    free(x);
}

void const *clig_xform_get(clig_xform_t *x, char const *value) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache_bucket_t *b = x->buckets + i;
        if (b->key && strcmp(b->key, value) == 0) {
            make_newest(x, b);
            return b->data;
        }
    }
    // No matches. Insert.
    cache_bucket_t *insert = x->oldest;
    if (insert->key) free(insert->key);
    if (insert->data) x->free_func(insert->data);
    insert->key = strdup(value);
    insert->data = x->make_func(value);
    make_newest(x, insert);
    return insert->data;
}

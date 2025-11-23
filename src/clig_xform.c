#include "../include/clig.h"

#define CACHE_SIZE 5

// This design sucks, we don't need to be able to access them by a string name
// and fuck around with a hashmap blah blah blah when we could just make it a
// type and create instances of it. The caching stuff is still necessary but
// yeah.

typedef struct {
    char const *key;
    void *data;
    cache_bucket *older;
    cache_bucket *newer;
} cache_bucket_t;

typedef struct {
    clig_xform_func make_func;
    clig_xform_free free_func;
    cache_bucket items[CACHE_SIZE];
} xform_cache_t;

static sc_map_sv xforms;

void clig_xform_register(
    char const *xform_name,
    clig_xform_func xform_func,
    clig_xform_free free_func
) {
    xform_cache *xform = malloc(sizeof(xform_cache_t));
    if (!xform) {
        fprintf(stderr, "Out of memory something, goodbye");
        abort(1);
    }
    sc_map_put_sv(&xforms, xform_name, xform);
}

void clig_xform_deregister(char const *xform_name) {

}

void const *clig_xform_get(char const *xform_name, char const *value) {

}


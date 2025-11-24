// clig.h
// Module for CLI and GUI interaction.

#pragma once

#include <stddef.h>

// type definitions.
typedef enum {
    clig_OK,
    clig_ERROR,
    clig_FATAL,
    clig_SHUTDOWN
} clig_input_result;
typedef struct clig_xform_t_ clig_xform_t;
typedef clig_input_result (*clig_input_handler)(char const *);
typedef void *(*clig_xform_func)(char const *);
typedef void (*clig_xform_free_func)(void *);

// Primarily REPL related functions.
int clig_main(int argc, char const **argv);
void clig_add_input_handler(char const *prefix, clig_input_handler cb);

// Primarily master related functions.

// Text transformer functions.
clig_xform_t *clig_xform_make(
    clig_xform_func xform_func,
    clig_xform_free_func free_func
);
void clig_xform_free(clig_xform_t *xform);
void const *clig_xform_get(clig_xform_t *xform, char const *value);

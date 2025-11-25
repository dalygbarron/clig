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
typedef void (*clig_prompt)();
typedef void *(*clig_xform_func)(char const *);
typedef void (*clig_xform_free_func)(void *);

// Primarily REPL related functions.
void clig_repl_add_input_handler(char const *prefix, clig_input_handler cb);
void clig_repl_set_info(
    char const *name,
    char const *version,
    char const *help,
    clig_prompt prompt
);
int clig_repl_main(int argc, char **argv);

// Primarily master related functions.

// Text transformer functions.
clig_xform_t *clig_xform_make(
    clig_xform_func xform_func,
    clig_xform_free_func free_func
);
void clig_xform_free(clig_xform_t *xform);
void const *clig_xform_get(clig_xform_t *xform, char const *value);

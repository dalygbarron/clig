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
typedef clig_input_result (*clig_input_handler)(char const *);
typedef void (*clig_xform_func)(char const *);
typedef void (*clig_xform_free)(void *);
typedef struct clig_vector_t_ clig_vector_t;
typedef struct clig_map_t_ clig_map_t;

// REPL related functions.
int clig_main(int argc, char const **argv);
void clig_add_input_handler(char const *prefix, clig_input_handler cb);

// General functions.
void clig_xform_register(
    char const *xform_name,
    clig_xform_func xform,
    clig_xform_free free
);
void clig_xform_deregister(char const *xform_name);
void const *clig_xform_get(char const *xform_name, char const *value);

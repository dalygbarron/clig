// clig.h
// Module for CLI and GUI interaction.

#pragma once

typedef enum {
    clig_OK,
    clig_ERROR,
    clig_FATAL,
    clig_SHUTDOWN
} clig_input_result;

typedef clig_input_result (*clig_input_handler)(char const *);

void clig_add_input_handler(char const *prefix, clig_input_handler cb);
int clig_main(int argc, char const **argv);
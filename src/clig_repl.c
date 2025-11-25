#include <clig.h>
#include <getopt.h>
#include <assert.h>
#include <stdio.h>
#include "sc_map.h"
#include "internal.h"

#define MAX_LINE 1024

static char const *repl_name = "Unnamed CLIG Project";
static char const *repl_version = "0.1.0";
static char const *repl_help = "No help defined.";
clig_prompt repl_prompt = NULL;
int initialised = 0;
static struct sc_map_sih input_handlers;

static clig_input_result handle_input(char const *input) {
    char token_buffer[MAX_TOKEN];
    char const *post_token_input = read_token(input, token_buffer);
    if (!post_token_input) {
        printf("Initial command too long\n");
        return clig_ERROR;
    }
    clig_input_handler cb = sc_map_get_sih(&input_handlers, token_buffer);
    if (cb == NULL) {
        printf("Unknown command %s\n", token_buffer);
        return clig_ERROR;
    }
    return cb(post_token_input);
}

void clig_repl_add_input_handler(const char *prefix, clig_input_handler cb) {
    if (!initialised) {
        sc_map_init_sih(&input_handlers, 0, 0);
        initialised = 1;
    }
    sc_map_put_sih(&input_handlers, prefix, cb);
}

void clig_repl_set_info(
    const char *name,
    const char *version,
    const char *help,
    clig_prompt prompt
) {
    assert(name && version && help && prompt);
    repl_name = name;
    repl_version = version;
    repl_help = help;
    repl_prompt = prompt;
}

int clig_repl_main(int argc, char **argv) {
    int help = 0;
    int version = 0;
    int use_prompt = 1;
    int opt;
    while ((opt = getopt(argc, argv, "vhP")) != -1) {
        switch (opt) {
            case 'v':
                version = 1;
                break;
            case 'h':
                help = 1;
                break;
            case 'P':
                use_prompt = 0;
            default:
                help = 1;
        }
    }
    if (help) {
        printf("%s - %s\n", repl_name, repl_version);
        printf("%s\n", repl_help);
        return 0;
    }
    if (version) {
        printf("%s - %s\n", repl_name, repl_version);
        return 0;
    }
    int keep_going = 1;
    while (keep_going) {
        char line_buffer[MAX_LINE];
        if (use_prompt) repl_prompt();
        char const *good_read = fgets(line_buffer, MAX_LINE, stdin);
        if (!good_read) break;
        int need_format = line_buffer[strlen(line_buffer) - 1] != '\n';
        if (need_format) printf("\n");
        clig_input_result result = handle_input(line_buffer);
        switch (result) {
        case clig_OK:
            break;
        case clig_ERROR:
            break;
        case clig_FATAL:
            keep_going = 0;
            break;
        case clig_SHUTDOWN:
            keep_going = 0;
            break;
        }
        if (need_format) printf("\n");
    }
    return 0;
}
#include <clig.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "internal.h"

char const *read_token(char const *input, char *dst) {
    assert(input && dst);
    char const *start = NULL;
    char const *end = NULL;
    char const *real_end = NULL;
    for (char const *i = input; *i; i++) {
        if (!start && !isspace(*i)) start = i;
        if (start) {
            real_end = i;
            if (i - start >= MAX_TOKEN) return NULL;
            if (!end && isspace(*i)) {
                end = i;
                break;
            }
        }
    }
    if (!start) return NULL;
    if (!end) end = real_end + 1;
    memcpy(dst, start, end - start);
    dst[end - start] = 0;
    while (*end && isspace(*end)) end++;
    return end;
}
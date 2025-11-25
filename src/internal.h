#pragma once

#define MAX_TOKEN 128

// Takes an input string and a destination buffer, and writes everything from
// the first non-whitespace character to the next whitespace character
// (exclusive) into the buffer and adds a null terminator on the end, unless all
// that is longer than MAX_TOKEN in which case it will do nothing.
// Return value is the next non whitespace character if there is any, or the end
// of the string if not, and it will be NULL if it couldn't fit it all in dst.
char const *read_token(char const *input, char *dst);
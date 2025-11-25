#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <clig.h>
#include "internal.h"

static void test_read_token(void) {
    char buffer[MAX_TOKEN];
    // basic.
    char const *next = read_token(" hello matey", buffer);
    CU_ASSERT_STRING_EQUAL(buffer, "hello");
    CU_ASSERT_EQUAL(*next, 'm');
    // no next token.
    next = read_token(" hello      ", buffer);
    CU_ASSERT_STRING_EQUAL(buffer, "hello");
    CU_ASSERT_EQUAL(*next, 0);
    // endless and long token.
    char *long_ass_string = malloc(sizeof(char) * MAX_TOKEN * 2);
    memset(long_ass_string, 'L', MAX_TOKEN * 2  - 1);
    long_ass_string[MAX_TOKEN * 2  - 1] = 0;
    next = read_token(long_ass_string, buffer);
    CU_ASSERT_STRING_EQUAL(buffer, "hello");
    CU_ASSERT_PTR_NULL(next);
    // very long token that does end.
    memset(long_ass_string + MAX_TOKEN + 5, ' ', MAX_TOKEN  - 6);
    next = read_token(long_ass_string, buffer);
    CU_ASSERT_STRING_EQUAL(buffer, "hello");
    CU_ASSERT_PTR_NULL(next);
    free(long_ass_string);
    // empty string.
    next = read_token("", buffer);
    CU_ASSERT_STRING_EQUAL(buffer, "hello");
    CU_ASSERT_PTR_NULL(next);
    // just token.
    next = read_token("funny", buffer);
    CU_ASSERT_STRING_EQUAL(buffer, "funny");
    CU_ASSERT_EQUAL(*next, 0);
}

int add_clig_tests(void) {
    CU_pSuite suite = CU_add_suite("clig_tests", NULL, NULL);
    CU_BOOL success = suite &&
        CU_ADD_TEST(suite, test_read_token);
    if (!success) return CU_get_error();
    return CUE_SUCCESS;
}
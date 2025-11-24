#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <clig.h>

#define TEST_STRINGS \
    X("Hello", "olleH") \
    X("apple", "elppa") \
    X("I have a Big Hernia!", "!ainreH giB a evah I") \
    X("X", "X") \
    X("123456", "654321") \
    X("321321", "123123") \
    X("Gaming Big Time", NULL) \
    X("loop", "pool") \
    X("", "") \
    X("+-+", "+-+")

static char *reverse_text(char const *input) {
    if (strcmp(input, "Gaming Big Time") == 0) return NULL;
    size_t size = strlen(input);
    char *buffer = malloc(sizeof(char) * (size + 1));
    buffer[size] = 0;
    for (size_t i = 0; i < size; i++) {
        buffer[i] = input[size - i - 1];
    }
    return buffer;
}

static void test_xform_get(void) {
    clig_xform_t *x = clig_xform_make((clig_xform_func)reverse_text, free);
    CU_ASSERT_PTR_NOT_NULL(x);
    for (int i = 0; i < 3; i++) {
        #define X(normal, reverse) \
        if (reverse) { \
            CU_ASSERT_STRING_EQUAL(reverse, clig_xform_get(x, normal)); \
        } else { \
            CU_ASSERT_PTR_EQUAL(reverse, clig_xform_get(x, normal)); \
        }
        TEST_STRINGS
        #undef X
    }
    clig_xform_free(x);
}

int add_xform_tests(void) {
    CU_pSuite suite = CU_add_suite("xform_tests", NULL, NULL);
    CU_BOOL success = suite &&
        CU_ADD_TEST(suite, test_xform_get);
    if (!success) return CU_get_error();
    return CUE_SUCCESS;
}

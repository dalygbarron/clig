#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/ds.h"

static clig_vector_t *make_int_vector(void) {
    clig_vector_t *v = clig_vector_make(sizeof(int));
    CU_ASSERT_PTR_NOT_NULL_FATAL(v);
    return v;
}

static void test_make_and_free(void) {
    clig_vector_t *v = clig_vector_make(sizeof(int));
    CU_ASSERT_PTR_NOT_NULL_FATAL(v);
    CU_ASSERT_EQUAL(clig_vector_size(v), 0);
    clig_vector_free(v);
}

static void test_push_and_peek_int(void) {
    clig_vector_t *v = make_int_vector();
    for (size_t i = 0; i < 10; ++i) {
        clig_vector_push(v, &i);
        CU_ASSERT_EQUAL(clig_vector_size(v), i + 1);
    }
    for (size_t i = 0; i < 10; ++i) {
        int value = -1;
        clig_vector_peek(v, i, &value);
        CU_ASSERT_EQUAL((size_t)value, i);
    }
    clig_vector_free(v);
}

static void test_pop_with_and_without_dst(void) {
    clig_vector_t *v = make_int_vector();
    int values[5];
    for (size_t i = 0; i < 5; ++i) {
        values[i] = (int)(i * 10);
        clig_vector_push(v, &values[i]);
    }
    CU_ASSERT_EQUAL(clig_vector_size(v), 5);
    // with dst.
    for (size_t i = 0; i < 3; ++i) {
        int out = -1;
        clig_vector_pop(v, &out);
        CU_ASSERT_EQUAL(out, values[4 - i]);
        CU_ASSERT_EQUAL(clig_vector_size(v), 5 - (i + 1));
    }
    // without dst.
    while (clig_vector_size(v) > 0) clig_vector_pop(v, NULL);
    CU_ASSERT_EQUAL(clig_vector_size(v), 0);
    clig_vector_free(v);
}

static void test_insert_swap_semantics(void) {
    clig_vector_t *v = make_int_vector();
    int a = 1, b = 2, c = 3, d = 4;
    int out;
    clig_vector_push(v, &a);
    clig_vector_push(v, &b);
    clig_vector_push(v, &c);
    CU_ASSERT_EQUAL(clig_vector_size(v), 3);
    clig_vector_insert(v, &d, 1);
    CU_ASSERT_EQUAL(clig_vector_size(v), 4);
    clig_vector_peek(v, 0, &out);
    CU_ASSERT_EQUAL(out, 1);
    clig_vector_peek(v, 1, &out);
    CU_ASSERT_EQUAL(out, 4);
    clig_vector_peek(v, 2, &out);
    CU_ASSERT_EQUAL(out, 3);
    clig_vector_peek(v, 3, &out);
    CU_ASSERT_EQUAL(out, 2);
    clig_vector_free(v);
}

static void test_remove_swap_semantics(void) {
    clig_vector_t *v = make_int_vector();
    int a = 10, b = 20, c = 30, d = 40;
    int out;
    clig_vector_push(v, &a);
    clig_vector_push(v, &b);
    clig_vector_push(v, &c);
    clig_vector_push(v, &d);
    clig_vector_remove(v, 1);
    CU_ASSERT_EQUAL(clig_vector_size(v), 3);
    clig_vector_peek(v, 0, &out);
    CU_ASSERT_EQUAL(out, 10);
    clig_vector_peek(v, 1, &out);
    CU_ASSERT_EQUAL(out, 40);
    clig_vector_peek(v, 2, &out);
    CU_ASSERT_EQUAL(out, 30);
    clig_vector_free(v);
}

static void test_trim_and_buffer_layout(void) {
    clig_vector_t *v = make_int_vector();
    int out;
    for (size_t i = 0; i < 50; ++i) {
        int value = i;
        clig_vector_push(v, &value);
    }
    CU_ASSERT_EQUAL(clig_vector_size(v), 50);
    // buffers should be ok before and after trim, although not equal.
    int *buf_before = (int *)clig_vector_get_buffer(v);
    CU_ASSERT_PTR_NOT_NULL(buf_before);
    // trim with margin zero should be same as if margin was 1.
    clig_vector_trim(v, 0);
    int *buf_after = (int *)clig_vector_get_buffer(v);
    CU_ASSERT_PTR_NOT_NULL(buf_after);
    CU_ASSERT_NOT_EQUAL(buf_before, buf_after);
    // Elements should be the same and contiguous.
    for (size_t i = 0; i < 50; ++i) {
        clig_vector_peek(v, i, &out);
        CU_ASSERT_EQUAL(out, (int)i);
        CU_ASSERT_EQUAL(buf_after[i], (int)i);
    }
    // pushing after trim should be fine.
    for (size_t i = 50; i < 55; ++i) {
        int value = (int)i;
        clig_vector_push(v, &value);
    }
    CU_ASSERT_EQUAL(clig_vector_size(v), 55);
    clig_vector_free(v);
}

static void test_large_resize_sequence(void) {
    clig_vector_t *v = make_int_vector();
    int out;
    // push a heap of stuff and force resizes.
    for (size_t i = 0; i < 1000; ++i) {
        int value = (int)i;
        clig_vector_push(v, &value);
    }
    CU_ASSERT_EQUAL(clig_vector_size(v), 1000);
    // verify some samples.
    clig_vector_peek(v, 0, &out);
    CU_ASSERT_EQUAL(out, 0);
    clig_vector_peek(v, 500, &out);
    CU_ASSERT_EQUAL(out, 500);
    clig_vector_peek(v, 999, &out);
    CU_ASSERT_EQUAL(out, 999);
    // pop a heap, forcing shrink, check they are in the correct order.
    for (size_t i = 0; i < 500; ++i) {
        clig_vector_pop(v, &out);
        CU_ASSERT_EQUAL(out, (int)(999 - i));
    }
    CU_ASSERT_EQUAL(clig_vector_size(v), 500);
    clig_vector_free(v);
}

int add_vector_tests(void) {
    CU_pSuite suite = CU_add_suite("vector_suite", NULL, NULL);
    CU_BOOL success = suite &&
        CU_ADD_TEST(suite, test_make_and_free) &&
        CU_ADD_TEST(suite, test_push_and_peek_int) &&
        CU_ADD_TEST(suite, test_pop_with_and_without_dst) &&
        CU_ADD_TEST(suite, test_insert_swap_semantics) &&
        CU_ADD_TEST(suite, test_remove_swap_semantics) &&
        CU_ADD_TEST(suite, test_trim_and_buffer_layout) &&
        CU_ADD_TEST(suite, test_large_resize_sequence);
    if (!success) return CU_get_error();
    return CUE_SUCCESS;
}
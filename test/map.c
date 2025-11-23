#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/ds.h"

static void test_make_and_free(void) {
    clig_map_t *m = clig_map_make(sizeof(int));
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_EQUAL(clig_map_size(m), 0);
    clig_map_free(m);
}

int add_map_tests(void) {
    CU_pSuite suite = CU_add_suite("map_suite", NULL, NULL);
    CU_BOOL success = suite &&
        CU_ADD_TEST(suite, test_make_and_free);
    if (!success) return CU_get_error();
    return CUE_SUCCESS;
}
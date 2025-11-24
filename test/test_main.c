#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define SUITES \
    X(add_xform_tests)

// forward declare the test suites.
#define X(name) int name(void);
SUITES
#undef X

int main(void) {
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
    #define X(name) if (CUE_SUCCESS != name()) { \
        CU_cleanup_registry(); \
        return CU_get_error(); \
    }
    SUITES
    #undef X
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

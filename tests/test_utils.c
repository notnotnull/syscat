#include <CUnit/Basic.h>
#include "utils.h"

void test_len() {
    CU_ASSERT(len("Hello") == 5);
    CU_ASSERT(len("") == 0);
}

void test_len_null() {
    CU_ASSERT(len(NULL) == 0);
}

void test_len_spec_chars() {
    CU_ASSERT(len("\t\n\0") == 2);
}

void test_clear_buff() {
    char buffer[BUFFER_SIZE] = "Test Data";
    clear_buff(buffer);
    CU_ASSERT_STRING_EQUAL(buffer, "");
}

void test_arg_check() {
    CU_ASSERT(arg_check(1) == FAILURE);
    CU_ASSERT(arg_check(2) == SUCCESS);
}

int main() {
    CU_initialize_registry();
    CU_pSuite pSuite = CU_add_suite("Utils_Test_Suite", 0, 0);
    CU_add_test(pSuite, "Test len function", test_len);
    CU_add_test(pSuite, "Test len function with NULL input", test_len_null);
    CU_add_test(pSuite, "Test len function with special characters", test_len_spec_chars);
    CU_add_test(pSuite, "Test clear_buff function", test_clear_buff);
    CU_add_test(pSuite, "Test arg_check function", test_arg_check);
    CU_basic_run_tests();
    int num_failed = CU_get_number_of_failures();
    CU_cleanup_registry();
    return num_failed > 0 ? 1 : 0;
}
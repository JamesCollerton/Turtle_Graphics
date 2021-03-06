#include "turtle_functions.h"

// Needed to intialise the CUnit components.
int initialise_CUnit(CU_pSuite *pSuite, char *suite_name)
{
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   *pSuite = CU_add_suite(suite_name, NULL, NULL);
   if (NULL == *pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   return 0;
}

// Closes the CUnit components.
int close_CUnit(void)
{
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

// Initialises and sets up a suite for CUnit for testing.
void set_up_test(char *suite_name, char *test_name, CU_TestFunc function)
{
   CU_pSuite pSuite = NULL;

   initialise_CUnit(&pSuite, suite_name);

   if ((NULL == CU_add_test(pSuite, test_name, function))){
      CU_cleanup_registry();
      CU_get_error();
    }

   close_CUnit();
}

// Used as wrappers for the assert, string_equal and string_not_equal functionalities so they print informative messages.
void assert_test(int test, char *message)
{
    CU_ASSERT(test);
    if(test){
        printf("\n%s\n", message);
    }
    else{
        printf("\nWARNING FAILED: %s\n", message);
    }

}

void string_equal_test(char *string_one, char *string_two, char *message)
{
    CU_ASSERT_STRING_EQUAL(string_one, string_two);
    if( strcmp(string_one, string_two) == 0 ){
        printf("\n%s\n", message);
    }
    else{
        printf("\nWARNING FAILED: %s\n", message);
    }
}

void string_not_equal_test(char *string_one, char *string_two, char *message)
{
    CU_ASSERT_STRING_NOT_EQUAL(string_one, string_two);
    if( strcmp(string_one, string_two) != 0 ){
        printf("\n%s\n", message);
    }
    else{
        printf("\nWARNING FAILED: %s\n", message);
    }
}

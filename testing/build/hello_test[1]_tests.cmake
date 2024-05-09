add_test( HelloTest.BasicAssertions /home/charles/Task-2.1.b--Secure_Slicing-Xapp/testing/build/hello_test [==[--gtest_filter=HelloTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties( HelloTest.BasicAssertions PROPERTIES WORKING_DIRECTORY /home/charles/Task-2.1.b--Secure_Slicing-Xapp/testing/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( hello_test_TESTS HelloTest.BasicAssertions)

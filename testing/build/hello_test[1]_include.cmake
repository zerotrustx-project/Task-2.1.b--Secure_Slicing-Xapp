if(EXISTS "/home/charles/Task-2.1.b--Secure_Slicing-Xapp/testing/build/hello_test[1]_tests.cmake")
  include("/home/charles/Task-2.1.b--Secure_Slicing-Xapp/testing/build/hello_test[1]_tests.cmake")
else()
  add_test(hello_test_NOT_BUILT hello_test_NOT_BUILT)
endif()

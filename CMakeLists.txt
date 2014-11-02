
cmake_minimum_required (VERSION 2.8)

set(PROJECT_NAME Yawot-tests)
project(${PROJECT_NAME})

# Configuration variables
# - We namespace the cache variable so the configuration variables are grouped
set(YAWOT_UNIT_TESTS FALSE CACHE BOOL "Build unit tests.")
if(UNIT_TESTS)
  set(YAWOT_UNIT_TESTS ${UNIT_TESTS} CACHE BOOL "Build unit tests." FORCE)
endif()

if(YAWOT_UNIT_TESTS)
  # Any tests included here must append the test name
  # to the YAWOT_TESTS list.  Any source files for the
  # test must be set to <test_name>_SRCS, likewise for
  # header files <test_name>_HDRS.
  include(version/tests.cmake)

  enable_testing()
  add_subdirectory(gtest-1.7.0)
  include_directories("here")

  foreach(TEST ${YAWOT_TESTS})
	set(CURRENT_TEST test_${TEST})
	add_executable(${CURRENT_TEST} ${${TEST}_SRCS} ${${TEST}_HDRS})
	target_link_libraries(${CURRENT_TEST} yawot gtest gtest_main)
	add_test(${CURRENT_TEST} ${CURRENT_TEST})
  endforeach()

endif()


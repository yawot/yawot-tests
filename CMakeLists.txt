
cmake_minimum_required (VERSION 2.8)

set(PROJECT_NAME Yawot-tests)
project(${PROJECT_NAME})

# Configuration variables
# - We namespace the cache variable so the configuration variables are grouped
set(YAWOT_UNIT_TESTS OFF CACHE BOOL "Build unit tests.")
if(UNIT_TESTS)
  set(YAWOT_UNIT_TESTS ${UNIT_TESTS} CACHE BOOL "Build unit tests." FORCE)
endif()
if(YAWOT_ENABLE_COVERAGE)
  set(YAWOT_UNIT_TESTS ON CACHE BOOL "Build unit tests." FORCE)
endif()

if(YAWOT_UNIT_TESTS)
  # Any tests included here must append the test name
  # to the YAWOT_TESTS list.  Any source files for the
  # test must be set to <test_name>_SRCS, likewise for
  # header files <test_name>_HDRS.
  include(version/tests.cmake)

  enable_testing()
  add_subdirectory(gtest-1.7.0)

  get_target_property(YAWOT_INCLUDE_DIRECTORIES yawot INCLUDE_DIRECTORIES)
  include_directories(${YAWOT_INCLUDE_DIRECTORIES})

  # Cycle through all the tests 'included' above
  set(test_list)
  set(valgrind_test_list)
  foreach(TEST ${YAWOT_TESTS})
    set(CURRENT_TEST test_${TEST})
    add_executable(${CURRENT_TEST} ${${TEST}_SRCS} ${${TEST}_HDRS})
    target_link_libraries(${CURRENT_TEST} yawot gtest gtest_main)
    add_test(${CURRENT_TEST} ${CURRENT_TEST})
    list(APPEND test_list ${CURRENT_TEST})
  endforeach()

  if(YAWOT_ENABLE_COVERAGE)
    # Prepare for test coverage by generating the gcov note files
    # and creating an output directory for the data files
    add_custom_target(prepare_coverage
      COMMAND find ${GCOV_ANALYSIS_PATH} -type f -name '*.gcda' -print | xargs /bin/rm -f
      COMMAND ctest > /dev/null
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/gcov
      COMMENT "Prepare for test coverage")
    add_custom_target(coverage
      COMMAND gcov `find ${GCOV_ANALYSIS_PATH} -type f -name *.gcno` -r -s ${GCOV_PREFIX_PATH}
      COMMAND gcov `find ${GCOV_ANALYSIS_PATH} -type f -name *.gcno` -r -s ${GCOV_BUILD_PREFIX_PATH}
      DEPENDS ${test_list} prepare_coverage
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gcov
      COMMENT "Running test coverage")
  endif()

endif()


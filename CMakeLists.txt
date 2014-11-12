
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
if(YAWOT_ENABLE_MEMCHECK)
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
  set(TEST_LIST)
  foreach(TEST ${YAWOT_TESTS})
    set(CURRENT_TEST test_${TEST})
    if(CURRENT_CATEGORY)
      set(CURRENT_CATEGORY "${CURRENT_CATEGORY}_")
    endif()
    add_executable(${CURRENT_TEST} ${${TEST}_SRCS} ${${TEST}_HDRS})
    target_link_libraries(${CURRENT_TEST} yawot gtest gtest_main)
    add_test(${CURRENT_CATEGORY}unit_${CURRENT_TEST} ${CURRENT_TEST})
    list(APPEND TEST_LIST ${CURRENT_TEST})
    unset(CURRENT_CATEGORY)
  endforeach()

  if(YAWOT_ENABLE_COVERAGE)
    # Prepare for test coverage by generating the gcov note files
    # and creating an output directory for the data files
    add_custom_target(prepare_coverage
      COMMAND find ${GCOV_ANALYSIS_PATH} -type f -name '*.gcda' -print | xargs /bin/rm -f
      COMMAND ctest > /dev/null
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/gcov
      COMMENT "Prepare for coverage tests")
    add_custom_target(coverage
      COMMAND gcov `find ${GCOV_ANALYSIS_PATH} -type f -name *.gcno` -r -s ${GCOV_PREFIX_PATH}
      COMMAND gcov `find ${GCOV_ANALYSIS_PATH} -type f -name *.gcno` -r -s ${GCOV_BUILD_PREFIX_PATH}
      DEPENDS ${TEST_LIST} prepare_coverage
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gcov
      COMMENT "Running coverage tests")
  endif()

  if(YAWOT_ENABLE_MEMCHECK)
    add_custom_target(prepare_memcheck
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/memcheck
      COMMENT "Prepare for memcheck tests")
    add_custom_target(memcheck
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/valgrindtest.py ${CMAKE_CURRENT_BINARY_DIR} ${TEST_LIST}
      DEPENDS ${TEST_LIST} prepare_memcheck
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/memcheck
      COMMENT "Running memcheck tests")
  endif()

endif()


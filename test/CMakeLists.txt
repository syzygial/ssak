SET(TEST_FILES
  test_sqlite.cpp
  test_archive.cpp)

add_executable(run_tests EXCLUDE_FROM_ALL test_main.cpp ${TEST_FILES})
target_include_directories(run_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(run_tests PRIVATE sqlite3)
add_custom_target(test
  COMMAND test/run_tests
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  DEPENDS run_tests
  COMMENT "Running tests")
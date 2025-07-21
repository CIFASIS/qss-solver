# Output directories
set(SYSTEM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/system)
set(TEST_DATA_DIR ${SYSTEM_DIR}/test_data)
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(BIN_DIR ${BUILD_DIR}/bin)
set(MMOC_DIR ${QSS_SOLVER_ROOT}/mmoc)

set(GOOGLE_TEST_INSTALL ${QSS_SOLVER_ROOT}/3rd-party/gtest)
set(GOOGLE_TEST_LIB gtest)
set(GOOGLE_MOCK_LIB gmock)
set(GOOGLE_TEST_INCLUDE ${GOOGLE_TEST_INSTALL}/usr/include)

# Create output directories
file(MAKE_DIRECTORY
      ${BUILD_DIR}
      ${BIN_DIR}
      ${TEST_DATA_DIR}
)

# Add executable
add_executable(
    mmoc-test
)

target_sources(
    mmoc-test
    PRIVATE
        main.cpp
        test_utils.cpp
)

target_compile_options(mmoc-test PRIVATE "-Wall" "-std=c++17")

# Link the library
target_link_libraries(mmoc-test
                      PRIVATE 
                      ${GOOGLE_TEST_LIB}
                      ${GOOGLE_MOCK_LIB}
                      mmoc-library
                      ginac
                      pthread
)

target_include_directories(mmoc-test PRIVATE ${MMOC_DIR})
target_include_directories(mmoc-test PRIVATE ${GOOGLE_TEST_INCLUDE})
target_link_directories(mmoc-test PRIVATE ${GOOGLE_TEST_INSTALL}/usr/lib)

add_subdirectory(integration)
add_subdirectory(system)

# Set output directory for executable
set_target_properties(mmoc-test PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
    ${BIN_DIR}
)

add_custom_target(run-mmoc-test
      COMMENT "Running MMOC tests."
      COMMAND cd ${TEST_DATA_DIR} && rm -rf *
      COMMAND cd ${BIN_DIR} && ./mmoc-test
)

add_dependencies(run-mmoc-test mmoc-test)
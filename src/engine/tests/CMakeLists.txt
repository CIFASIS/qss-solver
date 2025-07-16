# Output directories
set(SYSTEM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/system)
set(TEST_DATA_DIR ${SYSTEM_DIR}/test_data)
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(BIN_DIR ${BUILD_DIR}/bin)

set(GOOGLE_TEST_INSTALL ${QSS_SOLVER_ROOT}/3rd-party/gtest)
set(GOOGLE_TEST_LIB gtest)
set(GOOGLE_MOCK_LIB gmock)
set(GOOGLE_TEST_INCLUDE ${GOOGLE_TEST_INSTALL}/usr/include)

# Create output directories
file(MAKE_DIRECTORY
      ${BUILD_DIR}
      ${BIN_DIR}
)

# Add executable
add_executable(
    engine-test
)

target_sources(
    engine-test
    PRIVATE
        main.cpp
)

target_compile_options(engine-test PRIVATE "-Wall" "-std=c++11")

# Link the library
target_link_libraries(engine-test
                      PRIVATE 
                      ${GOOGLE_TEST_LIB}
                      ${GOOGLE_MOCK_LIB}
                      pthread
)

target_include_directories(engine-test PRIVATE ${GOOGLE_TEST_INCLUDE})

target_link_directories(engine-test PRIVATE ${GOOGLE_TEST_INSTALL}/usr/lib)

#target_include_directories(qss-seq PUBLIC ${CMAKE_INSTALL_PREFIX}/include)


add_subdirectory(system)

# Set output directory for executable
set_target_properties(engine-test PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
    ${BIN_DIR}
)

add_custom_target(run-engine-test
      COMMAND echo "Running engine tests."
      COMMAND cd ${TEST_DATA_DIR} && rm -rf *
      COMMAND cd ${BIN_DIR} && ./engine-test
)

add_dependencies(run-engine-test engine-test)
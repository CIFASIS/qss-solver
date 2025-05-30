cmake_minimum_required(VERSION 3.10)

project(gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build mode flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Include directories
include_directories(
  \${PROJECT_SOURCE_DIR}
  \${PROJECT_SOURCE_DIR}/usr/include
)

# Source files
file(GLOB_RECURSE SRC_FILES "*.cpp" "*.h")

# Output directories
set(BIN_DIR \${PROJECT_SOURCE_DIR}/usr/bin)
set(LIB_DIR \${PROJECT_SOURCE_DIR}/usr/lib)
set(INCLUDE_DIR \${PROJECT_SOURCE_DIR}/usr/include)
set(BUILD_DIR \${PROJECT_SOURCE_DIR}/usr/obj)

# Create output directories
file(MAKE_DIRECTORY \${BIN_DIR} \${LIB_DIR} \${INCLUDE_DIR} \${BUILD_DIR})

# Add executable
add_executable(qss-solver-gui \${SRC_FILES})

# Set output directory for executable
set_target_properties(qss-solver-gui PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY \${BIN_DIR}
)

# Add test target
add_custom_target(test
  COMMAND \${CMAKE_COMMAND} --build . --target test
  WORKING_DIRECTORY \${PROJECT_SOURCE_DIR}/tests
  COMMENT "Building and running tests"
)

# Add documentation target
add_custom_target(doc
  COMMAND doxygen \${PROJECT_SOURCE_DIR}/QSSSolverGUI.doxyfile
  WORKING_DIRECTORY \${PROJECT_SOURCE_DIR}
  COMMENT "Generating API documentation with Doxygen"
)

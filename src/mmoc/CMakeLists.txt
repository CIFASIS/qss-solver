cmake_minimum_required(VERSION 3.10)

project(mmoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build mode flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(YY_MCC_Parser_DEBUG)
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
set(SRC_FILES)
file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS *.cpp)
list(FILTER CPP_SOURCES EXCLUDE REGEX ".*/tests/.*")
list(APPEND SRC_FILES \${CPP_SOURCES})
list(APPEND SRC_FILES \${PROJECT_SOURCE_DIR}/main.cpp)

# Output directories
set(BIN_DIR \${PROJECT_SOURCE_DIR}/usr/bin)
set(LIB_DIR \${PROJECT_SOURCE_DIR}/usr/lib)
set(INC_DIR \${PROJECT_SOURCE_DIR}/usr/include)
set(BUILD_DIR \${PROJECT_SOURCE_DIR}/usr/obj)

# Create output directories
file(MAKE_DIRECTORY \${BIN_DIR} \${LIB_DIR} \${INC_DIR} \${BUILD_DIR})

# Add executable
add_executable(mmoc \${SRC_FILES})

# Link libraries
target_link_libraries(mmoc ginac cln gmp)

# Set output directory for executable
set_target_properties(mmoc PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY \${BIN_DIR}
)

# Add custom target for documentation
add_custom_target(doc
  COMMAND doxygen \${PROJECT_SOURCE_DIR}/MMOCompiler.doxyfile
  WORKING_DIRECTORY \${PROJECT_SOURCE_DIR}
  COMMENT "Generating API documentation with Doxygen"
)

# Add test target
add_custom_target(test
  COMMAND \${CMAKE_COMMAND} --build . --target mmoc
  COMMAND \${CMAKE_COMMAND} -E chdir tests \${CMAKE_MAKE_PROGRAM} test
  WORKING_DIRECTORY \${PROJECT_SOURCE_DIR}
  COMMENT "Building and running tests"
)

cmake_minimum_required(VERSION 3.10)

project(usr)

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
  \${PROJECT_SOURCE_DIR}/include
)

# Source files
file(GLOB_RECURSE SRC_FILES "*.cpp" "*.c" "*.h")

# Output directories
set(BIN_DIR \${PROJECT_SOURCE_DIR}/bin)
set(LIB_DIR \${PROJECT_SOURCE_DIR}/lib)
set(INCLUDE_DIR \${PROJECT_SOURCE_DIR}/include)
set(BUILD_DIR \${PROJECT_SOURCE_DIR}/obj)

# Create output directories
file(MAKE_DIRECTORY \${BIN_DIR} \${LIB_DIR} \${INCLUDE_DIR} \${BUILD_DIR})

# Add libraries or executables as needed
# Example: add_library or add_executable here if applicable

# Install targets for libraries and binaries
install(DIRECTORY \${LIB_DIR}/ DESTINATION lib)
install(DIRECTORY \${BIN_DIR}/ DESTINATION bin)

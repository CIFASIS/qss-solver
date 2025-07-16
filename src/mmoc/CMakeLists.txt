cmake_minimum_required(VERSION 3.16)

project(MMOC)

# mmoc-library
add_library(mmoc-library STATIC)


# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Build mode flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(YY_MCC_Parser_DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-write-strings -Wall -std=c++17")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wall -std=c++17")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")

target_include_directories(mmoc-library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Source files.
add_subdirectory(ast)
add_subdirectory(deps)
add_subdirectory(generator)
add_subdirectory(ir)
add_subdirectory(tests)
add_subdirectory(util)

# Add executable
add_executable(
    mmoc
)

target_sources(
    mmoc
    PRIVATE
        main.cpp
)

target_link_libraries(
    mmoc
    PRIVATE
        mmoc-library
        ginac
        cln
        gmp   
)

# Add custom target for documentation
add_custom_target(mmoc-doc
  COMMAND doxygen MMOCompiler.doxyfile
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mmoc
  COMMENT "Generating MMOC API documentation"
)

# install
install(
    TARGETS mmoc
    RUNTIME DESTINATION bin
    COMPONENT mmoc
)

cmake_minimum_required(VERSION 3.16)

project(Engine)

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH)


# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(Fortran)

# Set build mode flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -msse2 -mfpmath=sse")
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -g -Wall -fallow-argument-mismatch")
  else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -msse2 -mfpmath=sse")
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -O2 -Wall -fallow-argument-mismatch")
endif()

# Add libraries
add_library(qss-seq OBJECT)
add_library(time-step STATIC)
add_library(qss STATIC)

set_target_properties(qss-seq PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(time-step PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(qss-seq PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(time-step PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Source files.
add_subdirectory(3rd-party)
add_subdirectory(classic)
add_subdirectory(common)
add_subdirectory(qss)
add_subdirectory(tests)

target_link_libraries(qss PRIVATE qss-seq qss-parallel)
set_target_properties(qss PROPERTIES DEBUG_POSTFIX "d")

# Add custom target for documentation
add_custom_target(engine-doc
  COMMENT "Generating QSS Solver Engine documentation."
  COMMAND doxygen QSSSolver.doxyfile
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

set_target_properties(engine-doc PROPERTIES EXCLUDE_FROM_ALL ON)

# install
install(
    TARGETS qss time-step
    RUNTIME DESTINATION lib
    COMPONENT qss-libraries
    LIBRARY
        DESTINATION lib
        PERMISSIONS
            OWNER_READ
            OWNER_WRITE
            OWNER_EXECUTE
            GROUP_READ
            GROUP_EXECUTE
            WORLD_READ
            WORLD_EXECUTE
    COMPONENT qss-libraries
)

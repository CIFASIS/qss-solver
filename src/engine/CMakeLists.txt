cmake_minimum_required(VERSION 3.10)

project(engine C CXX Fortran)

# Set build mode flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -msse2 -mfpmath=sse")
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -g -Wall -fallow-argument-mismatch")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -msse2 -mfpmath=sse")
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -O2 -Wall -fallow-argument-mismatch")
endif()

# Include directories
include_directories(
  \${PROJECT_SOURCE_DIR}
  \${PROJECT_SOURCE_DIR}/usr/include
  \${PROJECT_SOURCE_DIR}/common
)

# Source files
file(GLOB_RECURSE C_SOURCES "*.c")
file(GLOB_RECURSE FORTRAN_SOURCES "*.f")

# Output directories
set(BIN_DIR \${PROJECT_SOURCE_DIR}/usr/bin)
set(LIB_DIR \${PROJECT_SOURCE_DIR}/usr/lib)
set(INCLUDE_DIR \${PROJECT_SOURCE_DIR}/usr/include)
set(BUILD_DIR \${PROJECT_SOURCE_DIR}/usr/obj)

# Create output directories
file(MAKE_DIRECTORY \${BIN_DIR} \${LIB_DIR} \${INCLUDE_DIR} \${BUILD_DIR})

# Add libraries
add_library(qss STATIC \${C_SOURCES})
add_library(timestep STATIC \${FORTRAN_SOURCES})

# Set output directories for libraries
set_target_properties(qss PROPERTIES ARCHIVE_OUTPUT_DIRECTORY \${LIB_DIR})
set_target_properties(timestep PROPERTIES ARCHIVE_OUTPUT_DIRECTORY \${LIB_DIR})

# Add custom targets for building subcomponents
add_custom_target(kahip
  COMMAND \${CMAKE_COMMAND} --build \${PROJECT_SOURCE_DIR}/3rd-party/KaHIP-3.16
  COMMENT "Building KaHIP partitioner"
)

add_custom_target(cvode
  COMMAND \${CMAKE_COMMAND} --build \${PROJECT_SOURCE_DIR}/cvode
  COMMENT "Building CVODE"
)

add_custom_target(ida
  COMMAND \${CMAKE_COMMAND} --build \${PROJECT_SOURCE_DIR}/ida
  COMMENT "Building IDA"
)

add_custom_target(gsl
  COMMAND \${CMAKE_COMMAND} --build \${PROJECT_SOURCE_DIR}/gsl
  COMMENT "Building GSL"
)

add_custom_target(qss
  DEPENDS qss
)

add_custom_target(time-step
  DEPENDS timestep
)

# Add test target
add_custom_target(test
  COMMAND \${CMAKE_COMMAND} --build . --target test
  WORKING_DIRECTORY \${PROJECT_SOURCE_DIR}/tests
  COMMENT "Building and running tests"
)

# Add documentation target
add_custom_target(doc
  COMMAND doxygen \${PROJECT_SOURCE_DIR}/QSSSolver.doxyfile
  WORKING_DIRECTORY \${PROJECT_SOURCE_DIR}
  COMMENT "Generating API documentation with Doxygen"
)

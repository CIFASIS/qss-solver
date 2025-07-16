cmake_minimum_required(VERSION 3.16)

project(SBML)

# Build mode flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# mmoc-library
add_library(sbml-library STATIC)

target_include_directories(sbml-library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Source files.
target_sources(
    sbml-library
    PRIVATE
        biorica_writer.cpp
        mmo_assignment.cpp
        mmo_convert.cpp
        mmo_decl.cpp
        mmo_equation.cpp
        mmo_event.cpp
        mmo_function.cpp
        mmo_math.cpp
        mmo_model.cpp
        mmo_section.cpp
        mmo_utils.cpp
        mmo_writer.cpp
        mmo_zerocrossing.cpp
)

# Add executable
add_executable(
    translate-sbml
)

target_sources(
    translate-sbml
    PRIVATE
        convert.cpp
)

target_link_libraries(
    translate-sbml
    PRIVATE
        sbml-library
        sbml
)

# install
install(
    TARGETS translate-sbml sbml-library
    RUNTIME DESTINATION bin
    COMPONENT translate-sbml
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
    COMPONENT translate-sbml
)

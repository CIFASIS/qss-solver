cmake_minimum_required(VERSION 3.16)

project(QSS-Solver)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
set(QSS_SOLVER_ROOT ${PROJECT_SOURCE_DIR})

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Output directories
set(ROOT_DIR ../..)
set(PROJECT_LIB_DIR ${ROOT_DIR}/lib)
set(PROJECT_BIN_DIR ${ROOT_DIR}/bin)
set(INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(DOC_DIR ${CMAKE_INSTALL_PREFIX}/doc)

# Create output directories
file(MAKE_DIRECTORY
      ${BIN_DIR}
      ${LIB_DIR}      
      ${INCLUDE_DIR}
      ${DOC_DIR}
)

add_custom_target(
  mmoc
  ALL
  COMMENT "Building MicroModelica Compiler."
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/mmoc/build
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/mmoc/build && cmake .. -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DQSS_SOLVER_ROOT=${QSS_SOLVER_ROOT} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/mmoc/build
)

add_custom_target(
  engine
  ALL
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/debug
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/debug && cmake ../.. -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DQSS_SOLVER_ROOT=${QSS_SOLVER_ROOT}
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/debug
  COMMENT "Building QSS Solver Debug lib"
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/release
  COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/release && cmake ../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DQSS_SOLVER_ROOT=${QSS_SOLVER_ROOT}
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/release
  COMMENT "Building QSS Solver Release lib"
)

add_custom_target(
  usr
  ALL
  COMMENT "Building MicroModelica User Libraries."
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/usr/build
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/usr/build && cmake .. -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DQSS_SOLVER_ROOT=${QSS_SOLVER_ROOT} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/usr/build
)

add_custom_target(
  interfaces
  ALL
  COMMENT "Building QSS Solver Interfaces."
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/sbml/build
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/sbml/build && cmake .. -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DQSS_SOLVER_ROOT=${QSS_SOLVER_ROOT} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/sbml/build
)

add_custom_target(
  solver-gui
  ALL
  COMMENT "Building QSS Solver GUI."
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/gui/build
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/gui/build && cmake .. -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DQSS_SOLVER_ROOT=${QSS_SOLVER_ROOT} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/gui/build
)

add_custom_target(
  doc
  COMMENT "Generating QSS Solver Documentation ${CMAKE_CURRENT_SOURCE_DIR}."
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/release && make engine-doc
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/mmoc/build && make mmoc-doc
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/sbml/build && make sbml-doc
)

add_dependencies(doc mmoc engine interfaces)

add_custom_target(test
    COMMAND echo "Running Tests."
	  COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/mmoc/build && make run-mmoc-test
	  COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/debug && make run-engine-test
)

add_custom_target(reset-targets
    COMMAND echo "Delete build directories from targets."
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/engine && ${CMAKE_COMMAND} -E rm -rf build
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/engine/3rd-party && ${CMAKE_COMMAND} -E rm -rf build
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/sbml && ${CMAKE_COMMAND} -E rm -rf build
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/mmoc && ${CMAKE_COMMAND} -E rm -rf build
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/gui && ${CMAKE_COMMAND} -E rm -rf build
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/usr && ${CMAKE_COMMAND} -E rm -rf build
    COMMAND cd ${PROJECT_LIB_DIR} && ${CMAKE_COMMAND} -E rm -rf *
    COMMAND cd ${PROJECT_BIN_DIR} && ${CMAKE_COMMAND} -E rm mmoc
    COMMAND cd ${PROJECT_BIN_DIR} && ${CMAKE_COMMAND} -E rm qss-solver
    COMMAND cd ${PROJECT_BIN_DIR} && ${CMAKE_COMMAND} -E rm translate-sbml
)


install(
    CODE "execute_process(COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/mmoc/build install)"
    CODE "execute_process(COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/usr/build install)"
    CODE "execute_process(COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/debug install)"
    CODE "execute_process(COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/engine/build/release install)"
    CODE "execute_process(COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/sbml/build install)"
    CODE "execute_process(COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/gui/build install)"
)

install(DIRECTORY
        ${LIB_DIR} 
        DESTINATION 
        ${ROOT_DIR}
        FILES_MATCHING PATTERN "*.a")

install(FILES
        ${BIN_DIR}/mmoc
        ${BIN_DIR}/qss-solver
        ${BIN_DIR}/translate-sbml 
        PERMISSIONS
            OWNER_READ
            OWNER_WRITE
            OWNER_EXECUTE
            GROUP_READ
            GROUP_EXECUTE
            WORLD_READ
            WORLD_EXECUTE
        DESTINATION 
        ${PROJECT_BIN_DIR})



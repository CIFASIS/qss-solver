set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KLU_LIB_DIR /usr/lib/${ARCH}-linux-gnu)
set(KLU_INCLUDE_DIR /usr/include/suitesparse)

target_include_directories(qss-seq PUBLIC ${CMAKE_INSTALL_PREFIX}/include)

set(CVODE_LIB "${CMAKE_INSTALL_PREFIX}/lib/libsundials_cvode.a")
set(GSL_LIB "${CMAKE_INSTALL_PREFIX}/lib/libgsl.a")
set(IDA_LIB "${CMAKE_INSTALL_PREFIX}/lib/libsundials_ida.a")

add_custom_command(
    OUTPUT ${CVODE_LIB}  
    COMMAND ${CMAKE_COMMAND} -E echo "Build CVODE."
    COMMAND ${CMAKE_COMMAND} -E tar xvzf ${3RD_PARTY_DIR}/cvode/cvode-2.9.0.tar.gz
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/cvode-2.9.0/build
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/cvode-2.9.0/build && cmake ..  -DLAPACK_ENABLE=ON -DCMAKE_BUILD_TYPE=Release  -DKLU_ENABLE=ON -DKLU_INCLUDE_DIR=${KLU_INCLUDE_DIR} -DKLU_LIBRARY_DIR=${KLU_LIB_DIR}  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/cvode-2.9.0/build install
	COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/cvode-2.9.0
	COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/build/examples
)

add_custom_command(
    OUTPUT ${GSL_LIB}  
    COMMAND ${CMAKE_COMMAND} -E echo "Build GSL."
    COMMAND ${CMAKE_COMMAND} -E tar xvzf ${3RD_PARTY_DIR}/gsl/gsl-2.5.tar.gz
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/gsl-2.5/ && ./configure --prefix=${CMAKE_INSTALL_PREFIX}
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/gsl-2.5/ && make
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/gsl-2.5/ && make install
    COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/gsl-2.5
)

add_custom_command(
    OUTPUT ${IDA_LIB}  
    COMMAND ${CMAKE_COMMAND} -E echo "Build IDA."
    COMMAND ${CMAKE_COMMAND} -E tar xvzf ${3RD_PARTY_DIR}/ida/ida-2.9.0.tar.gz
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/ida-2.9.0/build
	COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/ida-2.9.0/build && cmake .. -DLAPACK_ENABLE=ON -DCMAKE_BUILD_TYPE=Release  -DKLU_ENABLE=ON -DKLU_INCLUDE_DIR=${KLU_INCLUDE_DIR} -DKLU_LIBRARY_DIR=${KLU_LIB_DIR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/ida-2.9.0/build install
	COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/ida-2.9.0
	COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/build/examples
)

add_custom_target(
    3rd-party-libs
    DEPENDS  ${CVODE_LIB}
    DEPENDS  ${GSL_LIB}
    DEPENDS  ${IDA_LIB}   
)

add_dependencies(qss-seq 3rd-party-libs)

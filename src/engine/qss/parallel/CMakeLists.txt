set(QSS_DIR ${ENGINE_DIR}/qss)

add_library(qss-parallel OBJECT)

target_include_directories(qss-parallel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${ENGINE_DIR})

target_compile_definitions(qss-parallel
  PRIVATE
  QSS_PARALLEL
)

target_sources(
    qss-parallel
    PUBLIC
        qss_dt.c
        qss_lp.c
        qss_parallel.c
        qss_par_integrator.c
        qss_partition.c
        qss_sim_steps.c
        ${QSS_DIR}/qss_file.c
		${QSS_DIR}/qss_memory.c
		${QSS_DIR}/qss_sampled.c
		${QSS_DIR}/qss_step.c
		${QSS_DIR}/qss_binary.c
		${QSS_DIR}/qss_linear.c
		${QSS_DIR}/qss_frw_imp.c 
)

add_subdirectory(methods)

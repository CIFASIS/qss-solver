target_sources(
    qss-seq
    PRIVATE
        classic_commands.c
        classic_cvode_integrator.c
        classic_dassl_integrator.c
        classic_data.c
        classic_dopri_integrator.c
        classic_ida_integrator.c
        classic_integrator.c
        classic_simulator.c
)

add_subdirectory(dassl)
add_subdirectory(dopri5)
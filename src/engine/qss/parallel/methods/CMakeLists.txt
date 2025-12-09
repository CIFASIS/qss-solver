set(METHODS_DIR ${ENGINE_DIR}/qss/methods)

target_sources(
    qss-parallel
    PRIVATE
        ${METHODS_DIR}/cheqss1.c
        ${METHODS_DIR}/cheqss2.c
        ${METHODS_DIR}/cheqss3.c
        ${METHODS_DIR}/eqss1.c
        ${METHODS_DIR}/eqss2.c
        ${METHODS_DIR}/eqss3.c
        ${METHODS_DIR}/liqss.c
        ${METHODS_DIR}/liqss2.c
        ${METHODS_DIR}/liqss3.c
        ${METHODS_DIR}/liqss_bdf.c
        ${METHODS_DIR}/mliqss.c
        ${METHODS_DIR}/mliqss2.c
        ${METHODS_DIR}/mliqss3.c
        ${METHODS_DIR}/qss.c
        ${METHODS_DIR}/qss2.c
        ${METHODS_DIR}/qss3.c
        ${METHODS_DIR}/qss4.c
)

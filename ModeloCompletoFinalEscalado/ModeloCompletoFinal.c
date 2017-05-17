#include <gsl/gsl_multiroots.h>
#define pre(X) X
int
fsolve1_eval (const gsl_vector * __x, void * __p, gsl_vector * __f)
{
    double *args = (double*) __p;
    const double boost_panel1_panel_n_i = gsl_vector_get (__x, 0);
    const double boost_panel1_capacitor1_v = args[0];
    gsl_vector_set (
            __f,
            0,
            (boost_panel1_panel_n_i - 8.48234
                    + 6.51513e-07 * (exp (1.6e-19 * (boost_panel1_capacitor1_v / 60 + boost_panel1_panel_n_i * 0.01 / 1) / 6.58222e-21) - 1)) - (0));
    return GSL_SUCCESS;
}
double
fsolve1 (double boost_panel1_capacitor1_v)
{
    size_t __iter = 0;
    int __status, i;
    const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
    gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 1);
    gsl_multiroot_function __F;
    static gsl_vector *__x = NULL;
    if (__x == NULL)
    {
        __x = gsl_vector_alloc (1);
        for (i = 0; i < 1; i++)
            gsl_vector_set (__x, i, 0);
    }
    __F.n = 1;
    __F.f = fsolve1_eval;
    double __args[1];
    __args[0] = boost_panel1_capacitor1_v;
    __F.params = __args;
    gsl_vector *__f = gsl_vector_alloc (1);
    // Try if we are already in the solution from the start (useful for discrete dependendt loops)
    fsolve1_eval (__x, (void*) __args, __f);
    if (gsl_multiroot_test_residual (__f, 1e-7) == GSL_SUCCESS)
    {
        gsl_vector_free (__f);
        gsl_multiroot_fsolver_free (__s);
        return gsl_vector_get (__x, 0);
    }
    gsl_vector_free (__f);
    gsl_multiroot_fsolver_set (__s, &__F, __x);
    do
    {
        __iter++;
        __status = gsl_multiroot_fsolver_iterate (__s);
        if (__status) /* check if solver is stuck */
            break;
        __status = gsl_multiroot_test_residual (__s->f, 1e-7);
    }
    while (__status == GSL_CONTINUE && __iter < 100);
    if (__iter == 100)
        printf ("Warning: GSL could not solve an algebraic loop after %d iterations\n", (int) __iter);
    double ret = gsl_vector_get (__s->x, 0);
    gsl_vector_set (__x, 0, ret);
    gsl_multiroot_fsolver_free (__s);
    return ret;
}
int
fsolve2_eval (const gsl_vector * __x, void * __p, gsl_vector * __f)
{
    double *args = (double*) __p;
    const double boost_panel2_panel_n_i = gsl_vector_get (__x, 0);
    const double boost_panel2_capacitor1_v = args[0];
    gsl_vector_set (
            __f,
            0,
            (boost_panel2_panel_n_i - 5.93764
                    + 6.51513e-07 * (exp (1.6e-19 * (boost_panel2_capacitor1_v / 60 + boost_panel2_panel_n_i * 0.01 / 1) / 6.58222e-21) - 1)) - (0));
    return GSL_SUCCESS;
}
double
fsolve2 (double boost_panel2_capacitor1_v)
{
    size_t __iter = 0;
    int __status, i;
    const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
    gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 1);
    gsl_multiroot_function __F;
    static gsl_vector *__x = NULL;
    if (__x == NULL)
    {
        __x = gsl_vector_alloc (1);
        for (i = 0; i < 1; i++)
            gsl_vector_set (__x, i, 0);
    }
    __F.n = 1;
    __F.f = fsolve2_eval;
    double __args[1];
    __args[0] = boost_panel2_capacitor1_v;
    __F.params = __args;
    gsl_vector *__f = gsl_vector_alloc (1);
    // Try if we are already in the solution from the start (useful for discrete dependendt loops)
    fsolve2_eval (__x, (void*) __args, __f);
    if (gsl_multiroot_test_residual (__f, 1e-7) == GSL_SUCCESS)
    {
        gsl_vector_free (__f);
        gsl_multiroot_fsolver_free (__s);
        return gsl_vector_get (__x, 0);
    }
    gsl_vector_free (__f);
    gsl_multiroot_fsolver_set (__s, &__F, __x);
    do
    {
        __iter++;
        __status = gsl_multiroot_fsolver_iterate (__s);
        if (__status) /* check if solver is stuck */
            break;
        __status = gsl_multiroot_test_residual (__s->f, 1e-7);
    }
    while (__status == GSL_CONTINUE && __iter < 100);
    if (__iter == 100)
        printf ("Warning: GSL could not solve an algebraic loop after %d iterations\n", (int) __iter);
    double ret = gsl_vector_get (__s->x, 0);
    gsl_vector_set (__x, 0, ret);
    gsl_multiroot_fsolver_free (__s);
    return ret;
}
int
fsolve3_eval (const gsl_vector * __x, void * __p, gsl_vector * __f)
{
    double *args = (double*) __p;
    const double fCellAndElectro_fuellCell_IFC = gsl_vector_get (__x, 0);
    const double algSuperv_Pref_FC = args[0];
    gsl_vector_set (
            __f,
            0,
            (fCellAndElectro_fuellCell_IFC)
                    - (algSuperv_Pref_FC
                            / ((1.22894 + 0.0128394 * log (0.3)) * 47
                                    - (26.523
                                            + ((-1.0526) + 6.945e-11 * (pow (fCellAndElectro_fuellCell_IFC, 6))
                                                    - 1.7272e-08 * (pow (fCellAndElectro_fuellCell_IFC, 5))
                                                    + 1.7772e-06 * (pow (fCellAndElectro_fuellCell_IFC, 4))
                                                    - 9.8133e-05 * (pow (fCellAndElectro_fuellCell_IFC, 3))
                                                    + 0.003143 * (pow (fCellAndElectro_fuellCell_IFC, 2)) - 0.03532 * fCellAndElectro_fuellCell_IFC)
                                                    * fCellAndElectro_fuellCell_IFC)
                                    - fCellAndElectro_fuellCell_IFC * (1.7941 - 0.023081 * fCellAndElectro_fuellCell_IFC)
                                    - (-0.0128394 * log (1 - fCellAndElectro_fuellCell_IFC / 75)))));
    return GSL_SUCCESS;
}
double
fsolve3 (double algSuperv_Pref_FC)
{
    size_t __iter = 0;
    int __status, i;
    const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
    gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 1);
    gsl_multiroot_function __F;
    static gsl_vector *__x = NULL;
    if (__x == NULL)
    {
        __x = gsl_vector_alloc (1);
        for (i = 0; i < 1; i++)
            gsl_vector_set (__x, i, 0);
    }
    __F.n = 1;
    __F.f = fsolve3_eval;
    double __args[1];
    __args[0] = algSuperv_Pref_FC;
    __F.params = __args;
    gsl_vector *__f = gsl_vector_alloc (1);
    // Try if we are already in the solution from the start (useful for discrete dependendt loops)
    fsolve3_eval (__x, (void*) __args, __f);
    if (gsl_multiroot_test_residual (__f, 1e-7) == GSL_SUCCESS)
    {
        gsl_vector_free (__f);
        gsl_multiroot_fsolver_free (__s);
        return gsl_vector_get (__x, 0);
    }
    gsl_vector_free (__f);
    gsl_multiroot_fsolver_set (__s, &__F, __x);
    do
    {
        __iter++;
        __status = gsl_multiroot_fsolver_iterate (__s);
        if (__status) /* check if solver is stuck */
            break;
        __status = gsl_multiroot_test_residual (__s->f, 1e-7);
    }
    while (__status == GSL_CONTINUE && __iter < 100);
    if (__iter == 100)
        printf ("Warning: GSL could not solve an algebraic loop after %d iterations\n", (int) __iter);
    double ret = gsl_vector_get (__s->x, 0);
    gsl_vector_set (__x, 0, ret);
    gsl_multiroot_fsolver_free (__s);
    return ret;
}
int
fsolve4_eval (const gsl_vector * __x, void * __p, gsl_vector * __f)
{
    double *args = (double*) __p;
    const double fCellAndElectro_electComp_electrolizadorAnda1_Ucell = gsl_vector_get (__x, 0);
    const double fCellAndElectro_electComp_electrolizadorAnda1_u = args[0];
    gsl_vector_set (
            __f,
            0,
            (fCellAndElectro_electComp_electrolizadorAnda1_Ucell)
                    - (1.229
                            + 6.8882e-05 * (fCellAndElectro_electComp_electrolizadorAnda1_u / (fCellAndElectro_electComp_electrolizadorAnda1_Ucell))
                                    / 0.25
                            + 0.188024
                                    * log10 (
                                            0.246753
                                                    * (fCellAndElectro_electComp_electrolizadorAnda1_u
                                                            / (fCellAndElectro_electComp_electrolizadorAnda1_Ucell)) / 0.25 + 1)));
    return GSL_SUCCESS;
}
double
fsolve4 (double fCellAndElectro_electComp_electrolizadorAnda1_u)
{
    size_t __iter = 0;
    int __status, i;
    const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
    gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 1);
    gsl_multiroot_function __F;
    static gsl_vector *__x = NULL;
    if (__x == NULL)
    {
        __x = gsl_vector_alloc (1);
        for (i = 0; i < 1; i++)
            gsl_vector_set (__x, i, 1);
    }
    __F.n = 1;
    __F.f = fsolve4_eval;
    double __args[1];
    __args[0] = fCellAndElectro_electComp_electrolizadorAnda1_u;
    __F.params = __args;
    gsl_vector *__f = gsl_vector_alloc (1);
    // Try if we are already in the solution from the start (useful for discrete dependendt loops)
    fsolve4_eval (__x, (void*) __args, __f);
    if (gsl_multiroot_test_residual (__f, 1e-7) == GSL_SUCCESS)
    {
        gsl_vector_free (__f);
        gsl_multiroot_fsolver_free (__s);
        return gsl_vector_get (__x, 0);
    }
    gsl_vector_free (__f);
    gsl_multiroot_fsolver_set (__s, &__F, __x);
    do
    {
        __iter++;
        __status = gsl_multiroot_fsolver_iterate (__s);
        if (__status) /* check if solver is stuck */
            break;
        __status = gsl_multiroot_test_residual (__s->f, 1e-7);
    }
    while (__status == GSL_CONTINUE && __iter < 100);
    if (__iter == 100)
        printf ("Warning: GSL could not solve an algebraic loop after %d iterations\n", (int) __iter);
    double ret = gsl_vector_get (__s->x, 0);
    gsl_vector_set (__x, 0, ret);
    gsl_multiroot_fsolver_free (__s);
    return ret;
}

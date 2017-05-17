#include <gsl/gsl_multiroots.h>
#define pre(X) X
int
fsolve1_eval (const gsl_vector * __x, void * __p, gsl_vector * __f)
{
    double *args = (double*) __p;
    const double boost_panel1_panel_Ipv = gsl_vector_get (__x, 0);
    const double boost_panel1_panel_exponente = gsl_vector_get (__x, 1);
    const double boost_panel1_panel_Iph = args[0];
    const double boost_panel1_panel_Vpv = args[1];
    gsl_vector_set (__f, 0,
                    (boost_panel1_panel_exponente) - (1.6e-19 * (boost_panel1_panel_Vpv / 60 + boost_panel1_panel_Ipv * 0.01 / 1) / (6.58222e-21)));
    gsl_vector_set (__f, 1, (boost_panel1_panel_Ipv - 1 * boost_panel1_panel_Iph + 6.51513e-07 * (exp (boost_panel1_panel_exponente) - 1)) - (0));
    return GSL_SUCCESS;
}
void
fsolve1 (double boost_panel1_panel_Iph, double boost_panel1_panel_Vpv, double *boost_panel1_panel_Ipv, double *boost_panel1_panel_exponente)
{
    size_t __iter = 0;
    int __status, i;
    const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
    gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 2);
    gsl_multiroot_function __F;
    static gsl_vector *__x = NULL;
    if (__x == NULL)
    {
        __x = gsl_vector_alloc (2);
        for (i = 0; i < 2; i++)
            gsl_vector_set (__x, i, 0);
    }
    __F.n = 2;
    __F.f = fsolve1_eval;
    double __args[2];
    __args[0] = boost_panel1_panel_Iph;
    __args[1] = boost_panel1_panel_Vpv;
    __F.params = __args;
    gsl_multiroot_fsolver_set (__s, &__F, __x);
    do
    {
        __iter++;
        __status = gsl_multiroot_fsolver_iterate (__s);
        if (__status) /* check if solver is stuck */
            break;
        __status = gsl_multiroot_test_residual (__s->f, 1e-7);
    }
    while (__status == GSL_CONTINUE && __iter < 1000);
    boost_panel1_panel_Ipv[0] = gsl_vector_get (__s->x, 0);
    gsl_vector_set (__x, 0, gsl_vector_get (__s->x, 0));
    boost_panel1_panel_exponente[0] = gsl_vector_get (__s->x, 1);
    gsl_vector_set (__x, 1, gsl_vector_get (__s->x, 1));
    gsl_multiroot_fsolver_free (__s);
}
int
fsolve2_eval (const gsl_vector * __x, void * __p, gsl_vector * __f)
{
    double *args = (double*) __p;
    const double boost_panel1_boost1_idealDiode_v = gsl_vector_get (__x, 0);
    const double boost_panel1_boost1_idealDiode_s = gsl_vector_get (__x, 1);
    const double boost_panel1_boost1_idealDiode_p_v = gsl_vector_get (__x, 2);
    const double boost_panel1_boost1_idealDiode_i = gsl_vector_get (__x, 3);
    const double boost_panel1_boost1_controlledIdealClosingSwitch_s = gsl_vector_get (__x, 4);
    const double boost_panel1_boost1_controlledIdealClosingSwitch_p_i = gsl_vector_get (__x, 5);
    const double boost_panel1_boost1_controlledIdealClosingSwitch_p_v = gsl_vector_get (__x, 6);
    const double boost_panel1_boost1_idealDiode_p_i = gsl_vector_get (__x, 7);
    const double boost_panel1_boost1_controlledIdealClosingSwitch_n_v = args[0];
    const double boost_panel1_boost1_idealDiode_n_v = args[1];
    const double boost_panel1_boost1_inductor_n_i = args[2];
    const double d1 = args[3];
    const double d2 = args[4];
    const double d3 = args[5];
    const double d4 = args[6];
    gsl_vector_set (__f, 0,
                    (boost_panel1_boost1_idealDiode_v) - (boost_panel1_boost1_idealDiode_s * 1 * (pre(d1) * (1) + (1 - pre(d1)) * (1e-05)) + 0));
    gsl_vector_set (__f, 1,
                    (boost_panel1_boost1_idealDiode_i) - (boost_panel1_boost1_idealDiode_s * 1 * (pre(d2) * (1e-05) + (1 - pre(d2)) * (1)) + 0));
    gsl_vector_set (__f, 2, (boost_panel1_boost1_idealDiode_v) - (boost_panel1_boost1_idealDiode_p_v - boost_panel1_boost1_idealDiode_n_v));
    gsl_vector_set (__f, 3, (boost_panel1_boost1_idealDiode_i) - (boost_panel1_boost1_idealDiode_p_i));
    gsl_vector_set (
            __f,
            4,
            (boost_panel1_boost1_controlledIdealClosingSwitch_p_v - boost_panel1_boost1_controlledIdealClosingSwitch_n_v)
                    - (boost_panel1_boost1_controlledIdealClosingSwitch_s * 1 * (pre(d3) * (1e-05) + (1 - pre(d3)) * (1))));
    gsl_vector_set (
            __f,
            5,
            (boost_panel1_boost1_controlledIdealClosingSwitch_p_i)
                    - (boost_panel1_boost1_controlledIdealClosingSwitch_s * 1 * (pre(d4) * (1) + (1 - pre(d4)) * (1e-05))));
    gsl_vector_set (__f, 6, (boost_panel1_boost1_controlledIdealClosingSwitch_p_v) - (boost_panel1_boost1_idealDiode_p_v));
    gsl_vector_set (
            __f, 7,
            (boost_panel1_boost1_controlledIdealClosingSwitch_p_i + boost_panel1_boost1_idealDiode_p_i + boost_panel1_boost1_inductor_n_i) - (0));
    return GSL_SUCCESS;
}
void
fsolve2 (double boost_panel1_boost1_controlledIdealClosingSwitch_n_v, double boost_panel1_boost1_idealDiode_n_v,
         double boost_panel1_boost1_inductor_n_i, double d1, double d2, double d3, double d4, double *boost_panel1_boost1_idealDiode_v,
         double *boost_panel1_boost1_idealDiode_s, double *boost_panel1_boost1_idealDiode_p_v, double *boost_panel1_boost1_idealDiode_i,
         double *boost_panel1_boost1_controlledIdealClosingSwitch_s, double *boost_panel1_boost1_controlledIdealClosingSwitch_p_i,
         double *boost_panel1_boost1_controlledIdealClosingSwitch_p_v, double *boost_panel1_boost1_idealDiode_p_i)
{
    size_t __iter = 0;
    int __status, i;
    const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
    gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 8);
    gsl_multiroot_function __F;
    static gsl_vector *__x = NULL;
    if (__x == NULL)
    {
        __x = gsl_vector_alloc (8);
        for (i = 0; i < 8; i++)
            gsl_vector_set (__x, i, 0);
    }
    __F.n = 8;
    __F.f = fsolve2_eval;
    double __args[7];
    __args[0] = boost_panel1_boost1_controlledIdealClosingSwitch_n_v;
    __args[1] = boost_panel1_boost1_idealDiode_n_v;
    __args[2] = boost_panel1_boost1_inductor_n_i;
    __args[3] = d1;
    __args[4] = d2;
    __args[5] = d3;
    __args[6] = d4;
    __F.params = __args;
    gsl_multiroot_fsolver_set (__s, &__F, __x);
    do
    {
        __iter++;
        __status = gsl_multiroot_fsolver_iterate (__s);
        if (__status) /* check if solver is stuck */
            break;
        __status = gsl_multiroot_test_residual (__s->f, 1e-7);
    }
    while (__status == GSL_CONTINUE && __iter < 1000);
    boost_panel1_boost1_idealDiode_v[0] = gsl_vector_get (__s->x, 0);
    gsl_vector_set (__x, 0, gsl_vector_get (__s->x, 0));
    boost_panel1_boost1_idealDiode_s[0] = gsl_vector_get (__s->x, 1);
    gsl_vector_set (__x, 1, gsl_vector_get (__s->x, 1));
    boost_panel1_boost1_idealDiode_p_v[0] = gsl_vector_get (__s->x, 2);
    gsl_vector_set (__x, 2, gsl_vector_get (__s->x, 2));
    boost_panel1_boost1_idealDiode_i[0] = gsl_vector_get (__s->x, 3);
    gsl_vector_set (__x, 3, gsl_vector_get (__s->x, 3));
    boost_panel1_boost1_controlledIdealClosingSwitch_s[0] = gsl_vector_get (__s->x, 4);
    gsl_vector_set (__x, 4, gsl_vector_get (__s->x, 4));
    boost_panel1_boost1_controlledIdealClosingSwitch_p_i[0] = gsl_vector_get (__s->x, 5);
    gsl_vector_set (__x, 5, gsl_vector_get (__s->x, 5));
    boost_panel1_boost1_controlledIdealClosingSwitch_p_v[0] = gsl_vector_get (__s->x, 6);
    gsl_vector_set (__x, 6, gsl_vector_get (__s->x, 6));
    boost_panel1_boost1_idealDiode_p_i[0] = gsl_vector_get (__s->x, 7);
    gsl_vector_set (__x, 7, gsl_vector_get (__s->x, 7));
    gsl_multiroot_fsolver_free (__s);
}

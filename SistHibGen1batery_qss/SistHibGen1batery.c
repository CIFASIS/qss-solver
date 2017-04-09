#include <gsl/gsl_multiroots.h>
#define pre(X) X
int fsolve1_eval(const gsl_vector * __x, void * __p, gsl_vector * __f) {
  double *args=(double*)__p;
  const double boost_panel1_panel_exponente = gsl_vector_get(__x,0);
  const double boost_panel1_panel_n_i = gsl_vector_get(__x,1);
  const double boost_panel1_capacitor1_v = args[0];
  gsl_vector_set (__f,0, (boost_panel1_panel_exponente) - (1.6e-19*(boost_panel1_capacitor1_v/60+boost_panel1_panel_n_i*0.01/1)/6.58222e-21));
  gsl_vector_set (__f,1, (boost_panel1_panel_n_i-6.78587+6.51513e-07*(exp(boost_panel1_panel_exponente)-1)) - (0));
  return GSL_SUCCESS;
}
void fsolve1(double boost_panel1_capacitor1_v,double *boost_panel1_panel_exponente,double *boost_panel1_panel_n_i) { 
  size_t __iter = 0;
  int __status,i;
  const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
  gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 2);
  gsl_multiroot_function __F;
  static gsl_vector *__x = NULL;
  if (__x==NULL) {
    __x=gsl_vector_alloc(2);
    for (i=0;i<2;i++)
      gsl_vector_set (__x, i,0);
  }
  __F.n = 2;
  __F.f = fsolve1_eval;
  double __args[1];
  __args[0] = boost_panel1_capacitor1_v;
   __F.params  = __args;
   gsl_multiroot_fsolver_set (__s, &__F,__x);
   do {
     __iter++;
     __status = gsl_multiroot_fsolver_iterate (__s);
     if (__status)   /* check if solver is stuck */
       break;
       __status = gsl_multiroot_test_residual (__s->f, 1e-7);
   } while (__status == GSL_CONTINUE && __iter < 100);
   if (__iter == 100) printf("Warning: GSL could not solve an algebraic loop after %d iterations\n", __iter); 
  boost_panel1_panel_exponente[0] = gsl_vector_get(__s->x,0);
  gsl_vector_set (__x, 0 , gsl_vector_get(__s->x, 0));
  boost_panel1_panel_n_i[0] = gsl_vector_get(__s->x,1);
  gsl_vector_set (__x, 1 , gsl_vector_get(__s->x, 1));
   gsl_multiroot_fsolver_free (__s);
}
int fsolve2_eval(const gsl_vector * __x, void * __p, gsl_vector * __f) {
  double *args=(double*)__p;
  const double boost_panel2_panel_exponente = gsl_vector_get(__x,0);
  const double boost_panel2_panel_n_i = gsl_vector_get(__x,1);
  const double boost_panel2_capacitor1_v = args[0];
  gsl_vector_set (__f,0, (boost_panel2_panel_exponente) - (1.6e-19*(boost_panel2_capacitor1_v/60+boost_panel2_panel_n_i*0.01/1)/6.58222e-21));
  gsl_vector_set (__f,1, (boost_panel2_panel_n_i-6.36176+6.51513e-07*(exp(boost_panel2_panel_exponente)-1)) - (0));
  return GSL_SUCCESS;
}
void fsolve2(double boost_panel2_capacitor1_v,double *boost_panel2_panel_exponente,double *boost_panel2_panel_n_i) { 
  size_t __iter = 0;
  int __status,i;
  const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
  gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 2);
  gsl_multiroot_function __F;
  static gsl_vector *__x = NULL;
  if (__x==NULL) {
    __x=gsl_vector_alloc(2);
    for (i=0;i<2;i++)
      gsl_vector_set (__x, i,0);
  }
  __F.n = 2;
  __F.f = fsolve2_eval;
  double __args[1];
  __args[0] = boost_panel2_capacitor1_v;
   __F.params  = __args;
   gsl_multiroot_fsolver_set (__s, &__F,__x);
   do {
     __iter++;
     __status = gsl_multiroot_fsolver_iterate (__s);
     if (__status)   /* check if solver is stuck */
       break;
       __status = gsl_multiroot_test_residual (__s->f, 1e-7);
   } while (__status == GSL_CONTINUE && __iter < 100);
   if (__iter == 100) printf("Warning: GSL could not solve an algebraic loop after %d iterations\n", __iter); 
  boost_panel2_panel_exponente[0] = gsl_vector_get(__s->x,0);
  gsl_vector_set (__x, 0 , gsl_vector_get(__s->x, 0));
  boost_panel2_panel_n_i[0] = gsl_vector_get(__s->x,1);
  gsl_vector_set (__x, 1 , gsl_vector_get(__s->x, 1));
   gsl_multiroot_fsolver_free (__s);
}
int fsolve3_eval(const gsl_vector * __x, void * __p, gsl_vector * __f) {
  double *args=(double*)__p;
  const double boost_panel3_panel_exponente = gsl_vector_get(__x,0);
  const double boost_panel3_panel_n_i = gsl_vector_get(__x,1);
  const double boost_panel3_capacitor1_v = args[0];
  gsl_vector_set (__f,0, (boost_panel3_panel_exponente) - (1.6e-19*(boost_panel3_capacitor1_v/60+boost_panel3_panel_n_i*0.01/1)/6.58222e-21));
  gsl_vector_set (__f,1, (boost_panel3_panel_n_i-6.95552+6.51513e-07*(exp(boost_panel3_panel_exponente)-1)) - (0));
  return GSL_SUCCESS;
}
void fsolve3(double boost_panel3_capacitor1_v,double *boost_panel3_panel_exponente,double *boost_panel3_panel_n_i) { 
  size_t __iter = 0;
  int __status,i;
  const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
  gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 2);
  gsl_multiroot_function __F;
  static gsl_vector *__x = NULL;
  if (__x==NULL) {
    __x=gsl_vector_alloc(2);
    for (i=0;i<2;i++)
      gsl_vector_set (__x, i,0);
  }
  __F.n = 2;
  __F.f = fsolve3_eval;
  double __args[1];
  __args[0] = boost_panel3_capacitor1_v;
   __F.params  = __args;
   gsl_multiroot_fsolver_set (__s, &__F,__x);
   do {
     __iter++;
     __status = gsl_multiroot_fsolver_iterate (__s);
     if (__status)   /* check if solver is stuck */
       break;
       __status = gsl_multiroot_test_residual (__s->f, 1e-7);
   } while (__status == GSL_CONTINUE && __iter < 100);
   if (__iter == 100) printf("Warning: GSL could not solve an algebraic loop after %d iterations\n", __iter); 
  boost_panel3_panel_exponente[0] = gsl_vector_get(__s->x,0);
  gsl_vector_set (__x, 0 , gsl_vector_get(__s->x, 0));
  boost_panel3_panel_n_i[0] = gsl_vector_get(__s->x,1);
  gsl_vector_set (__x, 1 , gsl_vector_get(__s->x, 1));
   gsl_multiroot_fsolver_free (__s);
}
int fsolve4_eval(const gsl_vector * __x, void * __p, gsl_vector * __f) {
  double *args=(double*)__p;
  const double boost_panel4_panel_exponente = gsl_vector_get(__x,0);
  const double boost_panel4_panel_n_i = gsl_vector_get(__x,1);
  const double boost_panel4_capacitor1_v = args[0];
  gsl_vector_set (__f,0, (boost_panel4_panel_exponente) - (1.6e-19*(boost_panel4_capacitor1_v/60+boost_panel4_panel_n_i*0.01/1)/6.58222e-21));
  gsl_vector_set (__f,1, (boost_panel4_panel_n_i-5.0894+6.51513e-07*(exp(boost_panel4_panel_exponente)-1)) - (0));
  return GSL_SUCCESS;
}
void fsolve4(double boost_panel4_capacitor1_v,double *boost_panel4_panel_exponente,double *boost_panel4_panel_n_i) { 
  size_t __iter = 0;
  int __status,i;
  const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
  gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc (__T, 2);
  gsl_multiroot_function __F;
  static gsl_vector *__x = NULL;
  if (__x==NULL) {
    __x=gsl_vector_alloc(2);
    for (i=0;i<2;i++)
      gsl_vector_set (__x, i,0);
  }
  __F.n = 2;
  __F.f = fsolve4_eval;
  double __args[1];
  __args[0] = boost_panel4_capacitor1_v;
   __F.params  = __args;
   gsl_multiroot_fsolver_set (__s, &__F,__x);
   do {
     __iter++;
     __status = gsl_multiroot_fsolver_iterate (__s);
     if (__status)   /* check if solver is stuck */
       break;
       __status = gsl_multiroot_test_residual (__s->f, 1e-7);
   } while (__status == GSL_CONTINUE && __iter < 100);
   if (__iter == 100) printf("Warning: GSL could not solve an algebraic loop after %d iterations\n", __iter); 
  boost_panel4_panel_exponente[0] = gsl_vector_get(__s->x,0);
  gsl_vector_set (__x, 0 , gsl_vector_get(__s->x, 0));
  boost_panel4_panel_n_i[0] = gsl_vector_get(__s->x,1);
  gsl_vector_set (__x, 1 , gsl_vector_get(__s->x, 1));
   gsl_multiroot_fsolver_free (__s);
}

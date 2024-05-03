#ifndef _RETQSS_BOIDS_H_
#define _RETQSS_BOIDS_H_


#ifdef __cplusplus

#include <retqss_cgal_main_types.hh>

extern "C"
{

int process_volume(int,int,Vector_3&,Vector_3&,Vector_3&);
#endif

int boids_update(int);
int is_in_neighborhood(int,int,double,double,double*);

#ifdef __cplusplus
}
#endif

#endif

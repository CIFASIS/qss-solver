/**
 * \file retqss_cgal_main_types.hh
 * \brief Convenience type definitions for CGAL objects
 */

#ifndef _RETQSS_CGAL_MAIN_TYPES_H_
#define _RETQSS_CGAL_MAIN_TYPES_H_

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Aff_transformation_3.h>

#include <CGAL/Polygon_mesh_processing/distance.h>
#include <CGAL/Linear_algebraCd.h>

/**
 * \brief CGAL kernel used by retQSS
 *
 * We use the simple cartesian kernel to represent CGAL objects using cartesian
 * coordinates with values in the field of real numbers.
 */
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Ray_3 Ray_3;
typedef Kernel::Triangle_3 Triangle_3;
typedef Kernel::Plane_3 Plane_3;
typedef Kernel::Iso_cuboid_3 Iso_cuboid_3;
typedef CGAL::Vector_3<Kernel> Vector_3;
typedef CGAL::Aff_transformation_3<Kernel> Transformation;
typedef CGAL::Surface_mesh<Point_3> CGALPolyhedron;

typedef CGAL::Linear_algebraCd<double>::Matrix LA_Matrix;
typedef CGAL::Linear_algebraCd<double>::Vector LA_Vector;

#endif

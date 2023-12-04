/**
 * \file retqss_cgal_aabb_tree_types.hh
 * \brief Convenience type definitions for CGAL AABB trees
 *
 * We use two types of AABB trees: trees storing faces of a single polyhedron
 * and trees storing faces of several polyhedrons, which use as primitives
 * ::Primitive_without_mesh and ::Primitive_with_mesh, respectively. The latter
 * stores, for each face, a pointer to the surface mesh where that face came
 * from, which is needed to retrieve the underlying polyhedron used in
 * intersection queries.
 *
 * \sa retQSS::AABBTreeForSinglePolyhedron,
 * retQSS::AABBTreeForMultiplePolyhedrons
 */

#ifndef _RETQSS_CGAL_H_
#define _RETQSS_CGAL_H_

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/Side_of_triangle_mesh.h>

#include "retqss_cgal_main_types.hh"
#include "retqss_polyhedron.hh"


/**
 * Primitive for an AABB tree over a single surface mesh.
 * Used by retQSS::AABBTreeForSinglePolyhedron.
 */
typedef CGAL::AABB_face_graph_triangle_primitive<
			retQSS::Polyhedron
		> Primitive_without_mesh;
typedef CGAL::AABB_traits<Kernel, Primitive_without_mesh> Traits_without_mesh;
typedef CGAL::AABB_tree<Traits_without_mesh> AABB_tree;
typedef boost::optional<
			AABB_tree::Intersection_and_primitive_id<Ray_3>::Type
		> Ray_intersection_for_standard_tree;
typedef CGAL::Side_of_triangle_mesh<
			retQSS::Polyhedron,
			Kernel,
			CGAL::Default,
			AABB_tree
		> Standard_point_locator;

/**
 * Primitive for an AABB tree over several surface meshes.
 * Used by retQSS::AABBTreeForMultiplePolyhedrons.
 */
typedef CGAL::AABB_face_graph_triangle_primitive<
			retQSS::Polyhedron,
			CGAL::Default,
			CGAL::Tag_false,
			CGAL::Tag_false
		> Primitive_with_mesh;
typedef CGAL::AABB_traits<Kernel, Primitive_with_mesh> Traits_with_mesh;
typedef CGAL::AABB_tree<Traits_with_mesh> AABB_tree_with_mesh_primitive;
typedef boost::optional<
			AABB_tree_with_mesh_primitive::Intersection_and_primitive_id<Ray_3>::Type
		> Ray_intersection_for_custom_tree;
typedef CGAL::Side_of_triangle_mesh<
			retQSS::Polyhedron,
			Kernel,
			CGAL::Default,
			AABB_tree_with_mesh_primitive
		> Custom_point_locator;

#endif

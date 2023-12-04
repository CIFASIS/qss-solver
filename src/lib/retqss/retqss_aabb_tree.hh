/**
 * \file retqss_aabb_tree.hh
 * \brief Wrappers of AABB trees implemented by CGAL
 */

#ifndef _RETQSS_AABB_TREE_H_
#define _RETQSS_AABB_TREE_H_

#include <unordered_set>
#include <utility>

#include "retqss_cgal_main_types.hh"
#include "retqss_cgal_aabb_tree_types.hh"
#include "retqss_polyhedron.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_types.hh"


/**
 * \namespace retQSS Main retQSS namespace
 */
namespace retQSS
{

/**
 * \brief Abstract base class of AABB trees
 *
 * AABB trees are essential for fast intersection or distance queries regarding
 * surface meshes (retQSS::Polyhedron). These queries are performed by retQSS
 * during the simulation e.g. to find a target face whose neighborhood will be
 * tested to detect a boundary crossing (retQSS::FaceCrossingAlgorithm). In
 * turn, this usually happens when a particle is entering the geometry (i.e.,
 * it is inside volume 0 and about enter one of the polyhedral volumes), but it
 * also may happen when crossing the boundaries of regular polyhedral volumes
 * if they have a large number of facets. The face crossing algorithm defines a
 * threshold to decide whether to test every facet or evaluate a reduced
 * neighborhood of a target face found using such an intersection query.
 */
class AABBTree
{
protected:
	void check_query_result(void*) const;

	virtual double squared_distance_to(const Point_3&) = 0;

public:
	AABBTree() {};
	virtual ~AABBTree() {};

	/**
	 * \brief Find first face crossed by ray.
	 *
	 * Finds the first face crossed by the given ray along with the point where
	 * both objects meet.
	 *
	 * \note Must be implemented by subclasses.
	 *
	 * \param ray CGAL ray.
	 * \param face_to_skip An optional face that can be supplied so that it is
	 * ignored in the intersection query.
	 * \return A tuple formed by the face pointer and the point (the former set
	 * to null if there is no such intersection).
	 */
	virtual retQSS::FaceAndPoint crossed_face(
			const Ray_3 &ray,
			retQSS::PolyhedronFace *face_to_skip =nullptr) = 0;

	/**
	 * \brief Close internal tree data structures.
	 *
	 * Closes internal CGAL AABB tree data structures.
	 *
	 * \note Must be implemented by subclasses.
	 */
	virtual void close() = 0;

	/**
	 * \brief Find distance to a point.
	 *
	 * Finds the shortest distance of the primitives stored in the tree to a
	 * given point.
	 *
	 * \param point A 3D point.
	 * \return The shortest distance to the point.
	 */
	double distance_to(const Point_3 &point);
};

/**
 * \brief AABB tree storing faces of a single surface mesh
 *
 * This kind of tree is used to solve intersection and distance queries over a
 * single polyhedron. One such tree is kept as an attribute of polyhedral
 * volumes (retQSS::PolyhedralVolume).
 */
class AABBTreeForSinglePolyhedron : public retQSS::AABBTree
{
private:
	AABB_tree tree;
	retQSS::Polyhedron *poly;
	Standard_point_locator *locator;

protected:
	virtual double squared_distance_to(const Point_3&);

public:
	AABBTreeForSinglePolyhedron(retQSS::Polyhedron*);
	virtual ~AABBTreeForSinglePolyhedron() {};

	virtual retQSS::FaceAndPoint crossed_face(
			const Ray_3&,
			retQSS::PolyhedronFace* =nullptr);

	virtual void close()
	{
		this->tree.accelerate_distance_queries();
	}

	/**
	 * \brief Build point locator.
	 *
	 * Builds a CGAL object that can be used to locate 3D points relative to
	 * the underlying AABB tree (i.e., given a point, this object can tell
	 * whether it is outside the mesh, inside the mesh or lying on its
	 * surface).
	 *
	 * \return A point locator for the given tree.
	 */
	inline Standard_point_locator *point_locator()
	{
		if(this->locator == nullptr)
			this->locator = new Standard_point_locator(this->tree);
		return this->locator;
	}
};


/**
 * \brief AABB tree storing faces of several surface meshes
 *
 * This kind of tree is used to solve intersection and distance queries
 * over several polyhedrons. One such tree is kept as an attribute of
 * volume 0 (retQSS::OpenVolume), which represents the exterior of the
 * geometry. As such, different facets might possibly correspond to
 * different polyhedral volumes, and thus it is important to retrieve
 * a reference to the underlying polyhedron in intersection/distance
 * queries.
 */
class AABBTreeForMultiplePolyhedrons : public retQSS::AABBTree
{
private:
	AABB_tree_with_mesh_primitive tree;
	Custom_point_locator *locator;

protected:
	virtual double squared_distance_to(const Point_3&);

public:
	typedef std::pair<
				boost::graph_traits<retQSS::Polyhedron>::face_descriptor,
				const retQSS::Polyhedron*> PrimitiveID;

	AABBTreeForMultiplePolyhedrons() : locator(nullptr) {};
	virtual ~AABBTreeForMultiplePolyhedrons() {};

	void insert(retQSS::PolyhedronFace*);
	virtual retQSS::FaceAndPoint crossed_face(
			const Ray_3&,
			retQSS::PolyhedronFace* =nullptr);

	virtual void close()
	{
		this->tree.accelerate_distance_queries();
	}

	/**
	 * \brief Build point locator.
	 *
	 * Builds a CGAL object that can be used to locate 3D points relative to
	 * the underlying AABB tree (i.e., given a point, this object can tell
	 * whether it is outside the mesh, inside the mesh or lying on its
	 * surface).
	 *
	 * \return A point locator for the given tree.
	 */
	inline Custom_point_locator *point_locator()
	{
		if(this->locator == nullptr)
			this->locator = new Custom_point_locator(this->tree);
		return this->locator;
	}
};

/**
 * \brief CGAL functor to skip faces from intersection queries
 */
struct FaceSkippingFunctor
{
	std::unordered_set<FaceIdx> subfaces;
	const retQSS::PolyhedronFace *face;

	FaceSkippingFunctor(const retQSS::PolyhedronFace *face)
		: face(face)
	{
		for(auto subface_idx : face->subfaces())
			this->subfaces.insert(subface_idx);
	}

	bool operator()(const AABBTreeForMultiplePolyhedrons::PrimitiveID &p) const
	{
		auto face_idx = p.first;
		auto vol = p.second->volume();

		return
				vol == this->face->volume() &&
				this->subfaces.find(face_idx) != this->subfaces.end();
	}
};

}

#endif

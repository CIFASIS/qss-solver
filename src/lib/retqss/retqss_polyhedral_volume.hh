/**
 * \file retqss_polyhedral_volume.hh
 */

#ifndef _RETQSS_POLYHEDRAL_VOLUME_H_
#define _RETQSS_POLYHEDRAL_VOLUME_H_

#include <fstream>
#include <unordered_map>
#include <string>

#include "retqss_cgal_main_types.hh"
#include "retqss_cgal_aabb_tree_types.hh"
#include "retqss_face_neighborhood.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_volume.hh"
#include "retqss_polyhedron.hh"
#include "retqss_types.hh"


namespace retQSS
{

class Geometry;


/**
 * \brief Representation of a volume supported by a 3D polyhedron
 *
 * This class represents a geometrical volume that is supported by a convex 3D
 * polyhedron. This kind of volumes conform a tessellated 3D mesh of the domain
 * described by the geometry.
 */
class PolyhedralVolume : public retQSS::Volume
{
	friend class retQSS::Geometry;
	friend class retQSS::Polyhedron;

private:
	retQSS::Polyhedron *poly; //!< Underlying 3D convex polyhedron

	Standard_point_locator *locator; //!< CGAL data structure to locate points
	                                 //!< relative to the polyhedron

	bool on_boundary; //!< Whether the volume is on the geometry boundary

	/**
	 * \brief Set up polyhedral volume
	 *
	 * - Close the underlying 3D polyhedron so that faces and neighbors are
	 * computed
	 * - Create an internal AABB tree for geometrical queries
	 * - Set static properties of the polyhedral volume
	 */
	void initialize();

	/**
	 * \brief Initialize underlying data structures
	 *
	 * Check and eventually initalize the underlying data structures for
	 * intersection and distance queries
	 */
	virtual void ensure_query_structures_are_assembled();

	/**
	 * \brief Map point to vertex object
	 *
	 * Map the given point to the corresponding vertex object.
	 */
	retQSS::Vertex *point_to_vertex(const Point_3 &point) const;

	/**
	 * \brief Constructor
	 *
	 * Create a new polyhedral volume supported by the given 3D polyhedron and
	 * vertices.
	 */
	PolyhedralVolume(
			VolumeID v,
			const std::string &name,
			retQSS::Polyhedron *poly,
			const retQSS::VertexArray &vertices);

public:
	PolyhedralVolume() = delete;

	virtual ~PolyhedralVolume();

	/**
	 * \brief Check whether the volume is polyhedral
	 */
	virtual bool is_polyhedral() const;

	/**
	 * \brief Compute the location of a point relative to the volume
	 *
	 * Compute the location of the given point relative to the volume:
	 *  - retQSS::POINT_INSIDE if the point is inside the volume,
	 *  - retQSS::POINT_OUTSIDE if the point is outside the volume, and
	 *  - retQSS::POINT_ON_BOUNDARY if the point lies in one of the faces of
	 *  the volume
	 */
	virtual retQSS::PointLocation locate_point(const Point_3 &point);

	/**
	 * \brief Check whether a vector points outside the volume
	 *
	 * Check whether following the direction given by the vector leads to the
	 * exterior of the volume, when positioned at the given face. In other
	 * words, check whether the angle between this vector and the outward
	 * normal vector of the face is acute.
	 */
	virtual bool is_exit_direction(
			const retQSS::PolyhedronFace *face,
			const Vector_3 &dir);

	/**
	 * \brief Compute a random point inside the volume
	 *
	 * 	Compute a random point inside the volume. This method implements a
	 * 	hit-and-run sampling algorithm to find such a point (see e.g.
	 * 	'Convergence properties of Hit-and-Run samplers').
	 */
	Point_3 random_point();

	/**
	 * \brief Retrieve the underlying 3D polyhedron
	 */
	inline retQSS::Polyhedron *get_polyhedron() const
	{
		return this->poly;
	}

	/**
	 * \brief Get adjacent neighboring faces
	 *
	 * Get the faces in the volume that are immediate adjacent neighbors of the
	 * given face (i.e., those that share a common edge with it).
	 */
	inline const retQSS::FaceNeighbors&
	get_face_neighbors(retQSS::PolyhedronFace *face) const
	{
		return face->get_polyhedron_neighbors();
	}

	/**
	 * \brief Check whether a face is not on boundary
	 */
	inline bool is_internal_face(const retQSS::PolyhedronFace *face)
	{
		if(face->volume() != this) return false;

		auto it = this->face_to_face.find(face);
		return it != this->face_to_face.end() && it->second[0] != face;
	}

	/**
	 * \brief Compute volume centroid
	 */
	inline Point_3 centroid() const
	{
		return this->poly->centroid();
	}

	/**
	 * \brief Compute volume capacity
	 */
	inline double capacity() const
	{
		return this->poly->capacity();
	}

	/**
	 * \brief Compute rectangular box bounding this volume
	 */
	inline Iso_cuboid_3 bounding_box()
	{
		retQSS::Lock lock(this->cgal_mutex);
		return this->poly->bounding_box();
	}

	/**
	 * \brief Check whether the volume is on the geometry boundary
	 */
	inline bool is_on_boundary() const
	{
		return this->on_boundary;
	}

	/**
	 * \brief Export volume to OFF file
	 */
	inline void save_to_OFF(const std::string &path =std::string())
	{
		std::ofstream file(path + "/" + this->get_name() + ".off");
		file << *this->poly;
		file.close();
	}
};

}

#endif

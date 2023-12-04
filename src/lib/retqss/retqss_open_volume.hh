/**
 * \file retqss_open_volume.hh
 */

#ifndef _RETQSS_OPEN_VOLUME_H_
#define _RETQSS_OPEN_VOLUME_H_

#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "retqss_cgal_main_types.hh"
#include "retqss_cgal_aabb_tree_types.hh"
#include "retqss_face_neighborhood.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_polyhedral_volume.hh"
#include "retqss_volume.hh"
#include "retqss_vertex.hh"
#include "retqss_polyhedron.hh"


namespace retQSS
{

class Geometry;

/**
 * \brief Representation of an open volume
 *
 * An \em open \em volume is a special kind of volume represents the exterior
 * of the tessellated 3D mesh conformed by the polyhedral volumes that shape
 * the geometry. The geometry object (retQSS::Geometry) keeps track of a single
 * instance of this class, assigning to it volume ID 0.
 */
class OpenVolume : public retQSS::Volume
{
	friend class retQSS::Geometry;

private:
	bool closed;

	std::unordered_map<
		retQSS::Vertex*,
		std::list<PolyhedronFace*>
	> vertex_to_faces; //!< Lookup table to map a vertex to the sequence of
	                   //!< faces have it (data structure cleared once the
	                   //!< volume is closed)

	std::unordered_set<retQSS::Vertex*>
		vertices_added; //!< Vertices already added (data structured cleared
	                    //!< once the volume is closed)

	Custom_point_locator *locator; //!< CGAL data structure to locate points
	                               //!< relative to the volume

	/**
	 * \brief Add new face to the volume
	 *
	 * Add the given face to the volume, which will be inserted into the
	 * internal AABB tree and checked for other neighboring faces previously
	 * added (i.e., sharing common vertices).
	 *
	 * \note The face should be a boundary face
	 */
	void add_face(retQSS::PolyhedronFace *face);

	/**
	 * \brief Add new vertex to the volume
	 *
	 * Add a new vertex to the volume, which should belong to a previously
	 * added boundary face.
	 */
	void add_vertex(retQSS::Vertex *vertex);

	/**
	 * \brief Close volume
	 *
	 * Close volume. No additional faces or vertices can be inserted once the
	 * volume is closed.
	 */
	void close();

	/**
	 * \brief Constructor
	 *
	 * Create a new open volume with the given ID and name.
	 */
	OpenVolume(VolumeID v, const std::string &name);

protected:
	/**
	 * \brief Initialize underlying data structures
	 *
	 * Check and eventually initialize the underlying data structures for
	 * intersection and distance queries
	 */
	virtual void ensure_query_structures_are_assembled();

public:
	OpenVolume() = delete;

	virtual ~OpenVolume();

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
	 * normal vector of the face is obtuse (since exiting the volume is
	 * intepreted as entering the geometry).
	 */
	virtual bool is_exit_direction(
			const retQSS::PolyhedronFace *face,
			const Vector_3 &dir);

	/**
	 * \brief Get adjacent neighboring faces
	 *
	 * Get the faces in the volume that are immediate adjacent neighbors of the
	 * given face (i.e., those that share a common edge with it).
	 */
	inline const retQSS::FaceNeighbors&
	get_face_neighbors(retQSS::PolyhedronFace *face) const
	{
		return face->get_boundary_neighbors();
	}
};

}

#endif

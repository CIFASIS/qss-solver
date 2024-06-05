/**
 * \file retqss_volume.hh
 */

#ifndef _RETQSS_VOLUME_H_
#define _RETQSS_VOLUME_H_

#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <mutex>

#include "retqss_cgal_main_types.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_face_neighborhood.hh"
#include "retqss_aabb_tree.hh"
#include "retqss_object.hh"
#include "retqss_types.hh"
#include "retqss_vertex.hh"
#include "retqss_object_properties.hh"
#include "retqss_exceptions.hh"


namespace retQSS
{

typedef std::vector<retQSS::PolyhedronFace*> FaceNeighbors;
typedef std::unordered_map<
			const retQSS::PolyhedronFace*,
			FaceNeighbors> FaceNeighborsMap;

class Geometry;
class GeometryParser;
class OFFParser;
class VTKParser;
class Interface;

/**
 * \brief Representation of geometrical 3D volumes
 *
 * A \em volume is a 3D object that conforms the geometry throughout which
 * particles navigate along a retQSS simulation. A geometry is typically
 * composed of two different kinds of volumes: \em polyhedral \em volumes
 * (retQSS::PolyhedralVolume), supported by 3D convex polyhedrons, and a
 * single \em open \em volume, (retQSS::OpenVolume), which represents the
 * exterior of the tessellated 3D mesh conformed by these polyhedral
 * volumes.
 *
 * A volume is aware of its neighboring adjacent volumes, which are
 * those that share common logical faces (retQSS::LogicalFace). When a particle
 * crosses a volume boundary, the corresponding adjacent volume is retrieved
 * so that the particle state is updated.
 *
 * \paragraph Queries Intersection and distance queries
 *
 * Intersection (with directed linear rays) and distance queries with volumes
 * are supported by an underlying AABB tree data structure (retQSS::AABBTree).
 * This tree varies according to the type of the volume: for polyhedral
 * volumes, every face stored in the tree belongs to the same underlying
 * polyhedron, whereas an open volume spans faces from several polyhedrons
 * --those that are on the geometry boundary.
 */
class Volume : public retQSS::Object
{
	friend class retQSS::Geometry;
	friend class retQSS::GeometryParser;
	friend class retQSS::OFFParser;
	friend class retQSS::VTKParser;
	friend class retQSS::Interface;

protected:
	retQSS::VertexArray _vertices; //!< Vertices
	std::vector<retQSS::PolyhedronFace*> _faces; //! Physical faces

	retQSS::FaceNeighborsMap face_to_face; //!< Lookup table that maps a
	                                       //!< physical face to a neighbor
	                                       //!< face in an adjacent volume

	std::vector<const retQSS::Volume*> adjacent_volumes; //!< Adjacent volumes

	retQSS::AABBTree *tree; //!< AABB tree for intersection/distance queries

	retQSS::ParticleIDSet _particles_inside; //!< IDs of particles inside

	std::mutex cgal_mutex; //!< Mutex to protect concurrent access to
	                       //!< CGAL algorithms (for parallel simulations)

	/**
	 * \brief Initialize underlying data structures
	 *
	 * Check and eventually initalize the underlying data structures for
	 * intersection and distance queries
	 */
	virtual void ensure_query_structures_are_assembled() = 0;

	/**
	 * \brief Check whether a volume is an adjacent neighbor and set it
	 *
	 * Check whether the given volume is an adjacent neighbor, testing every
	 * local physical face for matching faces in the volume. In case a match is
	 * found, the volume is added as an adjacent neighbor, and reciprocally the
	 * current volume is added as an adjacent neighbor of the volume.
	 *
	 * This is called by the geometry parser (retQSS::GeometryParser) upon
	 * creating a new polyhedral volume. Unmatched faces are later gathered and
	 * classified with a different algorithm (see
	 * retQSS::GeometryParser::classify_unmatched_faces()).
	 */
	void check_neighbor(retQSS::Volume *vol);

	/**
	 * \brief Add new adjacent neighbor volume
	 *
	 * Set the given volume as adjacent neighbor, reached through the given local
	 * face.
	 *
	 * \note If <tt>neighbor</tt> is not defined, it is initialized after the
	 * owning volume of <tt>neighbor_face</tt>.
	 *
	 * \pre The given physical faces match with each other
	 */
	void add_neighbor(
			retQSS::PolyhedronFace *local_face,
			retQSS::PolyhedronFace *neighbor_face,
			retQSS::Volume *neighbor =nullptr);

public:
	/**
	 * \brief Volume constructor
	 *
	 * Create a new volume with the given ID and name.
	 */
	Volume(VolumeID v, const std::string &name);

	virtual ~Volume();

	/**
	 * \brief Check whether the volume is polyhedral
	 */
	virtual bool is_polyhedral() const;

	/**
	 * \brief Check whether a vector points outside the volume
	 *
	 * Check whether following the direction given by the vector leads to the
	 * exterior of the volume, when positioned at the given face.
	 */
	virtual bool is_exit_direction(
			const retQSS::PolyhedronFace *face,
			const Vector_3 &dir) = 0;

	/**
	 * \brief Compute the location of a point relative to the volume
	 *
	 * Compute the location of the given point relative to the volume:
	 *  - retQSS::POINT_INSIDE if the point is inside the volume,
	 *  - retQSS::POINT_OUTSIDE if the point is outside the volume, and
	 *  - retQSS::POINT_ON_BOUNDARY if the point lies in one of the faces of
	 *  the volume
	 */
	virtual retQSS::PointLocation locate_point(const Point_3 &point) = 0;

	/**
	 * \brief Retrieve volumes adjacent to a face
	 *
	 * Get the sequence of volumes that can be reached through the given
	 * physical face (there can be more than one if this face is partitioned
	 * into several smaller faces owned by other polyhedrons).
	 */
	retQSS::VolumeArray get_volumes_adjacent_to(
			retQSS::PolyhedronFace *face) const;

	/**
	 * \brief Retrieve the number of physical faces of the volume
	 */
	inline size_t num_faces() const
	{
		return this->_faces.size();
	}

	/**
	 * \brief Retrieve the number of vertices of the volume
	 */
	inline size_t num_vertices()
	{
		return this->_vertices.size();
	}

	/**
	 * \brief Retrieve the number of adjacent volumes of the volume
	 */
	inline size_t num_adjacent_volumes() const
	{
		return this->adjacent_volumes.size();
	}

	/**
	 * \brief Retrieve phyisical faces of the volume
	 */
	inline const std::vector<retQSS::PolyhedronFace*> &faces() const
	{
		return this->_faces;
	}

	/**
	 * \brief Retrieve vertices of the volume
	 */
	inline const retQSS::VertexArray &vertices() const
	{
		return this->_vertices;
	}

	/**
	 * \brief Retrieve a vertex of the volume
	 *
	 * Retrieve the \f$i\f$-th vertex of the volume.
	 */
	inline retQSS::Vertex *get_vertex(Internal_Index i)
	{
		if(i >= this->_vertices.size())
			throw retQSS::InvalidIndexException(this, i, "vertices");

		return this->_vertices[i];
	}

	/**
	 * \brief Retrieve a face of the volume
	 *
	 * Retrieve the \f$i\f$-th physical face of the volume.
	 */
	inline retQSS::PolyhedronFace *get_face(Internal_Index i)
	{
		if(i >= this->_faces.size())
			throw retQSS::InvalidIndexException(this, i, "faces");

		return this->_faces[i];
	}

	/**
	 * \brief Retrieve an adjacent volume of the volume
	 *
	 * Retrieve the \f$i\f$-th adjacent volume of the volume.
	 */
	inline retQSS::Volume *get_adjacent_volume(Internal_Index i) const
	{
		if(i >= this->num_adjacent_volumes())
			throw retQSS::InvalidIndexException(this, i, "adjacent volumes");

		return const_cast<retQSS::Volume*>(this->adjacent_volumes[i]);
	}

	/**
	 * \brief Get particles inside the volume
	 *
	 * Get a set with the IDs of the particles currently inside the volume.
	 */
	inline retQSS::ParticleIDSet particles_inside()
	{
		return this->_particles_inside;
	}

	/**
	 * \brief Get particles inside the volume
	 *
	 * Get a set with the IDs of the particles currently inside the volume.
	 *
	 * \note Returns a reference to avoid copying the set. Should not be used
	 * in parallel mode when iterating particle neighborhoods.
	 */
	inline const retQSS::ParticleIDSet &particles_inside_ref()
	{
		return this->_particles_inside;
	}

	/**
	 * \brief Set entering particle
	 *
	 * Add the given particle to the sequence of particles currently inside the
	 * volume.
	 */
	inline void particle_entering(Internal_ParticleID p)
	{
		this->_particles_inside.insert(p);
	}

	/**
	 * \brief Set exiting particle
	 *
	 * Remove the given particle from the sequence of particles currently
	 * inside the volume.
	 */
	inline void particle_exiting(Internal_ParticleID p)
	{
		this->_particles_inside.erase(p);
	}

	/**
	 * \brief Get a physical face in an adjacent volume
	 *
	 * Get the (unique) physical face \f$f\f$ of an adjacent volume that
	 * satisfies the following conditions:
	 *  - The given local face shares the same logical face with \f$f\f$, and
	 *  - The given point lies inside \f$f\f$.
	 *
	 *  This is used by the particle tracker (retQSS::ParticleTracker) when a
	 *  particle state has to be updated after crossing a volume boundary.
	 *
	 *  \note Null pointer returned if no face satisfies them
	 */
	inline const retQSS::PolyhedronFace *get_neighbor_face(
			const retQSS::PolyhedronFace *face,
			const Point_3 &point) const
	{
		const retQSS::PolyhedronFace *neighbor = nullptr;
		auto it = this->face_to_face.find(face);

		if(it != this->face_to_face.end())
		{
			for(auto target : it->second)
				if(target->is_point_inside(point))
					return target;
		}

		return neighbor;
	}

	/**
	 * \brief Compute intersection of a ray with the volume
	 *
	 * Compute the first intersection of the given ray with one of the faces of
	 * the volume.
	 *
	 * \param face_to_skip An optional face of the volume that should
	 * be ignored for the computation (e.g., if the ray starts at that face)
	 */
	inline retQSS::FaceAndPoint crossed_face(
			const Ray_3 &ray,
			retQSS::PolyhedronFace *face_to_skip =nullptr)
	{
		retQSS::Lock lock(this->cgal_mutex);
		this->ensure_query_structures_are_assembled();
		return this->tree->crossed_face(ray, face_to_skip);
	}

	/**
	 * \brief Compute distance to a point
	 *
	 * Compute distance from the volume to the given point
	 */
	inline double distance_to(const Point_3 &point)
	{
		retQSS::Lock lock(this->cgal_mutex);
		return this->tree->distance_to(point);
	}

	/**
	 * \brief Compute neighborhood of a face
	 *
	 * Compute the neighboring faces of the given face (see
	 * retQSS::FaceNeighborhood).
	 */
	inline retQSS::FaceNeighborhood
	face_neighborhood(retQSS::PolyhedronFace *face)
	{
		return retQSS::FaceNeighborhood(this, face, _NEIGHBORHOOD_DEPTH);
	}

	/**
	 * \brief Get adjacent neighboring faces
	 *
	 * Get the faces in the volume that are immediate adjacent neighbors of the
	 * given face (i.e., those that share a common edge with it).
	 */
	virtual const retQSS::FaceNeighbors&
	get_face_neighbors(retQSS::PolyhedronFace *face) const = 0;
};

}

#endif

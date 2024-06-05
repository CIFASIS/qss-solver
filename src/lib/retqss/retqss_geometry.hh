/**
 * \file retqss_geometry.hh
 */

#ifndef _RETQSS_GEOMETRY_H_
#define _RETQSS_GEOMETRY_H_

#include <list>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include "retqss_types.hh"
#include "retqss_polyhedron.hh"
#include "retqss_polyhedral_volume.hh"
#include "retqss_open_volume.hh"
#include "retqss_volume.hh"
#include "retqss_vertex.hh"
#include "retqss_utilities.hh"
#include "retqss_exceptions.hh"

#define RETQSS_WORLD_NAME	"World"


namespace retQSS
{

/**
 * \brief Geometry class
 *
 * This class keeps track, administrates and mediates access to every
 * geometrical object in retQSS: volumes, faces and vertices. A unique
 * instance of this class is created by the retQSS interface
 * (retQSS::Interface) and initialized by the geometry
 * parser (retQSS::GeometryParser) when instructed by the user (e.g. by
 * calling the model API method retQSS_geometry_setUp). After this, the
 * retQSS interface keeps an internal reference to the geometry and
 * delegates to it every geometry-related query.
 */
class Geometry
{
private:
	retQSS::PolyhedralVolumeArray _volumes;
	retQSS::VertexArray _vertices;
	retQSS::LogicalFaceArray _faces;
	retQSS::OpenVolume *_world; //!< Special volume (with ID 0) that
	                            //!< represents the exterior of the
	                            //!< geometry.

	VolumeID current_volume_id; //!< Keeps track of volume IDs assigned upon
	                            //!< initialization.
	Internal_VertexID current_vertex_id; //!< Keeps track of vertex IDs assigned upon
	                                     //!< initialization.
	Internal_LogicalFaceID current_face_id;  //!< Keeps track of face IDs
	                                         //!< assigned upon
	                                         //!< initialization.

	std::unordered_map<
		Point_3,
		retQSS::Vertex*,
		retQSS::PointHash> vertex_map; //!< Lookup table to map 3D points to
	                                   //!< vertex objects, used when creating
	                                   //!< new volumes.

	std::unordered_map<
	    VolumeID,
		retQSS::Volume*> volume_map;   //!< Lookup table of volumes by volume
	                                   //!< ID.

	std::unordered_map<
		retQSS::Vertex*,
		retQSS::VolumeArray
	> vertex_to_volumes;               //!< Lookup table of volumes that share
	                                   //!< a common vertex.

	bool closed; //!< Whether the geometry has been completely initialized.

	/**
	 * \brief Create logical faces
	 *
	 * Create logical faces, which group different, adjacent physical faces
	 * (retQSS::PolyhedronFace) into a single retQSS object that can be
	 * manipulated through the model API.
	 */
	void add_logical_faces();

public:
	/**
	 * \brief Constructor
	 *
	 * The constructor instanciates an open volume (retQSS::OpenVolume) with
	 * ID 0, which will represent the exterior of the geometry. Polyhedral
	 * volumes (retQSS::PolyhedralVolumes) are created later, when the parser
	 * reads and interprets the geometry file.
	 */
	Geometry();

	/**
	 * \brief Destructor
	 *
	 * The destructor is in charge of freeing memory of vertices and volumes.
	 */
	~Geometry();

	/**
	 * \brief Add a new volume to the geometry
	 *
	 * Add a new polyhedral volume to the geometry, supported by an underlying
	 * polyhedron referenced by \c poly. This low-level 3D object is created
	 * by the geometry parser after the information retrieved by the file
	 * specification.
	 *
	 * This method retrieves the underlying low-level vertices and maps them
	 * to vertex objects, and also adds the new volume to the corresponding
	 * arrays of volumes that share common vertices and to the main volume
	 * lookup table.
	 *
	 * \note Geometry must not be closed.
	 *
	 * \param name Name given to the volume (needs not to be unique).
	 */
	retQSS::PolyhedralVolume *add_volume(
			retQSS::Polyhedron *poly,
			const std::string &name);

	/**
	 * \brief Add a new vertex to the geometry
	 *
	 * Add a new vertex to the geometry represented by the point given as
	 * argument. In case this point was already mapped to a vertex object,
	 * this method retrieves it from the vertex lookup table and returns it.
	 *
	 * \note Geometry must not be closed.
	 */
	retQSS::Vertex *add_vertex(const Point_3 &point);

	/**
	 * \brief Add a new face that is on the geometry boundary
	 *
	 * The face given an argument was found to be on the geometry surface. This
	 * method adds the face and its vertices to volume 0, and the polyhedral
	 * volume that owns it is marked as being on the boundary.
	 *
	 * \note Geometry must not be closed.
	 */
	void add_boundary_face(retQSS::PolyhedronFace *face);

	/**
	 * \brief Close geometry
	 *
	 * Close geometry. No additional volumes, vertices or faces can be inserted
	 * into a closed geometry.
	 */
	void close();

	/**
	 * \brief Compute intersection with the opposite side of the geometry
	 *
	 *	Compute intersection of a ray starting at the face centroid directed to
	 *	the opposite side of the geometry, using the direction of the inward
	 *	normal vector to the face.
	 *
	 *	\return Tuple containing the intersected polyhedron face and the
	 *	intersection point.
	 */
	retQSS::FaceAndPoint opposite_intersection(
			retQSS::PolyhedronFace *face) const;

	/**
	 * \brief Compute intersection with the opposite side of the geometry
	 *
	 *	Compute intersection of a ray starting at a given point and directed to
	 *	the opposite side of the geometry, using the direction of the inward
	 *	normal vector to the face.
	 *
	 *	\return Tuple containing the intersected polyhedron face and the
	 *	intersection point.
	 */
	retQSS::FaceAndPoint opposite_intersection(
			retQSS::PolyhedronFace *face,
			const Point_3 &point) const;

	/**
	 * \brief Compute face at the opposite side of the geometry
	 *
	 *	Compute boundary face opposite to the given one, shooting for this
	 *	purpose a ray starting at the face centroid and using the direction of
	 *	the inward normal vector to the face.
	 */
	retQSS::PolyhedronFace *opposite_face(retQSS::PolyhedronFace *face) const;

	/**
	 * \brief Compute volume at the opposite side of the geometry
	 *
	 *	Compute boundary volume opposite to the given face, shooting for this
	 *	purpose a ray starting at the face centroid and using the direction of
	 *	the inward normal vector to the face.
	 */
	retQSS::Volume *opposite_volume(retQSS::PolyhedronFace *face) const;

	/**
	 * \brief Retrieve volume adjacent to a face and a point inside it
	 *
	 * Retrieve volume adjacent to the given face and the given point, which
	 * is expected to be inside the face. The given volume is the reference
	 * volume that contains the face and that will be used to compute the
	 * adjacent volume (recall that physical faces on the geometry boundary
	 * are shared by the corresponding polyhedral volume and the open volume
	 * outside the geometry).
	 *
	 * \pre <tt>volume</tt> contains <tt>face</tt>
	 * \pre <tt>point</tt> is inside <tt>face</tt>
	 *
	 * \param adjacent_face_ptr Optional. It will point to the adjacent face
	 * in the returned volume if a non-null pointer is given.
	 */
	retQSS::Volume *adjacent_volume(
			retQSS::Volume *volume,
			retQSS::PolyhedronFace *face,
			const Point_3 &point,
			retQSS::PolyhedronFace **adjacent_face_ptr =nullptr) const;

	/**
	 * \brief Find volume containing a point
	 *
	 * Find volume in the geometry that contains point \f$(x,y,z)\f$. In case
	 * the point is on a volume boundary (i.e., one of its faces), direction
	 * \f$(v_x, v_y, v_z)\f$ is used to break ties (since particle will be
	 * initially moving following that vector).
	 */
	retQSS::Volume *locate(
			const Point_3 &point,
			const Vector_3 &direction) const;

	/**
	 * \brief Dump volume to file.
	 *
	 * \sa retQSS_geometry_dumpVolume
	 */
	bool dump_volume(VolumeID id, const std::string &filename) const;

	/**
	 * \brief Dump assorted geometry statistics to file.
	 *
	 * \sa retQSS_geometry_dumpStats
	 */
	bool dump_stats(const std::string &filename) const;

	inline const retQSS::PolyhedralVolumeArray &volumes() const
	{
		return this->_volumes;
	}

	inline const retQSS::VertexArray &vertices() const
	{
		return this->_vertices;
	}

	inline const retQSS::LogicalFaceArray &logical_faces() const
	{
		return this->_faces;
	}

	inline retQSS::OpenVolume *world() const
	{
		return this->_world;
	}

	/**
	 * \brief Retrieve a volume by its ID.
	 *
	 * Retrieve a volume by its ID. A message is printed to the standard output
	 * in case it is not found, and retQSS is stopped.
	 */
	inline retQSS::Volume *get_volume(VolumeID v) const
	{
		auto it = this->volume_map.find(v);
		if(it == this->volume_map.end())
			throw retQSS::InvalidObjectIDException(v, "volume");

		return it->second;
	}

	/**
	 * \brief Retrieve a polyhedral volume by its ID.
	 *
	 * Retrieve a polyhedral volume by its ID. A message is printed to the standard
	 * output in case it is not found, and retQSS is stopped.
	 */
	inline retQSS::PolyhedralVolume *get_polyhedral_volume(
			VolumeID v) const
	{
		auto vol = dynamic_cast<retQSS::PolyhedralVolume*>(
					this->get_volume(v));

		if(vol == nullptr)
			throw retQSS::InvalidObjectIDException(v, "polyhedral volume");

		return vol;
	}

	/**
	 * \brief Retrieve a vertex by its ID.
	 *
	 * Retrieve a vertex by its ID. A message is printed to the standard
	 * output in case it is not found, and retQSS is stopped.
	 */
	inline retQSS::Vertex *get_vertex(Internal_VertexID u) const
	{
		if(u >= this->num_vertices())
			throw retQSS::InvalidObjectIDException(u, "vertex");

		return this->_vertices[u];
	}

	/**
	 * \brief Retrieve array of volumes that share a vertex.
	 *
	 * Retrieve every volume that share the given vertex. A message is printed
	 * to the standard output in case it is not found, and retQSS is stopped.
	 */
	inline retQSS::VolumeArray get_vertex_volumes(Internal_VertexID u) const
	{
		if(u >= this->num_vertices())
			throw retQSS::InvalidObjectIDException(u, "vertex");

		auto vertex = this->_vertices[u];
		return this->get_vertex_volumes(vertex);
	}

	/**
	 * \brief Retrieve array of volumes that share a vertex.
	 *
	 * Retrieve every volume that share the given vertex.
	 */
	inline retQSS::VolumeArray get_vertex_volumes(retQSS::Vertex *vertex) const
	{
		auto it = this->vertex_to_volumes.find(vertex);
		if(it != this->vertex_to_volumes.end())
			return it->second;
		else
			return retQSS::VolumeArray();
	}

	/**
	 * \brief Retrieve a logical face by its ID.
	 *
	 * Retrieve a logical face by its ID. A message is printed to the standard
	 * output in case it is not found, and retQSS is stopped.
	 */
	inline retQSS::LogicalFace *get_logical_face(
			Internal_LogicalFaceID f) const
	{
		if(f >= this->num_logical_faces())
			throw retQSS::InvalidObjectIDException(f, "logical face");

		return this->_faces[f];
	}

	inline int num_polyhedral_volumes() const
	{
		return this->num_volumes() - 1;
	}

	inline unsigned int num_volumes() const
	{
		return this->volume_map.size();
	}

	inline size_t num_vertices() const
	{
		return this->_vertices.size();
	}

	inline size_t num_logical_faces() const
	{
		return this->_faces.size();
	}
};

}

#endif

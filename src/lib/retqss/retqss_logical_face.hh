/**
 * \file retqss_logical_face.hh
 */

#ifndef _RETQSS_FACE_H_
#define _RETQSS_FACE_H_

#include <string>
#include <vector>

#include "retqss_polyhedron_face.hh"
#include "retqss_object.hh"
#include "retqss_types.hh"
#include "retqss_cgal_main_types.hh"


namespace retQSS
{

/**
 * \brief Representation of logical faces
 *
 * A logical face is a retQSS object that groups different, adjacent physical
 * faces (i.e., instances of retQSS::PolyhedronFace). As such, faces become
 * accessible from the model API.
 *
 * \note It is important to take into consideration that, in those geometries
 * in which a face is partitioned into several smaller faces from different
 * polyhedrons, the logical face assembled will span the whole face. It is
 * not possible to e.g. assign properties or interact via the model API with
 * those smaller faces that make up the face.
 */
class LogicalFace : public retQSS::Object
{
private:
	retQSS::PolyhedronFace *_primary_face;
	std::vector<retQSS::PolyhedronFace*> _secondary_faces;

public:
	LogicalFace(
		Internal_LogicalFaceID f,
		retQSS::PolyhedronFace *primary_face);

	virtual ~LogicalFace() {};

	/**
	 * \brief Add a phyisical face to this logical face
	 *
	 * Add the given face to the collection of secondary faces of the
	 * current logical face.
	 */
	void add_physical_face(retQSS::PolyhedronFace *face);

	/**
	 * \brief Check whether this face is on the geometry surface
	 *
	 * For this to happen, the primary face must be the only secondary face
	 * available (see retQSS::Geometry::add_boundary_face).
	 */
	inline bool is_on_boundary() const
	{
		return
			this->_secondary_faces.size() == 1 &&
			this->_primary_face == this->_secondary_faces[0];
	}

	/**
	 * \brief Retrieve primary physical face
	 *
	 * Retrieve the primary physical face represented by this logical face.
	 */
	inline retQSS::PolyhedronFace *primary_face() const
	{
		return this->_primary_face;
	}

	/**
	 * \brief Get the a normal vector to this face
	 *
	 * Note that vector orientation cannot be ensured as this face is shared
	 * among different polyhedrons.
	 */
	inline Vector_3 normal() const
	{
		return this->_primary_face->normal();
	}

	/**
	 * \brief Get the centroid of this face
	 */
	inline const Point_3 &centroid() const
	{
		return this->_primary_face->centroid();
	}
};

}

#endif

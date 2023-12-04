/**
 * \file retqss_vertex.hh
 */

#ifndef _RETQSS_VERTEX_H_
#define _RETQSS_VERTEX_H_

#include "retqss_cgal_main_types.hh"
#include "retqss_object.hh"
#include "retqss_types.hh"


namespace retQSS
{

class Geometry;
class Volume;

/**
 * \brief Representation of a 3D vertex
 *
 * This class implements a vertex of a 3D polyhedron as a retQSS object, which
 * enables the modeler to interact with them through the retQSS model API.
 */
class Vertex : public retQSS::Object
{
	friend class retQSS::Geometry;
	friend class retQSS::Volume;

private:
	Point_3 _point;

public:
	Vertex(Internal_VertexID u, const Point_3 &point) :
		retQSS::Object(u, "Vertex"),
		_point(point)
	{};

	virtual ~Vertex() {};

	/**
	 * \brief Retrieve the coordinates of the vertex
	 */
	inline const Point_3 &point() const
	{
		return this->_point;
	}
};

}

#endif

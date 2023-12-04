/**
 * \file retqss_polyhedron.hh
 */

#ifndef _RETQSS_POLYHEDRON_H_
#define _RETQSS_POLYHEDRON_H_

#include <vector>

#include <CGAL/bounding_box.h>

#include "retqss_cgal_main_types.hh"
#include "retqss_types.hh"


namespace retQSS
{

class PolyhedralVolume;
class PolyhedronFace;

/**
 * \brief Representation of 3D polyhedrons
 *
 * This class represents a 3D polyhedron through CGAL primitives. An instance
 * of this class is a low-level 3D object created by the geometry parsers, not
 * intended to be directly used after. Each polyhedron is used to create a
 * polyhedral volume, which hides away the geometrical aspects of the
 * polyhedron and incorporates additional behavior of adjacent neighboring
 * volumes in the geometry.
 *
 * A polyhedron is created as an "empty" object, into which points and faces
 * are manually added later. After this data is provided, the polyhedron is
 * \em closed, which will trigger several internal method calls to compute
 * physical faces (i.e., instances of retQSS::PolyhedronFace), neighboring
 * faces (i.e., those sharing at least one common vertex) and face
 * triangulation, where each face can be possible decomposed into several
 * triangular subfaces. This is important so that fast intersection and
 * distance queries can be issued to the polyhedron, as geometrical data
 * structures that support them are based upon triangular objects.
 */
class Polyhedron : public CGALPolyhedron
{
	friend class retQSS::PolyhedralVolume;

private:
	retQSS::PolyhedralVolume *vol; //!< Volume represented by this polyhedron

	bool triangulated; //!< Whether the polyhedron is already triangulated

	bool outward_oriented; //!< Whether the polyhedron is outward oriented

	double cap; //!< Precomputed capacity (i.e., volume)

	Point_3 cent; //!< Precomputed centroid of the polyhedron

	std::vector<retQSS::PolyhedronFace*> *_faces; //!< Physical faces
	                                              //!< (borrowed from the
	                                              //!< owning polyhedral volume
	                                              //!< )

	std::vector<retQSS::PolyhedronFace*> subface_parents; //!< To which
                                                          //!< physical face
	                                                      //!< each triangular
	                                                      //!< subface belongs

	std::vector<Point_3> _vertices; //!< Vertices of the polyhedron

	void set_faces();
	void set_face_neighbors();
	void triangulate();
	void close();
	void set_volume(retQSS::PolyhedralVolume*);

public:
	typedef CGALPolyhedron Base;

	Polyhedron();

	virtual ~Polyhedron() {};

	/**
	 * \brief Retrieve associated polyhedral volume
	 */
	inline retQSS::PolyhedralVolume *volume() const
	{
		return this->vol;
	}

	/**
	 * \brief Compute enclosed physical space
	 */
	inline double capacity() const
	{
		return this->cap;
	}

	/**
	 * Compute centroid of the polyhedron
	 */
	inline Point_3 centroid() const
	{
		return this->cent;
	}

	/**
	 * Compute bounding box of the polyhedron
	 */
	inline Iso_cuboid_3 bounding_box() const
	{
		return CGAL::bounding_box(
				this->points().begin(),
				this->points().end());
	}

	/**
	 * \brief Determine whether the polyhedron is outward oriented
	 *
	 * A polyhedron is \em outward \em oriented iff the normals to each of its
	 * faces point outside the domain bounded by the polyhedron.
	 */
	inline bool is_outward_oriented() const
	{
		return this->outward_oriented;
	}

	/**
	 * \brief Retrieve physical face
	 *
	 * Get the physical face associated to the given low-level face ID,
	 * corresponding to one of the triangular subfaces that make up the
	 * physical face.
	 */
	inline retQSS::PolyhedronFace *get_face(
			const retQSS::Polyhedron::Face_index &face_idx) const
	{
		return this->subface_parents[face_idx];
	}

	/**
	 * \brief Get vertices of the polyhedron
	 */
	inline const std::vector<Point_3> &get_vertices()
	{
		if(this->_vertices.size() == 0 && this->num_vertices() > 0)
		{
			BOOST_FOREACH(
					retQSS::Polyhedron::Vertex_index vh,
					this->vertices())
				this->_vertices.push_back(this->point(vh));
		}

		return this->_vertices;
	}
};

}

namespace boost
{

	template<>
	struct graph_traits<retQSS::Polyhedron> :
		public graph_traits<retQSS::Polyhedron::Base>
	{};

	template<class Tag>
	struct property_map<retQSS::Polyhedron, Tag> :
		public property_map<retQSS::Polyhedron::Base, Tag>
	{};

	template<class T>
	struct property_map<retQSS::Polyhedron, CGAL::dynamic_halfedge_property_t<T>> :
		public property_map<retQSS::Polyhedron::Base, CGAL::dynamic_halfedge_property_t<T>>
	{};

	template <class T>
	struct property_map<retQSS::Polyhedron, CGAL::dynamic_edge_property_t<T>> :
		public property_map<retQSS::Polyhedron::Base, CGAL::dynamic_edge_property_t<T>>
	{};

}

namespace CGAL
{

	template<class Tag>
	struct graph_has_property<retQSS::Polyhedron, Tag> :
		public graph_has_property<retQSS::Polyhedron::Base, Tag>
	{};

}

namespace retQSS
{

	template <typename T>
	typename boost::property_map<Polyhedron, CGAL::dynamic_halfedge_property_t<T> >::const_type
	get(const CGAL::dynamic_halfedge_property_t<T> &tag, Polyhedron &p)
	{
		return get(tag, static_cast<Polyhedron::Base&>(p));
	}

	template <typename T>
	typename boost::property_map<Polyhedron, CGAL::dynamic_edge_property_t<T> >::const_type
	get(const CGAL::dynamic_edge_property_t<T> &tag, Polyhedron &p)
	{
		return get(tag, static_cast<Polyhedron::Base&>(p));
	}

}

typedef retQSS::Polyhedron::Face_index FaceIdx;
typedef retQSS::Polyhedron::Vertex_index VertexIndex;

#endif

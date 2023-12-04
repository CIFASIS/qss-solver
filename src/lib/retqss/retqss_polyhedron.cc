#include <math.h>
#include <iostream>

#include <CGAL/enum.h>
#include <CGAL/Kernel/global_functions.h>
#include <CGAL/centroid.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>

#include "retqss_polyhedron.hh"

#include "retqss_interface.hh"
#include "retqss_polyhedral_volume.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_exceptions.hh"


retQSS::Polyhedron::Polyhedron() :
	Base(),
	vol(nullptr),
	triangulated(false),
	outward_oriented(false),
	cap(0),
	cent(Point_3()),
	_faces(nullptr)
{
}

void retQSS::Polyhedron::set_volume(retQSS::PolyhedralVolume *vol)
{
	this->vol = vol;
	this->_faces = &vol->_faces;
}

void retQSS::Polyhedron::close()
{
	if(this->num_vertices() > 0)
	{
		this->outward_oriented =
			CGAL::Polygon_mesh_processing::is_outward_oriented(*this);

		this->set_faces();
		this->set_face_neighbors();
		this->triangulate();

		this->cap = std::abs(CGAL::Polygon_mesh_processing::volume(*this));

		this->cent = CGAL::centroid(
				this->points().begin(),
				this->points().end());
	}
}

void retQSS::Polyhedron::set_faces()
{
	BOOST_FOREACH(
			FaceIdx face_idx,
			this->faces())
	{
		retQSS::PolyhedronFace *face;
		retQSS::Vertex *vertex;
		retQSS::VertexArray vertices;
		retQSS::Polyhedron::Halfedge_index idx = this->halfedge(face_idx);

		while(face_idx >= this->_faces->size())
			this->_faces->push_back(nullptr);

		BOOST_FOREACH(
				retQSS::Polyhedron::Vertex_index vh,
				this->vertices_around_face(idx))
		{
			Point_3 point = this->point(vh);
			vertex = this->vol->point_to_vertex(point);
			vertices.push_back(vertex);
		}

		face = retQSS::PolyhedronFace::from_vertices(this, vertices);

		face->add_subface(face_idx);
		(*this->_faces)[face_idx] = face;
	}
}

void retQSS::Polyhedron::set_face_neighbors()
{
	BOOST_FOREACH(
			FaceIdx face_idx,
			this->faces())
	{
		retQSS::Polyhedron::Halfedge_index idx = this->halfedge(face_idx);
		std::vector<retQSS::PolyhedronFace*> neighbors;

		BOOST_FOREACH(
				FaceIdx neighbor_idx,
				this->faces_around_face(idx))
		{
			if(neighbor_idx == retQSS::Polyhedron::null_face())
				continue;
			retQSS::PolyhedronFace *neighbor = (*this->_faces)[neighbor_idx];
			neighbors.push_back(neighbor);
		}

		(*this->_faces)[face_idx]->set_polyhedron_neighbors(neighbors);
	}
}

void retQSS::Polyhedron::triangulate()
{
	if(this->triangulated) return;

	size_t num_faces = this->num_faces();
	FaceIdx last_face = FaceIdx(num_faces);
	bool result;

	this->subface_parents.resize(num_faces);

	for(size_t i = 0; i < num_faces; i++)
	{
		result = CGAL::Polygon_mesh_processing::triangulate_face(FaceIdx(i), *this);
		if(!result)
			throw retQSS::CGALTriangulationException(this);

		this->subface_parents[i] = (*this->_faces)[i];

		for(; last_face < this->num_faces(); last_face++)
		{
			this->subface_parents.push_back((*this->_faces)[i]);
			(*this->_faces)[i]->add_subface(last_face);
		}
	}

	this->triangulated = true;
}

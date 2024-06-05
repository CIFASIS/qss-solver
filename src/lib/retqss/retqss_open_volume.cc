#include <CGAL/enum.h>
#include <CGAL/Kernel/global_functions.h>

#include "retqss_open_volume.hh"


retQSS::OpenVolume::OpenVolume(
		VolumeID v,
		const std::string &name)
	: retQSS::Volume(v, name),
	  closed(false),
	  locator(nullptr)
{
	this->tree = new retQSS::AABBTreeForMultiplePolyhedrons();
}

retQSS::OpenVolume::~OpenVolume()
{
	delete this->tree;

	if(this->locator != nullptr)
		delete this->locator;
}

void retQSS::OpenVolume::add_face(retQSS::PolyhedronFace *face)
{
	if(this->closed)
		throw retQSS::Exception(
				"Attempting to add new face into a closed open volume.");

	static_cast<retQSS::AABBTreeForMultiplePolyhedrons*>
		(this->tree)->insert(face);

	this->_faces.push_back(face);

	for(auto vertex : face->vertices)
	{
		auto it = this->vertex_to_faces.find(vertex);
		if(it != this->vertex_to_faces.end())
		{
			for(auto neighbor : it->second)
			{
				neighbor->add_boundary_neighbor(face);
				face->add_boundary_neighbor(neighbor);
			}

			this->vertex_to_faces[vertex].push_back(face);
		}
		else
		{
			auto list = std::list<retQSS::PolyhedronFace*>();
			list.push_back(face);
			this->vertex_to_faces[vertex] = list;
		}
	}
}

void retQSS::OpenVolume::add_vertex(retQSS::Vertex *vertex)
{
	if(this->closed)
		throw retQSS::Exception(
				"Attempting to add new vertex into a closed open volume.");

	if(this->vertices_added.find(vertex) == this->vertices_added.end())
	{
		retQSS::Volume::_vertices.push_back(vertex);
		this->vertices_added.insert(vertex);
	}

}

void retQSS::OpenVolume::close()
{
	if(!this->closed)
	{
		this->tree->close();
		this->locator =
				static_cast<retQSS::AABBTreeForMultiplePolyhedrons*>(this->tree)->
				point_locator();
		this->vertex_to_faces.clear();
		this->vertices_added.clear();
		this->closed = true;
	}
}

void retQSS::OpenVolume::ensure_query_structures_are_assembled()
{
	this->close();
}

bool retQSS::OpenVolume::is_exit_direction(
		const retQSS::PolyhedronFace *face,
		const Vector_3 &dir)
{
	retQSS::Lock lock(this->cgal_mutex);

	Vector_3 face_normal = face->normal();
	return CGAL::angle(face_normal, dir) == CGAL::OBTUSE;
}

retQSS::PointLocation retQSS::OpenVolume::locate_point(const Point_3 &point)
{
	retQSS::Lock lock(this->cgal_mutex);

	this->ensure_query_structures_are_assembled();

	CGAL::Bounded_side CGAL_result = (*this->locator)(point);
	retQSS::PointLocation result;

	switch(CGAL_result)
	{
	case CGAL::ON_BOUNDARY:
		result = retQSS::POINT_ON_BOUNDARY;
		break;
	// The unbounded side of the world is its interior!
	case CGAL::ON_UNBOUNDED_SIDE:
		result = retQSS::POINT_INSIDE;
		break;
	case CGAL::ON_BOUNDED_SIDE:
		result = retQSS::POINT_OUTSIDE;
		break;
	}

	return result;
}

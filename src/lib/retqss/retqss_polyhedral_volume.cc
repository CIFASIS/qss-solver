#include <algorithm>
#include <fstream>

#include "retqss_polyhedral_volume.hh"
#include "retqss_interface.hh"
#include "retqss_utilities.hh"


retQSS::PolyhedralVolume::PolyhedralVolume(
		VolumeID v,
		const std::string &name,
		retQSS::Polyhedron *poly,
		const retQSS::VertexArray &vertices)
	:
	retQSS::Volume(v, name),
	poly(poly),
	locator(nullptr),
	on_boundary(false)
{
	retQSS::Volume::_vertices = vertices;
	this->initialize();
}

void retQSS::PolyhedralVolume::initialize()
{
	this->poly->set_volume(this);
	this->poly->close();
	this->tree = new retQSS::AABBTreeForSinglePolyhedron(this->poly);
}

retQSS::PolyhedralVolume::~PolyhedralVolume()
{
	delete this->poly;
	delete this->tree;

	if(this->locator != nullptr)
		delete this->locator;

	for(auto face: this->_faces)
		delete face;
}

void retQSS::PolyhedralVolume::ensure_query_structures_are_assembled()
{
	if(this->locator == nullptr)
	{
		this->tree->close();
		this->locator =
				static_cast<retQSS::AABBTreeForSinglePolyhedron*>(this->tree)->
				point_locator();
	}
}

retQSS::Vertex *retQSS::PolyhedralVolume::point_to_vertex(
		const Point_3 &point) const
{
	retQSS::Vertex *vertex = nullptr;

	for(auto vol_vertex: this->_vertices)
	{
		if(vol_vertex->point() == point)
			vertex = vol_vertex;
	}

	return vertex;
}

retQSS::PointLocation retQSS::PolyhedralVolume::locate_point(const Point_3 &point)
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
	case CGAL::ON_BOUNDED_SIDE:
		result = retQSS::POINT_INSIDE;
		break;
	case CGAL::ON_UNBOUNDED_SIDE:
		result = retQSS::POINT_OUTSIDE;
		break;
	}

	return result;
}

bool retQSS::PolyhedralVolume::is_polyhedral() const
{
	return true;
}

bool retQSS::PolyhedralVolume::is_exit_direction(
		const retQSS::PolyhedronFace *face,
		const Vector_3 &dir)
{
	retQSS::Lock lock(this->cgal_mutex);

	Vector_3 face_normal = face->normal();
	return CGAL::angle(face_normal, dir) == CGAL::ACUTE;
}

Point_3 retQSS::PolyhedralVolume::random_point()
{
	double th0 = 1e-5, factor = 1.5;

	int iterations = retQSS::random_int(1,10);

	int index = retQSS::random_int(0, this->_faces.size()-1);
	auto face = this->_faces[index];
	auto xn = face->centroid();
	auto th = th0;

	while(true)
	{
		auto p = xn - face->normal()*th;
		if(this->locate_point(p) == retQSS::POINT_INSIDE)
		{
			xn = p;
			break;
		}
		else
			th *= 0.1;
	}

	for(int i = 0; i < iterations; i++)
	{
		double max_factor = th0;
		double dx = retQSS::random_double(-1,1);
		double dy = retQSS::random_double(-1,1);
		double dz = retQSS::random_double(-1,1);
		Vector_3 d(dx,dy,dz);

		while(true)
		{
			auto p = xn + d*max_factor;
			if(this->locate_point(p) == retQSS::POINT_INSIDE)
				break;
			else
				max_factor /= 10;
		}

		while(true)
		{
			auto p = xn + d*(max_factor*factor);
			if(this->locate_point(p) == retQSS::POINT_INSIDE)
				max_factor *= factor;
			else
			{
				xn = xn + d*max_factor;
				break;
			}
		}
	}

	return xn;
}

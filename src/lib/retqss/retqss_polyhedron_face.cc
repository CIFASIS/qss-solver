#include <cmath>

#include <CGAL/centroid.h>

#include "retqss_polyhedron_face.hh"
#include "retqss_logical_face.hh"
#include "retqss_polyhedral_volume.hh"
#include "retqss_utilities.hh"
#include "retqss_exceptions.hh"

#define _FACE_VERTEX(i) (this->vertices[i]->point())


retQSS::PolyhedronFace::PolyhedronFace(
		retQSS::Polyhedron *poly,
		retQSS::VertexArray &vertices) :
		poly(poly), vertices(vertices), _logical_face(nullptr)
{
}

void retQSS::PolyhedronFace::set_polyhedron_neighbors(
		const std::vector<retQSS::PolyhedronFace*> &neighbors)
{
	this->poly_neighbors = neighbors;
}

void retQSS::PolyhedronFace::precompute_common_data(
		const Vector_3 &v0,
		const Vector_3 &v1)
{
	Vector_3 normal = CGAL::cross_product(v0, v1);

	this->_ox = _FACE_VERTEX(0).x();
	this->_oy = _FACE_VERTEX(0).y();
	this->_oz = _FACE_VERTEX(0).z();

	double norm = CGAL::sqrt(normal.squared_length());
	this->_nx = normal.x() / norm;
	this->_ny = normal.y() / norm;
	this->_nz = normal.z() / norm;

	// TODO: double-check. We might need to orient the polyhedron
	// manually when parsing the geometry. Some faces may be oriented
	// outwards while others can be inwards, which can lead to wrong
	// results when testing points.
	if(!this->poly->is_outward_oriented())
	{
		this->_nx *= -1;
		this->_ny *= -1;
		this->_nz *= -1;
	}

	this->_sum_n_sq = this->_nx*this->_nx + this->_ny*this->_ny + this->_nz*this->_nz;
	this->_sub_n_o = -(this->_ox*this->_nx + this->_oy*this->_ny + this->_oz*this->_nz);

	std::vector<Point_3> points;
	for(auto vertex: this->vertices)
		points.push_back(vertex->point());
	this->_centroid = CGAL::centroid(points.begin(), points.end());
}

bool retQSS::PolyhedronFace::operator==(const retQSS::PolyhedronFace &face) const
{
	if(this->vertices.size() != face.vertices.size())
		return false;

	for(auto point_it1 = this->vertices.begin();
			point_it1 != this->vertices.end();
			point_it1++)
	{
		bool ok = false;

		for(auto point_it2 = face.vertices.begin();
				point_it2 != face.vertices.end();
				point_it2++)
			if((*point_it1)->point() == (*point_it2)->point())
			{
				ok = true;
				break;
			}

		if(!ok)
			return false;
	}

	return true;
}

bool retQSS::PolyhedronFace::is_point_on_boundary(double x, double y, double z) const
{
	Point_3 p(x, y, z);
	Vector_3 v1, v2, c;
	double d1, d2;

	for(size_t i = 0; i < this->vertices.size(); ++i)
	{
		v1 = Vector_3(
				_FACE_VERTEX(i),
				_FACE_VERTEX(i < this->vertices.size()-1 ? i+1 : 0));
		v2 = Vector_3(_FACE_VERTEX(i), p);

		c = CGAL::cross_product(v1, v2);
		if(
			std::abs(c[0]) > _INSIDE_TOLERANCE ||
			std::abs(c[1]) > _INSIDE_TOLERANCE ||
			std::abs(c[2]) > _INSIDE_TOLERANCE)
			continue;

		d1 = v1 * v1;
		d2 = v1 * v2;

		if(
			d2 < _INSIDE_TOLERANCE ||
			d2 > d1 + _INSIDE_TOLERANCE)
			continue;

		return true;
	}

	return false;
}

bool retQSS::PolyhedronFace::is_on_boundary() const
{
	return this->_logical_face->is_on_boundary();
}

void retQSS::PolyhedronFace::add_subface(FaceIdx subface_idx)
{
	this->_subfaces.push_back(subface_idx);
}

retQSS::TriangularFace::TriangularFace(
		retQSS::Polyhedron *poly,
		retQSS::VertexArray &vertices) :
			retQSS::PolyhedronFace(poly, vertices)
{
}

retQSS::QuadrilateralFace::QuadrilateralFace(
		retQSS::Polyhedron *poly,
		retQSS::VertexArray &vertices) :
			retQSS::PolyhedronFace(poly, vertices)
{
}

retQSS::RectangularFace::RectangularFace(
		retQSS::Polyhedron *poly,
		retQSS::VertexArray &vertices) :
			retQSS::QuadrilateralFace(poly, vertices)
{
}

void retQSS::TriangularFace::precompute_data()
{
	Vector_3 v01 = Vector_3(_FACE_VERTEX(0), _FACE_VERTEX(1));
	Vector_3 v02 = Vector_3(_FACE_VERTEX(0), _FACE_VERTEX(2));

	this->_v01x = v01.x();
	this->_v01y = v01.y();
	this->_v01z = v01.z();

	this->_v02x = v02.x();
	this->_v02y = v02.y();
	this->_v02z = v02.z();

	this->_dot0101 = v01 * v01;
	this->_dot0102 = v01 * v02;
	this->_dot0202 = v02 * v02;
	this->_inv_denom = 1. / (this->_dot0101 * this->_dot0202 - this->_dot0102 * this->_dot0102);

	this->precompute_common_data(v01, v02);
}

void retQSS::QuadrilateralFace::precompute_data()
{
	Vector_3 v01 = Vector_3(_FACE_VERTEX(0), _FACE_VERTEX(1));
	Vector_3 v02 = Vector_3(_FACE_VERTEX(0), _FACE_VERTEX(2));
	Vector_3 v03 = Vector_3(_FACE_VERTEX(0), _FACE_VERTEX(3));

	this->_v01x = v01.x();
	this->_v01y = v01.y();
	this->_v01z = v01.z();

	this->_v02x = v02.x();
	this->_v02y = v02.y();
	this->_v02z = v02.z();

	this->_v03x = v03.x();
	this->_v03y = v03.y();
	this->_v03z = v03.z();

	this->_dot0101 = v01 * v01;
	this->_dot0102 = v01 * v02;
	this->_dot0202 = v02 * v02;
	this->_dot0203 = v02 * v03;
	this->_dot0303 = v03 * v03;
	this->_inv_denom1 = 1. / (this->_dot0101 * this->_dot0202 - this->_dot0102 * this->_dot0102);
	this->_inv_denom2 = 1. / (this->_dot0202 * this->_dot0303 - this->_dot0203 * this->_dot0203);

	this->precompute_common_data(v01, v03);
}

void retQSS::RectangularFace::precompute_data()
{
	Vector_3 v01 = Vector_3(_FACE_VERTEX(0), _FACE_VERTEX(1));
	Vector_3 v03 = Vector_3(_FACE_VERTEX(0), _FACE_VERTEX(3));

	this->_v01x = v01.x();
	this->_v01y = v01.y();
	this->_v01z = v01.z();

	this->_v03x = v03.x();
	this->_v03y = v03.y();
	this->_v03z = v03.z();

	this->_dot0101 = v01 * v01;
	this->_dot0303 = v03 * v03;

	this->precompute_common_data(v01, v03);
}

retQSS::VTKCellType retQSS::TriangularFace::vtk_cell_type() const
{
	return retQSS::VTK_TRIANGLE;
}

retQSS::VTKCellType retQSS::QuadrilateralFace::vtk_cell_type() const
{
	return retQSS::VTK_QUAD;
}

retQSS::VTKCellType retQSS::RectangularFace::vtk_cell_type() const
{
	return retQSS::VTK_PIXEL;
}

bool retQSS::TriangularFace::is_point_inside(
		double x,
		double y,
		double z) const
{
	double v2x = x - this->_ox;
	double v2y = y - this->_oy;
	double v2z = z - this->_oz;

	double dot_norm = this->_nx * v2x + this->_ny * v2y + this->_nz * v2z;

	// If point is not inside the plane containing the triangle, return
	// immediately.
	if(dot_norm < -_INSIDE_TOLERANCE || dot_norm > _INSIDE_TOLERANCE)
		return false;

	double dot02 = this->_v02x*v2x + this->_v02y*v2y + this->_v02z*v2z;
	double dot12 = this->_v01x*v2x + this->_v01y*v2y + this->_v01z*v2z;

	double u = (this->_dot0101 * dot02 - this->_dot0102 * dot12) * this->_inv_denom;

	if(u < -_INSIDE_TOLERANCE)
		return false;

	double v = (this->_dot0202 * dot12 - this->_dot0102 * dot02) * this->_inv_denom;

	return
			v >= -_INSIDE_TOLERANCE &&
			u+v < 1 + _INSIDE_TOLERANCE;
}

bool retQSS::QuadrilateralFace::is_point_inside(
		double x,
		double y,
		double z) const
{
	double v2x = x - this->_ox;
	double v2y = y - this->_oy;
	double v2z = z - this->_oz;

	double dot_norm = this->_nx * v2x + this->_ny * v2y + this->_nz * v2z;

	// If point is not inside the plane containing the quad, return
	// immediately.
	if(dot_norm < -_INSIDE_TOLERANCE || dot_norm > _INSIDE_TOLERANCE)
		return false;

	// Check first triangle.
	double dot1 = this->_v02x*v2x + this->_v02y*v2y + this->_v02z*v2z;
	double dot2 = this->_v01x*v2x + this->_v01y*v2y + this->_v01z*v2z;

	double u = (this->_dot0101 * dot1 - this->_dot0102 * dot2) * this->_inv_denom1;

	if(u < -_INSIDE_TOLERANCE)
		return false;

	double v = (this->_dot0202 * dot2 - this->_dot0102 * dot1) * this->_inv_denom1;

	if(v >= -_INSIDE_TOLERANCE && u+v < 1 + _INSIDE_TOLERANCE)
		return true;

	// Check second triangle.
	dot1 = this->_v03x*v2x + this->_v03y*v2y + this->_v03z*v2z;
	dot2 = this->_v02x*v2x + this->_v02y*v2y + this->_v02z*v2z;

	u = (this->_dot0202 * dot1 - this->_dot0203 * dot2) * this->_inv_denom2;

	if(u < -_INSIDE_TOLERANCE)
		return false;

	v = (this->_dot0303 * dot2 - this->_dot0203 * dot1) * this->_inv_denom2;

	return v >= -_INSIDE_TOLERANCE && u+v < 1 + _INSIDE_TOLERANCE;
}

bool retQSS::RectangularFace::is_point_inside(
		double x,
		double y,
		double z) const
{
	double v2x = x - this->_ox;
	double v2y = y - this->_oy;
	double v2z = z - this->_oz;

	double dot_norm = this->_nx * v2x + this->_ny * v2y + this->_nz * v2z;

	// If point is not inside the plane containing the rectangle, return
	// immediately.
	if(dot_norm < -_INSIDE_TOLERANCE || dot_norm > _INSIDE_TOLERANCE)
		return false;

	double dot02 = this->_v03x*v2x + this->_v03y*v2y + this->_v03z*v2z;

	if(dot02 < -_INSIDE_TOLERANCE || dot02 > this->_dot0303+_INSIDE_TOLERANCE)
		return false;

	double dot12 = this->_v01x*v2x + this->_v01y*v2y + this->_v01z*v2z;

	return dot12 >= -_INSIDE_TOLERANCE && dot12 <= this->_dot0101+_INSIDE_TOLERANCE;
}

bool retQSS::TriangularFace::intersected_by(const Ray_3& ray)
{
	retQSS::Lock lock(this->cgal_mutex);

	Triangle_3 triangle(
			_FACE_VERTEX(0),
			_FACE_VERTEX(1),
			_FACE_VERTEX(2));

	return CGAL::do_intersect(ray, triangle);
}

bool retQSS::QuadrilateralFace::intersected_by(const Ray_3& ray)
{
	retQSS::Lock lock(this->cgal_mutex);

	// TODO: do this properly!

	Triangle_3 t1(
			_FACE_VERTEX(0),
			_FACE_VERTEX(1),
			_FACE_VERTEX(2));
	Triangle_3 t2(
			_FACE_VERTEX(2),
			_FACE_VERTEX(3),
			_FACE_VERTEX(0));

	return
			CGAL::do_intersect(ray, t1) ||
			CGAL::do_intersect(ray, t2);
}

retQSS::PolyhedronFace *retQSS::PolyhedronFace::from_vertices(
		retQSS::Polyhedron *poly,
		retQSS::VertexArray &vertices)
{
	retQSS::PolyhedronFace *face = nullptr;

	if(vertices.size() == 3)
		face = new retQSS::TriangularFace(poly, vertices);
	else if(vertices.size() == 4)
	{
		Vector_3 v1(vertices[0]->point(), vertices[1]->point());
		Vector_3 v2(vertices[0]->point(), vertices[3]->point());
		Vector_3 v3(vertices[2]->point(), vertices[1]->point());
		Vector_3 v4(vertices[2]->point(), vertices[3]->point());

		double a1 = retQSS::angle_between(v1, v2);
		double a2 = retQSS::angle_between(v3, v4);

		bool is_right1 = retQSS::numbers_are_equal(a1, M_PI/2.);
		bool is_right2 = retQSS::numbers_are_equal(a2, M_PI/2.);

		if(is_right1 && is_right2)
			face = new retQSS::RectangularFace(poly, vertices);
		else
			face = new retQSS::QuadrilateralFace(poly, vertices);
	}
	else
		throw retQSS::UnsupportedPolygonalFaceException(vertices);

	face->precompute_data();

	return face;
}

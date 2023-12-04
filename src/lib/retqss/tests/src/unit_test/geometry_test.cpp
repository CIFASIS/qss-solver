#define private public
#define protected public

#include <string>
#include <math.h>

#include "qss/qss_simulator.h"

#include "retqss/retqss_cgal_main_types.hh"
#include "retqss/retqss_geometry.hh"
#include "retqss/retqss_model_api.h"
#include "retqss/retqss_polyhedron_face.hh"
#include "retqss/retqss_intersection_locator.hh"
#include "retqss/retqss_volume.hh"
#include "retqss/retqss_polyhedron.hh"
#include "retqss/retqss_polyhedral_volume.hh"
#include "retqss/retqss_trajectory.hh"
#include "retqss/retqss_types.hh"
#include "retqss/retqss_vertex.hh"
#include "retqss/retqss_interface.hh"

#include "test_common/qss_test_utils.h"

#include "gtest/gtest.h"

using namespace std;


class retQSSGeometryTest : public testing::Test
{
private:

	void TearDown()
	{
		retQSS::Interface::clear_instance();
	}

public:
	QSS_simulator sim;
};


TEST_F(retQSSGeometryTest, point_in_triangle_test)
{
    Point_3 p(-2.0750999999999991, -81.988299999999995, -27.314);
    Point_3 q(-27.314, -2.0750999999999991, -81.988299999999995);
    Point_3 r(47.314, 22.075099999999999, 101.9883);

    retQSS::Vertex v1(1, p);
    retQSS::Vertex v2(2, q);
    retQSS::Vertex v3(3, r);

    auto poly = new retQSS::Polyhedron;
    retQSS::VertexArray vertices({&v1, &v2, &v3});
    auto face = retQSS::PolyhedronFace::from_vertices(poly, vertices);

    // This point is on the edge of the triangle.
    EXPECT_TRUE(face->is_point_inside(10, 10, 10));
    // This point is inside the triangle.
    EXPECT_TRUE(face->is_point_inside(5.115525, -13.60083, -3.927915));
    // This point is not coplanar with the triangle.
    EXPECT_FALSE(face->is_point_inside(1, 3, 10));
    // This point is completely outside the triangle (i.e. not even its
    // projection lies inside it).
    EXPECT_FALSE(face->is_point_inside(1, 52, 23));
}

TEST_F(retQSSGeometryTest, point_in_segment_test)
{
    Point_3 p(-2.0750999999999991, -81.988299999999995, -27.314);
    Point_3 q(-27.314, -2.0750999999999991, -81.988299999999995);
    Point_3 r(47.314, 22.075099999999999, 101.9883);

    retQSS::Vertex v1(1, p);
    retQSS::Vertex v2(2, q);
    retQSS::Vertex v3(3, r);

    auto poly = new retQSS::Polyhedron;
    retQSS::VertexArray vertices({&v1, &v2, &v3});
    auto face = retQSS::PolyhedronFace::from_vertices(poly, vertices);

    // Segment PQ test.
    Point_3 point1 = p + 0.3 * Vector_3(p, q);
    Point_3 point2 = p;
    Point_3 point3 = q;
    Point_3 point4 = p + 1.1 * Vector_3(p, q);
    Point_3 point5 = p - 0.2 * Vector_3(p, q);

    EXPECT_TRUE(face->is_point_on_boundary(point1));
    EXPECT_TRUE(face->is_point_on_boundary(point2));
    EXPECT_TRUE(face->is_point_on_boundary(point3));
    EXPECT_FALSE(face->is_point_on_boundary(point4));
    EXPECT_FALSE(face->is_point_on_boundary(point5));

    // Segment RP test.
    point1 = p + 0.83 * Vector_3(p, r);
    point2 = p;
    point3 = r;
    point4 = p + 1.1 * Vector_3(p, r);
    point5 = p - 0.2 * Vector_3(p, r);

    EXPECT_TRUE(face->is_point_on_boundary(point1));
    EXPECT_TRUE(face->is_point_on_boundary(point2));
    EXPECT_TRUE(face->is_point_on_boundary(point3));
    EXPECT_FALSE(face->is_point_on_boundary(point4));
    EXPECT_FALSE(face->is_point_on_boundary(point5));
}

TEST_F(retQSSGeometryTest, point_in_quadrilateral_test)
{
    Point_3 p1(-40.8418, -87.4855, 86.792);
    Point_3 p2(-54.6867, -87.0762, 32.8244);
    Point_3 p3(-23.584, -85.0357, 3.1907);
    Point_3 p4(-11.7168, -85.5296, 60.5564);

    retQSS::Vertex v1(1, p1);
    retQSS::Vertex v2(2, p2);
    retQSS::Vertex v3(3, p3);
    retQSS::Vertex v4(4, p4);

    Point_3 in1(-36.401289999999996, -87.05339, 76.14812);
    Point_3 in2(-30.00221, -85.55241000000001, 17.23267999999999);
    Point_3 out(-19.736700000000006, -84.72912, 1.341679999999993);

    auto poly = new retQSS::Polyhedron;
    retQSS::VertexArray vertices({&v1, &v2, &v3, &v4});
    auto face = retQSS::PolyhedronFace::from_vertices(poly, vertices);

    EXPECT_TRUE(face->is_point_inside(in1));
    EXPECT_TRUE(face->is_point_inside(in2));
    EXPECT_FALSE(face->is_point_inside(out));
    EXPECT_FALSE(face->is_point_inside(11, 512, 123));
}

TEST_F(retQSSGeometryTest, intersection_test)
{
    Point_3 p(5, -5 ,5);
    Point_3 q(5, 5, 5);
    Point_3 r(-5, 5, 5);

    retQSS::Vertex v1(1, p);
    retQSS::Vertex v2(2, q);
    retQSS::Vertex v3(3, r);

    auto poly = new retQSS::Polyhedron;
    retQSS::VertexArray vertices({&v1, &v2, &v3});
    auto face = retQSS::PolyhedronFace::from_vertices(poly, vertices);

    double x[3] = {0, 10.44, -3};
    double y[3] = {0, -10, 12.33};
    double z[3] = {0, -2.3233, 27.3};
    double polys[9] = {
    		x[0], x[1], x[2],
    		y[0], y[1], y[2],
    		z[0], z[1], z[2],
    };

    int order = 2;

    double tx = 0;
    double ty = 0;
    double tz = 0;
    double t0 = 0;

    auto trajectory = retQSS::Trajectory::for_order(
    		order,
			polys, polys+3, polys+6,
			&tx, &ty, &tz,
			&t0);
    trajectory->update();

    Point_3 expected(
    		4.2640587706144997,
			-1.9720487815397412,
			5);

    auto locator = retQSS::IntersectionLocator::for_order(order);
    auto data = locator->intersection_of(*trajectory, *face);
    EXPECT_TRUE(data.is_valid());

    Point_3 intersection = data.point();
    EXPECT_TRUE(retQSS::points_are_equal(intersection, expected));
}

TEST_F(retQSSGeometryTest, random_point_generation)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458},
		{0, 2, 0, -0.842883, 269.5433, 0},
		{-269.5433, 0, 130.5458, 269.5433, 0, 130.5458},
		{-269.5433, 0, -130.5458, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 4);

	auto geometry = RETQSS()->geometry();
	auto vol1 = geometry->get_polyhedral_volume(1);

	for(int i = 0; i < 30; i++)
	{
		auto point = vol1->random_point();
		EXPECT_NE(vol1->locate_point(point), retQSS::POINT_OUTSIDE);
	}
}

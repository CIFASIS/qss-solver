#define private public
#define protected public

#include <string>
#include <math.h>
#include <stdio.h>

#include "qss/qss_simulator.h"

#include "retqss/retqss_cgal_main_types.hh"
#include "retqss/retqss_geometry.hh"
#include "retqss/retqss_model_api.h"
#include "retqss/retqss_polyhedron_face.hh"
#include "retqss/retqss_intersection_locator.hh"
#include "retqss/retqss_volume.hh"
#include "retqss/retqss_polyhedral_volume.hh"
#include "retqss/retqss_trajectory.hh"
#include "retqss/retqss_types.hh"
#include "retqss/retqss_polyhedron.hh"
#include "retqss/retqss_logical_face.hh"
#include "retqss/retqss_face_crossing_algorithm.hh"
#include "retqss/retqss_interface.hh"

#include "test_common/qss_test_utils.h"

#include "gtest/gtest.h"

using namespace std;


class retQSSInterfaceTest : public testing::Test
{
private:

	void SetUp()
	{
		this->sim = nullptr;
	}

	void TearDown()
	{
		retQSSTest::free_simulator(this->sim);
	}

public:
	QSS_simulator sim;
};

// TODO: add one test for each function in the API!

TEST_F(retQSSInterfaceTest, particle_position_and_velocity)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458},
		{0, 2, 0, -0.842883, 269.5433, 0}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 2);

	double x,y,z,vx,vy,vz;

	retQSS_particle_currentPosition(1, &x, &y, &z);
	retQSS_particle_currentVelocity(1, &vx, &vy, &vz);

	EXPECT_EQ(x ,0);
	EXPECT_EQ(y ,0);
	EXPECT_EQ(z ,0);
	EXPECT_TRUE(retQSS::numbers_are_equal(vx, 269.5433));
	EXPECT_TRUE(retQSS::numbers_are_equal(vy, 0));
	EXPECT_TRUE(retQSS::numbers_are_equal(vz, 130.5458));

	retQSS_particle_currentPosition(2, &x, &y, &z);
	retQSS_particle_currentVelocity(2, &vx, &vy, &vz);

	EXPECT_EQ(x ,0);
	EXPECT_EQ(y ,2.);
	EXPECT_EQ(z ,0);
	EXPECT_TRUE(retQSS::numbers_are_equal(vx, -0.842883));
	EXPECT_TRUE(retQSS::numbers_are_equal(vy, 269.5433));
	EXPECT_TRUE(retQSS::numbers_are_equal(vz, 0));
}

TEST_F(retQSSInterfaceTest, public_to_internal_ID_conversion)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto retqss = RETQSS();
	auto tag = "test_prop";

	auto val = retQSS_vertex_setProperty(1, tag, 5);
	EXPECT_TRUE(retQSS_vertex_hasProperty(1, tag));
	EXPECT_TRUE(retqss->vertex_has_property(0, tag));

	val = retQSS_vertex_getProperty(1, tag);
	EXPECT_EQ(val, 5);
	val = retqss->vertex_get_property(0, tag);
	EXPECT_EQ(val, 5);

	val = retQSS_particle_setProperty(1, tag, 10);
	EXPECT_TRUE(retQSS_particle_hasProperty(1, tag));
	EXPECT_TRUE(retqss->particle_has_property(0, tag));

	val = retQSS_particle_getProperty(1, tag);
	EXPECT_EQ(val, 10);
	val = retqss->particle_get_property(0, tag);
	EXPECT_EQ(val, 10);
}

TEST_F(retQSSInterfaceTest, distance_between_two_particles)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458},
		{0.1, 0.3, 0.5, -0.842883, 269.5433, 0},
		{10, 10, 10, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 3);

	auto distance = retQSS_particle_squaredDistanceBetween(1,2);

	EXPECT_TRUE(retQSS::numbers_are_equal(distance, 0.35));
}

TEST_F(retQSSInterfaceTest, angle_between_two_particles)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458},
		{0, 2, 0, -0.842883, 269.5433, 0},
		{-269.5433, 0, 130.5458, 269.5433, 0, 130.5458},
		{-269.5433, 0, -130.5458, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 4);

	auto angle1 = retQSS_particle_angleBetween(1,2);
	auto angle2 = retQSS_particle_angleBetween(1,3);
	auto angle3 = retQSS_particle_angleBetween(1,4);

	EXPECT_TRUE(retQSS::numbers_are_equal(angle1, M_PI/2));
	EXPECT_TRUE(retQSS::numbers_are_equal(angle2, 2.2395389));
	EXPECT_TRUE(retQSS::numbers_are_equal(angle3, M_PI));
}

TEST_F(retQSSInterfaceTest, dump_and_read_initial_conditions)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458},
		{0, 2, 0, -0.842883, 269.5433, 0},
		{-269.5433, 0, 130.5458, 269.5433, 0, 130.5458},
		{-269.5433, 0, -130.5458, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	double x,y,z,vx,vy,vz;
	auto filename = "test_ic.txt";

	auto status = retQSS_particle_dumpAllInitialConditions(filename);
	auto ic_array = retQSS::read_initial_conditions(filename, ic.size());

	EXPECT_NE(ic_array, nullptr);
	EXPECT_EQ(ic_array->size(), ic.size());

	for(size_t i = 0; i < ic.size(); i++)
	{
		auto first = ic[i];
		auto second = ic_array->at(i);

		EXPECT_TRUE(retQSS::numbers_are_equal(first.x, second.x));
		EXPECT_TRUE(retQSS::numbers_are_equal(first.y, second.y));
		EXPECT_TRUE(retQSS::numbers_are_equal(first.z, second.z));
		EXPECT_TRUE(retQSS::numbers_are_equal(first.vx, second.vx));
		EXPECT_TRUE(retQSS::numbers_are_equal(first.vy, second.vy));
		EXPECT_TRUE(retQSS::numbers_are_equal(first.vz, second.vz));
	}

	remove(filename);
}

TEST_F(retQSSInterfaceTest, random_points_and_random_volumes)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458},
		{0, 2, 0, -0.842883, 269.5433, 0},
		{-269.5433, 0, 130.5458, 269.5433, 0, 130.5458},
		{-269.5433, 0, -130.5458, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 4);

	auto geometry = RETQSS()->geometry();
	int id = retQSS_geometry_randomVolumeID();
	auto vol = geometry->get_polyhedral_volume(id);
	double x, y, z;

	retQSS_volume_randomPoint(id, &x, &y, &z);
	auto point1 = Point_3(x,y,z);

	EXPECT_NE(vol->locate_point(point1), retQSS::POINT_OUTSIDE);

	retQSS_volume_randomPoint(id, &x, &y, &z);
	auto point2 = Point_3(x,y,z);

	EXPECT_NE(point1, point2);
	EXPECT_NE(vol->locate_point(point2), retQSS::POINT_OUTSIDE);
}

TEST_F(retQSSInterfaceTest, particle_bounce)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto tracker = RETQSS()->tracker();
	auto geometry = RETQSS()->geometry();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 10, x[2] = 0.01, x[3] = 0;
	x[5] = 2, x[6] = 0.01, x[7] = 0;
	x[9] = 3, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	tracker->set_exit_state(0, data);
	tracker->update_state(0);

	double vx, vy, vz;

	retQSS_particle_reflectiveBounce(1, &vx, &vy, &vz);

	EXPECT_EQ(vx, 10);
	EXPECT_EQ(vy, 2);
	EXPECT_EQ(vz, -3);
}

TEST_F(retQSSInterfaceTest, particle_bounce_with_speed)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto tracker = RETQSS()->tracker();
	auto geometry = RETQSS()->geometry();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 10, x[2] = 0.01, x[3] = 0;
	x[5] = 2, x[6] = 0.01, x[7] = 0;
	x[9] = 3, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);

	tracker->set_exit_state(0, data);
	tracker->update_state(0);

	double vx, vy, vz;

	retQSS_particle_reflectiveBounceWithSpeed(1, 20, &vx, &vy, &vz);
	double speed_sq = vx*vx + vy*vy + vz*vz;

	EXPECT_TRUE(retQSS::numbers_are_equal(speed_sq, 20*20));
	EXPECT_TRUE(retQSS::numbers_are_equal(vx, 18.81441737));
	EXPECT_TRUE(retQSS::numbers_are_equal(vy, 3.762883474));
	EXPECT_TRUE(retQSS::numbers_are_equal(vz, -5.64432521));
}

TEST_F(retQSSInterfaceTest, volume_vertices_and_faces)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();

	for(VolumeID v = 1; v <= geometry->num_polyhedral_volumes(); v++)
	{
		auto n_vertices = retQSS_volume_countVertices(v);
		auto n_faces = retQSS_volume_countFaces(v);

		EXPECT_EQ(n_vertices, 8);
		EXPECT_EQ(n_faces, 6);
	}
}

TEST_F(retQSSInterfaceTest, face_primary_volume)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();

	// No possible ambiguity for primary volumes of boundary faces.
	for(auto vol : geometry->volumes())
	{
		for(auto face : vol->faces())
		{
			if(face->is_on_boundary())
			{
				auto face_id = face->logical_face()->get_ID();
				auto public_face_id = RETQSS_TO_PUBLIC(face_id);
				auto vol_ID = retQSS_face_primaryVolumeID(public_face_id);

				EXPECT_EQ(vol_ID, vol->get_ID());
			}
		}
	}
}

TEST_F(retQSSInterfaceTest, face_centroid)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();

	retQSS::PolyhedronFace *face = nullptr;
	auto vol = geometry->get_polyhedral_volume(29);
	for(auto f : vol->faces())
		if(f->is_on_boundary())
		{
			face = f;
			break;
		}

	EXPECT_NE(face, nullptr);

	auto face_id = face->logical_face()->get_ID();
	auto public_face_id = RETQSS_TO_PUBLIC(face_id);
	double x, y, z;

	retQSS_face_centroid(public_face_id, &x, &y, &z);

	EXPECT_TRUE(retQSS::numbers_are_equal(x, -40));
	EXPECT_TRUE(retQSS::numbers_are_equal(y, -30));
	EXPECT_TRUE(retQSS::numbers_are_equal(z, 90));
}

TEST_F(retQSSInterfaceTest, face_normals)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();

	retQSS::PolyhedronFace *face = nullptr;
	auto vol = geometry->get_polyhedral_volume(29);
	for(auto f : vol->faces())
		if(f->is_on_boundary())
		{
			face = f;
			break;
		}

	EXPECT_NE(face, nullptr);

	auto face_id = face->logical_face()->get_ID();
	auto public_face_id = RETQSS_TO_PUBLIC(face_id);

	auto normal = face->normal();
	double nx = normal.x();
	double ny = normal.y();
	double nz = normal.z();

	auto is_normal = retQSS_face_isNormalTo(public_face_id, nx, ny, nz);
	EXPECT_TRUE(is_normal);

	face = nullptr;
	vol = geometry->get_polyhedral_volume(35);
	for(auto f : vol->faces())
		if(f->is_on_boundary())
		{
			face = f;
			break;
		}

	EXPECT_NE(face, nullptr);

	auto coplanar_face_id = face->logical_face()->get_ID();
	auto public_coplanar_face_id = RETQSS_TO_PUBLIC(coplanar_face_id);

	retQSS_face_normal(public_coplanar_face_id, &nx, &ny, &nz);

	is_normal = retQSS_face_isNormalTo(public_face_id, nx, ny, nz);
	EXPECT_TRUE(is_normal);

	face = nullptr;
	vol = geometry->get_polyhedral_volume(84);
	for(auto f : vol->faces())
		if(f->is_on_boundary())
		{
			face = f;
			break;
		}

	EXPECT_NE(face, nullptr);

	auto non_coplanar_face_id = face->logical_face()->get_ID();
	auto public_non_coplanar_face_id = RETQSS_TO_PUBLIC(non_coplanar_face_id);

	retQSS_face_normal(public_non_coplanar_face_id, &nx, &ny, &nz);

	is_normal = retQSS_face_isNormalTo(public_face_id, nx, ny, nz);
	EXPECT_FALSE(is_normal);
}

TEST_F(retQSSInterfaceTest, adjacent_volumes)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();
	auto v = 79;

	auto vol = geometry->get_polyhedral_volume(v);
	auto pos = vol->centroid();
	auto dir = Vector_3(0.01, 0.01, 1);
	auto v1 = retQSS_volume_adjacentVolumeID(
				v,
				pos.x(), pos.y(), pos.z(),
				dir.x(), dir.y(), dir.z());

	EXPECT_EQ(v1, v+1);

	dir = Vector_3(0.01, 1, 0.01);
	v1 = retQSS_volume_adjacentVolumeID(
				v,
				pos.x(), pos.y(), pos.z(),
				dir.x(), dir.y(), dir.z());

	EXPECT_EQ(v1, v+12);

	v = 30;
	vol = geometry->get_polyhedral_volume(v);
	pos = vol->centroid();
	dir = Vector_3(1, -0.01, 0.01);
	v1 = retQSS_volume_adjacentVolumeID(
				0,
				pos.x()-30, pos.y(), pos.z(),
				dir.x(), dir.y(), dir.z());

	EXPECT_EQ(v1, v);
}

TEST_F(retQSSInterfaceTest, face_opposite_objects)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();

	retQSS::PolyhedronFace *face = nullptr;
	auto vol = geometry->get_polyhedral_volume(85);
	for(auto f : vol->faces())
		if(f->is_on_boundary())
		{
			face = f;
			break;
		}

	EXPECT_NE(face, nullptr);

	double nx, ny, nz;
	auto face_id = face->logical_face()->get_ID();
	auto public_face_id = RETQSS_TO_PUBLIC(face_id);
	auto opposite_face_ID = retQSS_face_oppositeFaceID(public_face_id);

	retQSS_face_normal(opposite_face_ID, &nx, &ny, &nz);

	EXPECT_TRUE(retQSS::numbers_are_equal(nx, 0));
	EXPECT_TRUE(retQSS::numbers_are_equal(ny, 0));
	EXPECT_TRUE(retQSS::numbers_are_equal(nz, 1));

	auto opposite_vol_ID = retQSS_face_oppositeVolumeID(public_face_id);
	EXPECT_EQ(opposite_vol_ID, 96);
}

TEST_F(retQSSInterfaceTest, opposite_position_entrering_geometry)
{
	Point_3 pos(-45, -31.8122008077, 96.7604521314);
	Point_3 new_pos(-30, pos.y(), pos.z());

	retQSS::InitialConditionArray ic {
		{pos.x(), pos.y(), pos.z(), 1, 0, 0}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x, px, py, pz;
	x[0] = pos.x(), x[1] = 1, x[2] = 0, x[3] = 0;
	x[4] = pos.y(), x[5] = 0, x[6] = 0, x[7] = 0;
	x[8] = pos.z(), x[9] = 0, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	tracker->set_exit_state(0, data);
	tracker->update_state(0);

	tracker->sim_interface->set_position(0, new_pos);

	retQSS_particle_oppositePosition(1, &px, &py, &pz);

	EXPECT_TRUE(retQSS::numbers_are_equal(px, 30));
	EXPECT_TRUE(retQSS::numbers_are_equal(py, pos.y()));
	EXPECT_TRUE(retQSS::numbers_are_equal(pz, pos.z()));
}

TEST_F(retQSSInterfaceTest, opposite_position_leaving_geometry)
{
	Point_3 pos(-30, -31.8122008077, 96.7604521314);
	Point_3 new_pos(-45, pos.y(), pos.z());

	retQSS::InitialConditionArray ic {
		{pos.x(), pos.y(), pos.z(), -1, 0, 0}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x, px, py, pz;
	x[0] = pos.x(), x[1] = -1, x[2] = 0, x[3] = 0;
	x[4] = pos.y(), x[5] = 0, x[6] = 0, x[7] = 0;
	x[8] = pos.z(), x[9] = 0, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	tracker->set_exit_state(0, data);
	tracker->update_state(0);

	tracker->sim_interface->set_position(0, new_pos);

	retQSS_particle_oppositePosition(1, &px, &py, &pz);

	EXPECT_TRUE(retQSS::numbers_are_equal(px, 45));
	EXPECT_TRUE(retQSS::numbers_are_equal(py, pos.y()));
	EXPECT_TRUE(retQSS::numbers_are_equal(pz, pos.z()));
}

TEST_F(retQSSInterfaceTest, vectors)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 1);

	double x, y, z;

	retQSS_vectorWithNorm(2,3,4,10,&x,&y,&z);

	EXPECT_TRUE(retQSS::numbers_are_equal(x*x+y*y+z*z, 10*10));

	retQSS_randomVector(&x,&y,&z);

	EXPECT_GE(x, -1);
	EXPECT_LE(x, 1);
	EXPECT_GE(y, -1);
	EXPECT_LE(y, 1);
	EXPECT_GE(z, -1);
	EXPECT_LE(z, 1);

	retQSS_randomVectorWithNorm(15.,&x,&y,&z);

	EXPECT_TRUE(retQSS::numbers_are_equal(x*x+y*y+z*z, 15*15));
}

#define private public
#define protected public

#include <string>

#include "qss/qss_simulator.h"

#include "test_common/qss_test_utils.h"

#include "retqss/retqss_interface.hh"
#include "retqss/retqss_cgal_main_types.hh"
#include "retqss/retqss_geometry.hh"
#include "retqss/retqss_particle_tracker.hh"
#include "retqss/retqss_polyhedron_face.hh"
#include "retqss/retqss_volume.hh"
#include "retqss/retqss_trajectory.hh"
#include "retqss/retqss_face_crossing_algorithm.hh"
#include "retqss/retqss_types.hh"
#include "retqss/retqss_utilities.hh"

#include "gtest/gtest.h"

using namespace std;


class retQSSVolumeCrossingTest : public testing::Test
{
private:

	void TearDown()
	{
		retQSSTest::free_simulator(this->sim);
	}

public:
	QSS_simulator sim;
};


TEST_F(retQSSVolumeCrossingTest, dismiss_crossing_when_particle_near_boundary_is_entering)
{
	retQSS::InitialConditionArray ic {{-30, -30, 20+1e-13, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 269.5433, x[2] = 1., x[3] = 0;
	x[5] = 0, x[6] = 1., x[7] = 0;
	x[9] = 130.5458, x[10] = 1., x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto entry_face = vol->_faces[0];

	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	auto face = std::get<1>(data);

	EXPECT_NE(face, nullptr);
	EXPECT_NE(face, entry_face);
}

TEST_F(retQSSVolumeCrossingTest, detect_crossing_when_particle_near_boundary_is_exiting_polyhedral_volume)
{
	retQSS::InitialConditionArray ic {{-30, -30, 20+1e-13, 10, -2, -3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 10, x[2] = 0.01, x[3] = 0;
	x[5] = -2, x[6] = 0.01, x[7] = 0;
	x[9] = -3, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto entry_face = vol->_faces[0];

	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	auto face = std::get<1>(data);

	EXPECT_NE(face, nullptr);
	EXPECT_EQ(face, entry_face);
}

TEST_F(retQSSVolumeCrossingTest, detect_crossing_when_particle_near_boundary_is_exiting_open_volume)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-13, 10, 2, 3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 10, x[2] = 0.01, x[3] = 0;
	x[5] = 2, x[6] = 0.01, x[7] = 0;
	x[9] = 3, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto entry_face = vol->_faces[40];

	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	auto face = std::get<1>(data);

	EXPECT_NE(face, nullptr);
	EXPECT_EQ(face, entry_face);
}

TEST_F(retQSSVolumeCrossingTest, dismiss_crossing_when_particle_near_boundary_is_entering_open_volume_with_many_faces)
{
	retQSS::InitialConditionArray ic {{-30, -30, -1e-20, 10, 2, -3}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 10, x[2] = 0.01, x[3] = 0;
	x[5] = 2, x[6] = 0.01, x[7] = 0;
	x[9] = 3, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto entry_face = vol->_faces[40];

	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	auto face = std::get<1>(data);

	EXPECT_EQ(face, nullptr);
}

TEST_F(retQSSVolumeCrossingTest, entry_face_and_volume_when_exiting_open_volume)
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

	auto new_vol_before_update = tracker->next_volume(0);
	tracker->update_state(0);

	auto new_vol = tracker->current_volume(0);
	auto entry_face = tracker->entry_face(0);

	EXPECT_NE(new_vol, nullptr);
	EXPECT_NE(new_vol, geometry->world());
	EXPECT_EQ(new_vol_before_update, new_vol);
	EXPECT_NE(entry_face, nullptr);
}

TEST_F(retQSSVolumeCrossingTest, volume_crossing_through_face_with_multiple_neighbors)
{
	retQSS::InitialConditionArray ic {{7, 12, 2, 0.2, 1, -4}};
	this->sim = retQSSTest::initialize("../../data/box/geom/box.vtk", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 0.2, x[2] = 0.01, x[3] = 0;
	x[5] = 1, x[6] = 0.01, x[7] = 0;
	x[9] = -4, x[10] = 0, x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);

	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);
	tracker->set_exit_state(0, data);

	auto new_vol_before_update = tracker->next_volume(0);
	tracker->update_state(0);

	auto new_vol = tracker->current_volume(0);
	auto entry_face = tracker->entry_face(0);

	EXPECT_NE(new_vol, nullptr);
	EXPECT_EQ(new_vol->get_name(), "Voxel-11-16-19-21-14-17-20-3");
	EXPECT_EQ(new_vol_before_update, new_vol);
}

TEST_F(retQSSVolumeCrossingTest, face_crossing_in_small_volume)
{
	retQSS::InitialConditionArray ic {{-36.8413, -31.3836, 37.1106, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.off", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = x[2] = x[3] = 1.;
	x[5] = x[6] = x[7] = 1.;
	x[9] = x[10] = x[11] = 1.;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);

	Point_3 point = std::get<2>(data);
	Point_3 expected(-33.9519, -28.4942, 40);

	EXPECT_TRUE(retQSS::points_are_equal(point, expected));
}

TEST_F(retQSSVolumeCrossingTest, face_crossing_in_big_volume)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.off", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 2.837, x[2] = -20,  x[3] = 0;
	x[5] = 5.12, x[6] = -30.22,  x[7] = 0;
	x[9] = -20, x[10] = -2.9,  x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);

	auto crossed_face = std::get<1>(data);
	Point_3 point = std::get<2>(data);
	Point_3 expected(0.0975825, 2.21679595, 0.31546315);

	EXPECT_NE(crossed_face, nullptr);
	EXPECT_TRUE(retQSS::points_are_equal(point, expected));
}

TEST_F(retQSSVolumeCrossingTest, face_crossing_when_exiting_world)
{
	retQSS::InitialConditionArray ic {{0.04501281792, 2.17104938074112, -0.5172976009216002, 3.10892, 4.75328512, 20.7171584}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere_for_world_test.vtk", ic);
	auto tracker = RETQSS()->tracker();
	auto face_crossing = RETQSS()->crossing_algorithm();

	double *x = sim->data->x;
	x[1] = 2.10892, x[2] = -20,  x[3] = 0;
	x[5] = 2.35328512, x[6] = -30.22,  x[7] = 0;
	x[9] = 20.7171584, x[10] = -2.9,  x[11] = 0;

	retQSS::Volume *vol = tracker->current_volume(0);
	auto particle = tracker->get_particle(0);
	particle->trajectory()->update();

	auto data = face_crossing->find_crossed_face(particle, vol);

	auto crossed_face = std::get<1>(data);
	Point_3 point = std::get<2>(data);
	Point_3 expected(0.097582514, 2.216795953, 0.31546346);

	EXPECT_NE(crossed_face, nullptr);
	EXPECT_TRUE(retQSS::points_are_equal(point, expected));
}

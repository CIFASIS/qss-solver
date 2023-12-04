#define private public
#define protected public

#include <string>

#include "qss/qss_simulator.h"

#include "test_common/qss_test_utils.h"

#include "retqss/retqss_interface.hh"
#include "retqss/retqss_geometry.hh"
#include "retqss/retqss_volume.hh"
#include "retqss/retqss_logical_face.hh"
#include "retqss/retqss_types.hh"

#include "gtest/gtest.h"

using namespace std;


class retQSSPropertiesTest : public testing::Test
{
private:

	void TearDown()
	{
		retQSSTest::free_simulator(this->sim);
	}

public:
	QSS_simulator sim;
};


TEST_F(retQSSPropertiesTest, particles_inside_initial_volumes)
{
	retQSS::InitialConditionArray ic {
		{-0.1, -0.1, -0.91, 269.5433, 0, 130.5458},
		{0.229426, 0.0742439, -0.842883, 269.5433, 0, 130.5458},
		{10, 10, 10, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 3);
	auto tracker = RETQSS()->tracker();
	auto geometry = RETQSS()->geometry();

	auto current = tracker->current_volume(0);
	int n_particles;

	for(size_t i = 1; i < geometry->num_volumes(); i++)
	{
		n_particles = RETQSS()->volume_count_particles_inside(i);

		if(i == current->get_ID())
			EXPECT_EQ(n_particles, 2);
		else
			EXPECT_EQ(n_particles, 0);
	}

	n_particles = RETQSS()->volume_count_particles_inside(0);
	EXPECT_EQ(n_particles, 1);
}

TEST_F(retQSSPropertiesTest, ids_of_particles_inside_volume)
{
	retQSS::InitialConditionArray ic {
		{-0.1, -0.1, -0.91, 269.5433, 0, 130.5458},
		{0.229426, 0.0742439, -0.842883, 269.5433, 0, 130.5458},
		{10, 10, 10, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 3);
	auto tracker = RETQSS()->tracker();

	auto current = tracker->current_volume(0);
	auto id = current->get_ID();

	auto id1 = RETQSS()->volume_ID_of_particle_inside(id,0);
	auto id2 = RETQSS()->volume_ID_of_particle_inside(id,1);

	EXPECT_NE(id1, id2);
	EXPECT_TRUE(id1 == 0 || id2 == 0);
	EXPECT_TRUE(id1 == 1 || id2 == 1);

	EXPECT_EQ(RETQSS()->volume_ID_of_particle_inside(0,0), 2);
}

TEST_F(retQSSPropertiesTest, particles_inside_volume_after_relocation)
{
	retQSS::InitialConditionArray ic {
		{0, 0, 0, 269.5433, 0, 130.5458},
		{5, -10, 100, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();

	auto v0 = RETQSS()->particle_current_volume_ID(0);
	auto v1 = RETQSS()->particle_current_volume_ID(1);

	auto vol0 = geometry->get_polyhedral_volume(v0);
	auto vol1 = geometry->get_polyhedral_volume(v1);

	auto part0 = vol0->particles_inside();
	auto part1 = vol1->particles_inside();

	EXPECT_EQ(part0.size(), 1);
	EXPECT_EQ(part1.size(), 1);

	auto point = vol1->random_point();
	v0 = RETQSS()->particle_relocate(0, point, Vector_3(1, 1, 1));

	EXPECT_EQ(v0, v1);

	part0 = vol0->particles_inside();
	part1 = vol1->particles_inside();

	EXPECT_EQ(part0.size(), 0);
	EXPECT_EQ(part1.size(), 2);
}

TEST_F(retQSSPropertiesTest, particle_properties)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto tracker = RETQSS()->tracker();
	auto particle = tracker->get_particle(0);
	auto tag = "test_prop";

	RETQSS()->particle_set_property_vector(0, tag, Vector_3(1,2,3));

	auto has_prop = RETQSS()->particle_has_property(0, "other_prop");
	EXPECT_FALSE(has_prop);

	has_prop = RETQSS()->particle_has_property(0, tag);
	EXPECT_TRUE(has_prop);

	int rv;
	double x, y, z;

	RETQSS()->particle_get_property_vector(0, tag, &rv, &x, &y, &z);
	EXPECT_EQ(x, 1);
	EXPECT_EQ(y, 2);
	EXPECT_EQ(z, 3);

	auto first_value = RETQSS()->particle_get_property(0, tag);
	EXPECT_EQ(first_value, 1);

	RETQSS()->particle_set_property(0, tag, 50);
	RETQSS()->particle_get_property_vector(0, tag, &rv, &x, &y, &z);
	EXPECT_EQ(x, 50);
	EXPECT_EQ(y, 50);
	EXPECT_EQ(z, 50);
}

TEST_F(retQSSPropertiesTest, vertex_properties)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();
	auto vol1 = geometry->get_volume(2);
	auto vol2 = geometry->get_volume(14);

	EXPECT_EQ(vol1->num_vertices(), 8);
	EXPECT_EQ(vol2->num_vertices(), 8);

	auto vertex1 = vol1->get_vertex(7);
	auto vertex2 = vol2->get_vertex(5);

	vertex1->set_property("test_property", 15);

	EXPECT_TRUE(RETQSS()->vertex_has_property(vertex1->get_ID(), "test_property"));
	EXPECT_FALSE(RETQSS()->vertex_has_property(5, "test_property"));
	EXPECT_FALSE(RETQSS()->vertex_has_property(7, "test_property_2"));

	EXPECT_EQ(vertex1->get_property("test_property"), 15);
	EXPECT_EQ(vertex2->get_property("test_property"), 15);
}

TEST_F(retQSSPropertiesTest, volume_properties)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();

	RETQSS()->volume_set_property(2, "test_property", 100);

	EXPECT_TRUE(RETQSS()->volume_has_property(2, "test_property"));
	EXPECT_EQ(RETQSS()->volume_get_property(2, "test_property"), 100);

	EXPECT_FALSE(RETQSS()->volume_has_property(2, "test_property_2"));
	EXPECT_FALSE(RETQSS()->volume_has_property(0, "test_property"));
	EXPECT_FALSE(RETQSS()->volume_has_property(10, "test_property"));
}

TEST_F(retQSSPropertiesTest, face_properties)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();
	auto tag = "test_property";
	auto n = geometry->num_logical_faces();

	RETQSS()->face_set_property(10, tag, 20);

	EXPECT_TRUE(RETQSS()->face_has_property(10, tag));
	EXPECT_EQ(RETQSS()->face_get_property(10, tag), 20);

	EXPECT_FALSE(RETQSS()->face_has_property(10, "test_property_2"));
	EXPECT_FALSE(RETQSS()->face_has_property(0, tag));
	EXPECT_FALSE(RETQSS()->face_has_property(n-1, tag));
}

TEST_F(retQSSPropertiesTest, static_properties_in_volumes)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();

	for(auto vol : geometry->volumes())
	{
		double capacity = vol->capacity();

		double err = std::abs(capacity - 8000);
		EXPECT_LT(err, 1e-7);
	}

	auto vol = geometry->get_polyhedral_volume(1);
	auto centroid = vol->centroid();

	EXPECT_EQ(centroid.x(), -30);
	EXPECT_EQ(centroid.y(), -70);
	EXPECT_EQ(centroid.z(), 10);
}

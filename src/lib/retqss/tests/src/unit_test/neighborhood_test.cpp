#define private public
#define protected public

#include <string>

#include "qss/qss_simulator.h"

#include "test_common/qss_test_utils.h"

#include "retqss/retqss_interface.hh"
#include "retqss/retqss_cgal_main_types.hh"
#include "retqss/retqss_geometry.hh"
#include "retqss/retqss_polyhedron_face.hh"
#include "retqss/retqss_volume.hh"
#include "retqss/retqss_face_crossing_algorithm.hh"
#include "retqss/retqss_types.hh"
#include "retqss/retqss_utilities.hh"
#include "retqss/retqss_particle_neighbor.hh"
#include "retqss/retqss_volume_neighbor.hh"
#include "retqss/retqss_model_api.h"

#include "gtest/gtest.h"

using namespace std;


class retQSSNeighborhoodTest : public testing::Test
{
private:

	void TearDown()
	{
		retQSSTest::free_simulator(this->sim);
	}

public:
	QSS_simulator sim;
};


extern "C"
{
bool particle_test_func(retQSS::ParticleNeighbor *neigh, Vector_3 &v)
{
	auto pos = neigh->neighbor_position();
	v += Vector_3(pos.x(), pos.y(), pos.z());
	return true;
}

bool particle_test_func_args(
		retQSS::ParticleNeighbor *neigh,
		const std::vector<double> &args,
		Vector_3 &v)
{
	auto pos = neigh->neighbor_position();
	v += Vector_3(pos.x()*args[0], pos.y(), pos.z()*args[1]);
	return true;
}

Vector_3 particle_neighborhood_test_func_args(
		retQSS::ParticleNeighborhood *neighborhood,
		double arg1, double arg2)
{
	Vector_3 v(0,0,0);

	for(auto neigh : *neighborhood)
	{
		auto pos = neigh->neighbor_position();
		v += Vector_3(pos.x()*arg1, pos.y(), pos.z()*arg2);
	}

	return v;
}
}


TEST_F(retQSSNeighborhoodTest, face_neighborhood_test)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.off", ic);
	auto tracker = RETQSS()->tracker();

	retQSS::Volume *vol = tracker->current_volume(0);
	auto face = vol->_faces[0];
	auto neighborhood = retQSS::FaceNeighborhood(vol, face, 10000);
	std::set<retQSS::PolyhedronFace*> faces;

	for(auto neighbor: neighborhood)
		faces.insert(neighbor);

	EXPECT_EQ(faces.size(), vol->num_faces());
	for(auto f: vol->_faces)
		EXPECT_FALSE(faces.find(f) == faces.end());
}

TEST_F(retQSSNeighborhoodTest, volume_neighborhood_from_file)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	RETQSS()->volume_neighborhood_to_file("../../data/neighborhood/file_neighborhood.txt");

	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(8), 2);
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(7), 1);
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(50), 5);
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(1), 0);

	EXPECT_EQ(RETQSS()->volume_neighborhood_neighbor_ID(50, 0), 20);
	EXPECT_EQ(RETQSS()->volume_neighborhood_neighbor_ID(50, 1), 35);
	EXPECT_EQ(RETQSS()->volume_neighborhood_neighbor_ID(50, 2), 2);
	EXPECT_EQ(RETQSS()->volume_neighborhood_neighbor_ID(50, 3), 51);
	EXPECT_EQ(RETQSS()->volume_neighborhood_neighbor_ID(50, 4), 101);
}

TEST_F(retQSSNeighborhoodTest, vertex_sharing_volume_neighborhood)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);
	auto geometry = RETQSS()->geometry();

	RETQSS()->volume_neighborhood_to_vertex_sharing();

	int count = RETQSS()->volume_neighborhood_count_volumes(8);
	EXPECT_EQ(count, 11);

	auto vol = geometry->get_volume(8);
	int idx = 1;
	while(idx <= count)
	{
		bool ok = false;

		auto id = RETQSS()->volume_neighborhood_neighbor_ID(8, idx-1);
		auto neighbor = geometry->get_volume(id);

		EXPECT_NE(id, 8);

		for(auto vertex : neighbor->vertices())
		{
			bool found = false;

			for(auto vertex_vol : vol->vertices())
				if(vertex_vol == vertex)
				{
					found = true;
					break;
				}

			if(found)
			{
				ok = true;
				break;
			}
		}

		EXPECT_TRUE(ok);

		idx++;
	}
}

TEST_F(retQSSNeighborhoodTest, radial_volume_neighborhood)
{
	// HINT: use ParaView to visualize the geometry and the neighborhoods.

	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	// Cube size is 20 mm. Closest neighbor is thus 10 mm away from centroid.
	// Volume ID 146 corresponds to an internal cube.
	RETQSS()->volume_neighborhood_to_radial(9);
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(146), 1);

	// There should be now 6 neighbors (one for each face; not enough distance
	// to get to the others).
	RETQSS()->volume_neighborhood_to_radial(10.001);
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(146), 7);

	RETQSS()->volume_neighborhood_to_radial(30.001);
	std::vector<int> expected {146,86,134,145,147,158,206,74,85,87,98,133,135,157,159,194,205,207,218,73,75,97,99,193,195,217,219,26,122,148,170};

	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(146), expected.size());
	for(size_t i = 1; i <= expected.size(); i++)
		EXPECT_EQ(RETQSS()->volume_neighborhood_neighbor_ID(146,i-1), expected[i-1]);

	auto vol = RETQSS()->geometry()->get_volume(146);
	auto neighbors = RETQSS()->neighborhood()->neighbors(vol);
	for(Index i = 0; i < expected.size(); i++)
		EXPECT_LE(neighbors[i]->distance(), 30);

	// World should not have any radial neighbors.
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(0), 0);
}

// Uncomment to get a neighborhood to feed to ParaView for visualization.
/*
TEST_F(retQSSNeighborhoodTest, periodic_radial_volume_neighborhood_in_sphere)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere_mesh.vtk", ic);

	auto neighborhood = RETQSS()->neighborhood();
	auto geometry = RETQSS()->geometry();
	RETQSS()->volume_neighborhood_to_periodic_radial(0.1);

	auto vol = geometry->get_volume(12721); // 12720 in ParaView
	auto neighs = neighborhood->neighbors(vol);

	for(auto neigh : neighs)
		std::cout << (neigh->get_ID()-1) << ",";
	std::cout << std::endl;
}
*/

TEST_F(retQSSNeighborhoodTest, periodic_radial_volume_neighborhood)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto neighborhood = RETQSS()->neighborhood();
	auto geometry = RETQSS()->geometry();

	RETQSS()->volume_neighborhood_to_periodic_radial(15.001);

	auto vol = geometry->get_volume(12);

	std::vector<VolumeID> expected {12,1,11,24,60,72,192,13,23,49,59,61,71,84,120,181,191,204,240};

	EXPECT_EQ(
			RETQSS()->volume_neighborhood_count_volumes(12),
			expected.size());

	for(Index i = 0; i < expected.size(); i++)
		EXPECT_EQ(
				RETQSS()->volume_neighborhood_neighbor_ID(12,i),
				expected[i]);

	auto neighbors = neighborhood->neighbors(vol);
	for(Index i = 0; i < expected.size(); i++)
		EXPECT_LE(neighbors[i]->distance(), 15);
}

TEST_F(retQSSNeighborhoodTest, switch_between_neighborhoods)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	RETQSS()->volume_neighborhood_to_file("../../data/neighborhood/file_neighborhood.txt");
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(8), 2);

	RETQSS()->volume_neighborhood_to_vertex_sharing();
	EXPECT_EQ(RETQSS()->volume_neighborhood_count_volumes(8), 11);
}

TEST_F(retQSSNeighborhoodTest, neighborhood_properties)
{
	retQSS::InitialConditionArray ic {{0, 2, 2, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	RETQSS()->volume_neighborhood_to_radial(10.001);

	RETQSS()->volume_set_property(146, "test_property", 200);
	RETQSS()->volume_neighborhood_set_property(146, "test_property", 100);

	EXPECT_EQ(RETQSS()->volume_get_property(146, "test_property"), 100);
	for(size_t i = 1; i <= RETQSS()->volume_neighborhood_count_volumes(146); i++)
	{
		auto neighbor_id = RETQSS()->volume_neighborhood_neighbor_ID(146,i-1);
		EXPECT_EQ(RETQSS()->volume_get_property(neighbor_id, "test_property"), 100);
	}
}

TEST_F(retQSSNeighborhoodTest, particle_neighborhood_size)
{
	retQSS::InitialConditionArray ic {
		{0, 2, 2, 2.837, 5.12, -20},
		{0, 1, 1, 2.837, 5.12, -20},
		{20.5, 2, 2, 2.837, 5.12, -20},
		{0, 1, 22.1, 2.837, 5.12, -20},
		{30, 0, 100, 2, 2, 2},
		{100, 20, 20, 2.837, 5.12, -20},
		{0, 3, 3, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic, ic.size());

	RETQSS()->volume_neighborhood_to_radial(21);

	EXPECT_EQ(RETQSS()->particle_neighborhood_count_particles(0), 4);
	EXPECT_EQ(RETQSS()->particle_neighborhood_count_particles(4), 0);
	EXPECT_EQ(RETQSS()->particle_neighborhood_count_particles(5), 0);
}

TEST_F(retQSSNeighborhoodTest, particle_neighborhood_size_with_dead_particles)
{
	retQSS::InitialConditionArray ic {
		{0, 2, 2, 2.837, 5.12, -20},
		{0, 1, 1, 2.837, 5.12, -20},
		{20.5, 2, 2, 2.837, 5.12, -20},
		{0, 1, 22.1, 2.837, 5.12, -20},
		{30, 0, 100, 2, 2, 2},
		{100, 20, 20, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic, ic.size());

	RETQSS()->volume_neighborhood_to_radial(21);

	RETQSS()->particle_kill(2);
	EXPECT_EQ(RETQSS()->particle_neighborhood_count_particles(0), 2);

	RETQSS()->particle_kill(1);
	EXPECT_EQ(RETQSS()->particle_neighborhood_count_particles(0), 1);

	RETQSS()->particle_kill(3);
	EXPECT_EQ(RETQSS()->particle_neighborhood_count_particles(0), 0);
}

TEST_F(retQSSNeighborhoodTest, particle_neighborhood_avg_position)
{
	retQSS::InitialConditionArray ic {
		{0, 2, 2, 2.837, 5.12, -20},
		{0, 1, 1, 2.837, 5.12, -20},
		{20.5, 2, 2, 2.837, 5.12, -20},
		{0, 1, 22.1, 2.837, 5.12, -20},
		{30, 0, 100, 2, 2, 2},
		{100, 20, 20, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic, ic.size());

	RETQSS()->volume_neighborhood_to_radial(21);

	double neighbors;
	double x, y, z;

	RETQSS()->particle_neighborhood_average_position(0,&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 3);

	EXPECT_TRUE(retQSS::numbers_are_equal(x, 6.8333333));
	EXPECT_TRUE(retQSS::numbers_are_equal(y, 1.3333333));
	EXPECT_TRUE(retQSS::numbers_are_equal(z, 8.3666667));

	RETQSS()->particle_neighborhood_average_position(5,&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 0);
}

TEST_F(retQSSNeighborhoodTest, particle_neighborhood_avg_velocity)
{
	retQSS::InitialConditionArray ic {
		{0, 2, 2, 2.837, 5.12, -20},
		{0, 1, 1, 2.837, 5.12, -20},
		{20.5, 2, 2, 2.837, 5.12, -20},
		{0, 1, 22.1, 2.837, 5.12, -20},
		{30, 0, 100, 2, 2, 2},
		{100, 20, 20, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic, ic.size());

	RETQSS()->volume_neighborhood_to_radial(21);

	double neighbors;
	double x, y, z;

	RETQSS()->particle_neighborhood_average_velocity(0,&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 3);

	EXPECT_TRUE(retQSS::numbers_are_equal(x, 2.837));
	EXPECT_TRUE(retQSS::numbers_are_equal(y, 5.12));
	EXPECT_TRUE(retQSS::numbers_are_equal(z, -20));

	RETQSS()->particle_neighborhood_average_velocity(5,&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 0);
}

TEST_F(retQSSNeighborhoodTest, particle_neighborhood_repulsive_direction)
{
	retQSS::InitialConditionArray ic {
		{0, 2, 2, 2.837, 5.12, -20},
		{0, 1, 1, 2.837, 5.12, -20},
		{20.5, 2, 2, 2.837, 5.12, -20},
		{0, 1, 22.1, 2.837, 5.12, -20},
		{30, 0, 100, 2, 2, 2},
		{100, 20, 20, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic, ic.size());

	RETQSS()->volume_neighborhood_to_radial(21);

	double neighbors;
	double x, y, z;

	RETQSS()->particle_neighborhood_repulsive_direction(0,&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 3);

	EXPECT_TRUE(retQSS::numbers_are_equal(x*x+y*y+z*z, 1.));
	EXPECT_TRUE(retQSS::numbers_are_equal(x, -0.7766640926));
	EXPECT_TRUE(retQSS::numbers_are_equal(y, 0.5877767191));
	EXPECT_TRUE(retQSS::numbers_are_equal(z, -0.2265202325));

	RETQSS()->particle_neighborhood_repulsive_direction(5,&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 0);
}

TEST_F(retQSSNeighborhoodTest, particle_neighborhood_for_each)
{
	retQSS::InitialConditionArray ic {
		{0, 2, 2, 2.837, 5.12, -20},
		{0, 1, 1, 2.837, 5.12, -20},
		{20.5, 2, 2, 2.837, 5.12, -20},
		{0, 1, 22.1, 2.837, 5.12, -20},
		{30, 0, 100, 2, 2, 2},
		{100, 20, 20, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic, ic.size());

	RETQSS()->volume_neighborhood_to_radial(21);

	double neighbors;
	double x, y, z;

	RETQSS()->particle_neighborhood_for_each_particle(0,"particle_test_func",{},&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 3);

	EXPECT_TRUE(retQSS::numbers_are_equal(x, 20.5));
	EXPECT_TRUE(retQSS::numbers_are_equal(y, 4));
	EXPECT_TRUE(retQSS::numbers_are_equal(z, 25.1));

	std::vector<double> args {2,3};
	RETQSS()->particle_neighborhood_for_each_particle(0,"particle_test_func_args",args,&neighbors,&x,&y,&z);

	EXPECT_EQ(neighbors, 3);

	EXPECT_TRUE(retQSS::numbers_are_equal(x, 20.5*args[0]));
	EXPECT_TRUE(retQSS::numbers_are_equal(y, 4));
	EXPECT_TRUE(retQSS::numbers_are_equal(z, 25.1*args[1]));
}

TEST_F(retQSSNeighborhoodTest, particle_neighborhood_evaluate)
{
	retQSS::InitialConditionArray ic {
		{0, 2, 2, 2.837, 5.12, -20},
		{0, 1, 1, 2.837, 5.12, -20},
		{20.5, 2, 2, 2.837, 5.12, -20},
		{0, 1, 22.1, 2.837, 5.12, -20},
		{30, 0, 100, 2, 2, 2},
		{100, 20, 20, 2.837, 5.12, -20}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic, ic.size());

	RETQSS()->volume_neighborhood_to_radial(21);

	double neighbors;
	double x, y, z;

	double arg1 = 2;
	double arg2 = 3;

	RETQSS()->particle_neighborhood_evaluate_2(0,"particle_neighborhood_test_func_args",arg1,arg2,&neighbors,&x,&y,&z);

	EXPECT_TRUE(retQSS::numbers_are_equal(x, 20.5*arg1));
	EXPECT_TRUE(retQSS::numbers_are_equal(y, 4));
	EXPECT_TRUE(retQSS::numbers_are_equal(z, 25.1*arg2));
}

TEST_F(retQSSNeighborhoodTest, particles_in_periodic_volume_neighborhood)
{
	retQSS::InitialConditionArray ic {
		{-23.631834,-60.116973,236.08436,1,1,1},
		{-22,-61,235,1,1,1},
		{-30,-70,10,1,1,1},
		{-30,-70,210,1,1,1},
		{-30,-50,230,1,1,1},
		{-30,10,230,1,1,1},
		{-10,-70,230,1,1,1},
		{30,-70,230,1,1,1},
		{-30,-50,10,1,1,1},
		{-30,-50,210,1,1,1},
		{-30,10,10,1,1,1},
		{-30,10,210,1,1,1},
		{-10,-70,10,1,1,1},
		{-10,-70,210,1,1,1},
		{-10,-50,230,1,1,1},
		{-10,10,230,1,1,1},
		{30,-70,10,1,1,1},
		{30,-70,210,1,1,1},
		{30,-50,230,1,1,1},
		{30,10,230,1,1,1}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto neighborhood = RETQSS()->neighborhood();
	auto geometry = RETQSS()->geometry();

	RETQSS()->volume_neighborhood_to_periodic_radial(15.001);

	auto particle_neigh = RETQSS()->get_particle_neighborhood(0);
	size_t count = 0;

	// Expected IDs of volumes containing partilces whose Euclidean distance
	// is greater that the virtual distance through the periodic boundary.
	std::set<size_t> expected = {1,60,192,13,49,59,61,120,181,191,204,240};

	for(auto neighbor : *particle_neigh)
	{
		auto dist = neighbor->distance();
		auto euclidean_dist = neighbor->euclidean_distance();
		auto vol_id = neighbor->neighbor_volume()->get_ID();

		if(!retQSS::numbers_are_equal(dist, euclidean_dist))
			EXPECT_NE(expected.find(vol_id), expected.end());
		else
			EXPECT_EQ(expected.find(vol_id), expected.end());

		if(vol_id == 49)
		{
			auto shortest_vec = neighbor->shortest_vector();
			Vector_3 expected_shortest(-6.368166, -29.883027, 13.91564);
			EXPECT_TRUE(retQSS::vectors_are_equal(shortest_vec, expected_shortest));

			auto euclidean_vec = neighbor->euclidean_vector();
			Vector_3 expected_euclidean(-6.368166, 70.116973, -226.08436);
			EXPECT_TRUE(retQSS::vectors_are_equal(euclidean_vec, expected_euclidean));
		}

		count++;
	}

	EXPECT_EQ(count, ic.size()-1);
}

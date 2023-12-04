#define private public
#define protected public

#include <string>

#include "qss/qss_simulator.h"

#include "test_common/qss_test_utils.h"

#include "retqss/retqss_interface.hh"
#include "retqss/retqss_cgal_main_types.hh"
#include "retqss/retqss_geometry.hh"
#include "retqss/retqss_particle_tracker.hh"
#include "retqss/retqss_logical_face.hh"
#include "retqss/retqss_polyhedron_face.hh"
#include "retqss/retqss_volume.hh"
#include "retqss/retqss_types.hh"
#include "retqss/retqss_utilities.hh"

#include "gtest/gtest.h"

using namespace std;


class retQSSParserTest : public testing::Test
{
private:
	retQSS::Volume *find_volume(const std::string &name)
	{
		auto geometry = RETQSS()->geometry();
		for(auto vol : geometry->volumes())
			if(vol->get_name() == name)
				return vol;

		return nullptr;
	}

	bool is_neighbor(retQSS::Volume *vol, const std::string &neighbor_name)
	{
		for(auto face : vol->faces())
		{
			auto it = vol->face_to_face.find(face);

			if(it == vol->face_to_face.end())
				continue;

			for(auto neighbor_face : it->second)
				if(neighbor_face->volume()->get_name() == neighbor_name)
					return true;
		}

		return false;
	}

	void TearDown()
	{
		retQSSTest::free_simulator(this->sim);
	}

public:
	QSS_simulator sim;
};


TEST_F(retQSSParserTest, OFF_geometry_test)
{
	retQSS::InitialConditionArray ic {{0,0,0,1,0,1}};
	this->sim = retQSSTest::initialize("../../data/helix/geom/helix", ic);
	auto tracker = RETQSS()->tracker();

	retQSS::Volume *current = tracker->current_volume(0);
	EXPECT_NE(current, nullptr);
	EXPECT_EQ(current->get_name(), "Cube-2-0-0_34");

	std::string vol_name = "Cube-2-0-1_35";

	std::string neighbor1_name = "Cube-2-0-0_34";
	std::string neighbor2_name = "Cube-2-0-2_36";

	std::string neighbor3_name = "Cube-2-1-1_39";

	std::string neighbor4_name = "Cube-3-0-1_51";
	std::string neighbor5_name = "Cube-1-0-1_19";

	std::string non_neighbor1_name = "Cube-3-0-0_50";
	std::string non_neighbor2_name = "Cube-1-1-1_23";

	retQSS::Volume *vol = this->find_volume(vol_name);

	EXPECT_NE(vol, nullptr);

	EXPECT_TRUE(this->is_neighbor(vol, neighbor1_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor2_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor3_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor4_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor5_name));

	EXPECT_FALSE(this->is_neighbor(vol, non_neighbor1_name));
	EXPECT_FALSE(this->is_neighbor(vol, non_neighbor2_name));
}

TEST_F(retQSSParserTest, VTK_geometry_initial_volume_test_1)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic);
	auto tracker = RETQSS()->tracker();

	retQSS::Volume *current = tracker->current_volume(0);
	EXPECT_NE(current, nullptr);
	EXPECT_EQ(current->get_name(), "Tetra-452-127-139-3");
}

TEST_F(retQSSParserTest, VTK_geometry_initial_volume_test_2)
{
	retQSS::InitialConditionArray ic {{-0.1, -0.1, -0.91, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic);
	auto tracker = RETQSS()->tracker();

	retQSS::Volume *current = tracker->current_volume(0);
	EXPECT_NE(current, nullptr);
	EXPECT_EQ(current->get_name(), "Tetra-127-139-3-84");
}

TEST_F(retQSSParserTest, initial_volume_with_multiple_particles)
{
	retQSS::InitialConditionArray ic {
		{-0.1, -0.1, -0.91, 269.5433, 0, 130.5458},
		{0.186449, -0.0221191, -0.933383, 269.5433, 0, 130.5458},
		{10, 10, 10, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic, 3);
	auto tracker = RETQSS()->tracker();
	auto geometry = RETQSS()->geometry();

	retQSS::Volume *current = tracker->current_volume(0);
	EXPECT_NE(current, nullptr);
	EXPECT_EQ(current->get_name(), "Tetra-127-139-3-84");

	current = tracker->current_volume(1);
	EXPECT_NE(current, nullptr);
	EXPECT_EQ(current->get_name(), "Tetra-127-137-84-129");

	current = tracker->current_volume(2);
	EXPECT_NE(current, nullptr);
	EXPECT_EQ(current, geometry->world());
}

TEST_F(retQSSParserTest, VTK_geometry_with_cell_data)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();

	for(auto vol : geometry->volumes())
	{
		auto values1 = vol->get_property_array("property1");
		auto values2 = vol->get_property_array("property2");
		auto values3 = vol->get_property_array("property3");
		double value3 = vol->get_property("property3");

		EXPECT_EQ(values1.size(), 1);
		EXPECT_EQ(values1[0], 1);

		EXPECT_EQ(values2.size(), 3);
		EXPECT_EQ(values2[0], 3);
		EXPECT_EQ(values2[1], 2);
		EXPECT_EQ(values2[2], 4);

		EXPECT_EQ(values3.size(), 1);
		EXPECT_EQ(values3[0], vol->get_ID());
		EXPECT_EQ(value3, vol->get_ID());
	}
}

TEST_F(retQSSParserTest, VTK_geometry_with_voxels)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	std::string vol_name = "Voxel-118-196-131-209-119-197-132-210";

	std::string neighbor1_name = "Voxel-40-118-53-131-41-119-54-132";
	std::string neighbor2_name = "Voxel-105-183-118-196-106-184-119-197";
	std::string neighbor3_name = "Voxel-117-195-130-208-118-196-131-209";
	std::string neighbor4_name = "Voxel-119-197-132-210-120-198-133-211";
	std::string neighbor5_name = "Voxel-131-209-144-222-132-210-145-223";
	std::string neighbor6_name = "Voxel-196-274-209-287-197-275-210-288";

	std::string non_neighbor1_name = "Voxel-91-169-104-182-92-170-105-183";
	std::string non_neighbor2_name = "Voxel-92-170-105-183-93-171-106-184";

	auto vol = dynamic_cast<retQSS::PolyhedralVolume*>(this->find_volume(vol_name));

	EXPECT_NE(vol, nullptr);
	EXPECT_EQ(vol->num_adjacent_volumes(), 6);

	EXPECT_TRUE(this->is_neighbor(vol, neighbor1_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor2_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor3_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor4_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor5_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor6_name));

	EXPECT_FALSE(this->is_neighbor(vol, non_neighbor1_name));
	EXPECT_FALSE(this->is_neighbor(vol, non_neighbor2_name));
}

TEST_F(retQSSParserTest, VTK_single_hexahedron_cell)
{
	retQSS::InitialConditionArray ic {{1, 1, 1, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/hexahedron/hexahedron.vtk", ic);

	auto geometry = RETQSS()->geometry();

	int n_vols = RETQSS()->geometry_count_volumes();
	EXPECT_EQ(n_vols, 1);

	auto vol = geometry->get_polyhedral_volume(1);
	EXPECT_EQ(vol->get_name(), "Hexa-0-1-2-3-4-5-6-7");
	EXPECT_EQ(vol->num_faces(), 6);
	EXPECT_EQ(vol->num_vertices(), 8);
}

TEST_F(retQSSParserTest, VTK_geometry_with_hexahedrons)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/torus/torus.vtk", ic);

	auto geometry = RETQSS()->geometry();
	auto world = geometry->world();

	int n_vols = RETQSS()->geometry_count_volumes();
	int n_verts = RETQSS()->geometry_count_vertices();
	EXPECT_EQ(n_verts, 4664);
	EXPECT_EQ(n_vols, 3695);

	for(int i = 1; i <= n_vols; i++)
	{
		auto vol = geometry->get_polyhedral_volume(i);
		EXPECT_EQ(vol->num_adjacent_volumes(), 6);
	}

	auto vol = geometry->get_polyhedral_volume(3694);
	bool found = false;
	for(size_t i = 0; i < 6; i++)
	{
		if(vol->get_adjacent_volume(i) == world)
			found = true;
	}
	EXPECT_TRUE(found);
}

TEST_F(retQSSParserTest, volume_with_multiple_neighbors_in_same_face)
{
	retQSS::InitialConditionArray ic {{7, 12, 2, 0, 0, -4}};
	this->sim = retQSSTest::initialize("../../data/box/geom/box.vtk", ic);
	auto tracker = RETQSS()->tracker();

	auto vol = tracker->current_volume(0);
	EXPECT_EQ(vol->get_name(), "Voxel-0-1-2-3-4-5-6-7");

	auto vol1 = this->find_volume("Voxel-8-9-10-11-0-12-13-14");
	auto vol2 = this->find_volume("Voxel-9-15-11-16-12-1-14-17");
	auto vol3 = this->find_volume("Voxel-10-11-18-19-13-14-2-20");
	auto vol4 = this->find_volume("Voxel-11-16-19-21-14-17-20-3");

	EXPECT_TRUE(this->is_neighbor(vol, vol1->get_name()));
	EXPECT_TRUE(this->is_neighbor(vol, vol2->get_name()));
	EXPECT_TRUE(this->is_neighbor(vol, vol3->get_name()));
	EXPECT_TRUE(this->is_neighbor(vol, vol4->get_name()));

	EXPECT_TRUE(this->is_neighbor(vol1, vol->get_name()));
	EXPECT_TRUE(this->is_neighbor(vol2, vol->get_name()));
	EXPECT_TRUE(this->is_neighbor(vol3, vol->get_name()));
	EXPECT_TRUE(this->is_neighbor(vol4, vol->get_name()));
}

TEST_F(retQSSParserTest, VTK_big_geometry_test)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 0, 0, 0}};
	this->sim = retQSSTest::initialize("../../data/brain/geom/brain.vtk", ic);

	std::string vol_name = "Tetra-3170-3169-3159-13538";

	std::string neighbor1_name = "Tetra-3169-3445-3159-13538";
	std::string neighbor2_name = "Tetra-2907-3170-3159-13538";
	std::string neighbor3_name = "Tetra-3169-3170-3463-13538";
	std::string neighbor4_name = "Tetra-2918-3170-3169-3159";

	std::string non_neighbor1_name = "Tetra-8344-10422-8345-12552";
	std::string non_neighbor2_name = "Tetra-14686-14379-16048-16120";

	retQSS::Volume *vol = this->find_volume(vol_name);

	EXPECT_NE(vol, nullptr);

	EXPECT_TRUE(this->is_neighbor(vol, neighbor1_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor2_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor3_name));
	EXPECT_TRUE(this->is_neighbor(vol, neighbor4_name));

	EXPECT_FALSE(this->is_neighbor(vol, non_neighbor1_name));
	EXPECT_FALSE(this->is_neighbor(vol, non_neighbor2_name));
}

TEST_F(retQSSParserTest, polyhedral_volume_vertices)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();

	EXPECT_EQ(geometry->num_vertices(), 390);

	auto vol = geometry->get_volume(2);

	EXPECT_EQ(vol->num_vertices(), 8);
	EXPECT_EQ(vol->get_vertex(0), geometry->get_vertex(4));
	EXPECT_EQ(vol->get_vertex(1), geometry->get_vertex(5));
	EXPECT_EQ(vol->get_vertex(2), geometry->get_vertex(6));
	EXPECT_EQ(vol->get_vertex(3), geometry->get_vertex(7));
	EXPECT_EQ(vol->get_vertex(4), geometry->get_vertex(8));
	EXPECT_EQ(vol->get_vertex(5), geometry->get_vertex(9));
	EXPECT_EQ(vol->get_vertex(6), geometry->get_vertex(10));
	EXPECT_EQ(vol->get_vertex(7), geometry->get_vertex(11));
}

TEST_F(retQSSParserTest, world_vertices)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/sphere/geom/sphere.vtk", ic);

	auto geometry = RETQSS()->geometry();

	EXPECT_EQ(geometry->num_vertices(), 642);

	auto world = geometry->world();

	EXPECT_EQ(world->num_vertices(), geometry->num_vertices());
}

TEST_F(retQSSParserTest, world_misc_test)
{
	retQSS::InitialConditionArray ic {{0, 0, -30, 10, -30, 13}};
	this->sim = retQSSTest::initialize("../../data/helix/geom/helix", ic);
	auto tracker = RETQSS()->tracker();
	auto geometry = RETQSS()->geometry();

	retQSS::Volume *vol = tracker->current_volume(0);
	EXPECT_EQ(vol, geometry->world());

	Ray_3 ray(Point_3(0,0,-30), Vector_3(10,-30,13));
	auto data = vol->crossed_face(ray);
	retQSS::PolyhedronFace *face = std::get<0>(data);
	Point_3 point = std::get<1>(data);

	EXPECT_NE(face, nullptr);
	EXPECT_EQ(face->subfaces().size(), 2);

	Point_3 expected(23.07692308, -69.23076923, 0);
	EXPECT_TRUE(retQSS::points_are_equal(point, expected));
}

TEST_F(retQSSParserTest, logical_faces_in_simple_geometry)
{
	retQSS::InitialConditionArray ic {{0, 0, 0, 269.5433, 0, 130.5458}};
	this->sim = retQSSTest::initialize("../../data/cube/geom/cube.vtk", ic);

	auto geometry = RETQSS()->geometry();
	auto world = geometry->world();

	// 20*11 + 4*4*12 + 3*5*12 (internal) +
	// 20*2 (upper/lower surface) +
	// 5*12*2 + 4*12*2 (rest of surface)
	EXPECT_EQ(geometry->num_logical_faces(), 848);

	for(auto face : geometry->logical_faces())
		EXPECT_EQ(face->_secondary_faces.size(), 1);

	for(auto face : world->faces())
	{
		auto logical_face = face->logical_face();

		EXPECT_NE(logical_face, nullptr);
		EXPECT_TRUE(logical_face->is_on_boundary());
	}

	for(auto vol : geometry->volumes())
	{
		if(!vol->is_on_boundary())
			for(auto face : vol->faces())
			{
				auto logical_face = face->logical_face();

				EXPECT_NE(logical_face, nullptr);
				EXPECT_FALSE(logical_face->is_on_boundary());
			}
	}
}

TEST_F(retQSSParserTest, logical_faces_in_complex_geometry)
{
	retQSS::InitialConditionArray ic {{7, 12, 2, 0, 0, -4}};
	this->sim = retQSSTest::initialize("../../data/box/geom/box.vtk", ic);

	auto geometry = RETQSS()->geometry();
	auto world = geometry->world();

	EXPECT_EQ(geometry->num_logical_faces(), 22);

	auto vol = geometry->get_polyhedral_volume(4);
	auto logical_face = vol->faces()[0]->logical_face();

	EXPECT_EQ(logical_face->_secondary_faces.size(), 4);

	for(VolumeID v : {1,2,3,5})
	{
		vol = geometry->get_polyhedral_volume(v);
		bool found = false;

		for(auto face : vol->faces())
		{
			if(face->logical_face() == logical_face)
				found = true;
			else
				EXPECT_EQ(face->logical_face()->_secondary_faces.size(), 1);
		}

		EXPECT_TRUE(found);
	}
}

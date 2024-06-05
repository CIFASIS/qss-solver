#include "retqss_interface.hh"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <random>
#include <regex>
#include <map>
#include <dlfcn.h>

#include "retqss_particle_tracker.hh"
#include "retqss_geometry.hh"
#include "retqss_utilities.hh"
#include "retqss_parser.hh"
#include "retqss_particle_neighborhood.hh"
#include "retqss_logical_face.hh"
#include "retqss_exceptions.hh"
#include "retqss_qss_solver_interface.hh"

#ifdef RETQSS_DEBUG
#include "retqss_debug.h"
#endif


retQSS::Interface *retQSS::Interface::interface = nullptr;

retQSS::Interface *retQSS::Interface::instance()
{
	if(retQSS::Interface::interface == nullptr)
		retQSS::Interface::interface = new retQSS::Interface();
	return retQSS::Interface::interface;
}

void retQSS::Interface::clear_instance()
{
	delete retQSS::Interface::interface;
	retQSS::Interface::interface = nullptr;
}

retQSS::Interface::Interface() :
		_tracker(nullptr),
		_crossing_algorithm(nullptr),
		geometry_created(false),
		particles_created(false),
		current_x(0),
		current_y(0),
		current_z(0)
{
	this->_geometry = new retQSS::Geometry();
	this->_neighborhood = new retQSS::VolumeNeighborhood(this->_geometry);
#ifdef RETQSS_STATS
	this->_stats = new retQSS::Stats;
#endif
}

retQSS::Interface::~Interface()
{
	delete this->_tracker;
	delete this->_geometry;
	delete this->_neighborhood;
	delete this->_crossing_algorithm;
#ifdef RETQSS_STATS
	delete this->_stats;
#endif
}

retQSS::InitialConditionArray*
retQSS::Interface::get_initial_conditions(
		int n_particles)
{
	auto ic_array = new retQSS::InitialConditionArray;

	for(
		Internal_ParticleID p = 0;
		p < static_cast<size_t>(n_particles);
		p++)
	{
		auto pos = this->particle_current_position(p);
		auto vel = this->particle_current_velocity(p);
		retQSS::InitialCondition ic;
		ic.x = pos.x(); ic.y = pos.y(); ic.z = pos.z();
		ic.vx = vel.x(); ic.vy = vel.y(); ic.vz = vel.z();
		ic.volumeID = 0;
		ic_array->push_back(ic);
	}

	return ic_array;
}

bool retQSS::Interface::geometry_set_up(const char *geom_filename)
{
	if(this->geometry_created)
		throw retQSS::Exception("Geometry already initialized!");

	retQSS::GeometryParser *parser = nullptr;

	parser = retQSS::GeometryParser::build_from(geom_filename);
	parser->parse(this->_geometry);
	delete parser;

	this->geometry_created = true;

	return this->geometry_created;
}

int geometry_grid_point_id(int x, int y, int z, int yCount, int zCount){
	return z + (zCount+1) * (y + (yCount+1) * x);
}

bool retQSS::Interface::geometry_grid_set_up(int xCount, int yCount, int zCount, double cellEdgeLength)
{
	auto geometry = this->_geometry;
	std::map<int, std::map<int, std::map<int, retQSS::PolyhedralVolume*>>> volumes;
    for (int x = 0; x < xCount; x++) {
      volumes.erase(x-2);
      for (int y = 0; y < yCount; y++) {
        for (int z = 0; z < zCount; z++) {
			std::stringstream name;
			name << "Voxel";
			retQSS::Polyhedron *poly = new retQSS::Polyhedron();
			for (int dz = 0; dz <= 1; dz++) {
				for (int dy = 0; dy <= 1; dy++) {
					for (int dx = 0; dx <= 1; dx++) {
						double px = x+dx, py = y+dy, pz = z+dz;
						poly->add_vertex(Point_3(px*cellEdgeLength, py*cellEdgeLength, pz*cellEdgeLength));
						name << "-" << geometry_grid_point_id(x+dx, y+dy, z+dz, yCount, zCount);
					}
				}
			}
			auto z0 = poly->add_face(VertexIndex(0),VertexIndex(1),VertexIndex(3),VertexIndex(2));
			auto x1 = poly->add_face(VertexIndex(5),VertexIndex(7),VertexIndex(3),VertexIndex(1));
			auto z1 = poly->add_face(VertexIndex(6),VertexIndex(7),VertexIndex(5),VertexIndex(4));
			auto x0 = poly->add_face(VertexIndex(4),VertexIndex(0),VertexIndex(2),VertexIndex(6));
			auto y1 = poly->add_face(VertexIndex(7),VertexIndex(6),VertexIndex(2),VertexIndex(3));
			auto y0 = poly->add_face(VertexIndex(1),VertexIndex(0),VertexIndex(4),VertexIndex(5));
			auto vol = geometry->add_volume(poly, name.str());
			volumes[x][y][z] = vol;
			if (z == 0) {
				geometry->add_boundary_face(vol->get_face(z0));
			} else {
				vol->check_neighbor(volumes[x][y][z-1]);
			}
			if (z==zCount-1) {
				geometry->add_boundary_face(vol->get_face(z1));
			}
			if (y==0) {
				geometry->add_boundary_face(vol->get_face(y0));
			} else {
				vol->check_neighbor(volumes[x][y-1][z]);
			}
			if (y==yCount-1) {
				geometry->add_boundary_face(vol->get_face(y1));
			}
			if (x==0) {
				geometry->add_boundary_face(vol->get_face(x0));
			} else {
				vol->check_neighbor(volumes[x-1][y][z]);
			}
			if (x==xCount-1) {
				geometry->add_boundary_face(vol->get_face(x1));
			}
        }
      }
    }
	geometry->close();
	this->geometry_created = true;
	return this->geometry_created;
}

void retQSS::Interface::create_particle_dependent_objects(
		int n_particles)
{
	auto sim_interface =
			retQSS::SimulatorInterface::create_for(this, n_particles);

	this->_tracker = new
			retQSS::ParticleTracker(
					sim_interface,
					this->_geometry);

	this->_crossing_algorithm = new
			retQSS::FaceCrossingAlgorithm(
					this->_tracker);
}

bool retQSS::Interface::particle_set_up(
		int n_particles,
		const char *ic_filename,
		const char *model_name)
{
	if(this->particles_created)
		throw retQSS::Exception("Particles already initialized!");

	if(!this->geometry_created)
		throw retQSS::Exception(
					"Geometry must be initialized before particles!");

	if(n_particles <= 0)
		throw retQSS::Exception("Invalid number of particles");

	this->create_particle_dependent_objects(n_particles);
	retQSS::InitialConditionArray *ic_array;

	std::string filename(ic_filename);

	if(filename.empty())
		ic_array = this->get_initial_conditions(n_particles);
	else
		ic_array = retQSS::read_initial_conditions(filename, n_particles);

	tracker()->create_particles(*ic_array, model_name);

	delete ic_array;
	this->particles_created = true;

	return this->particles_created;
}

bool retQSS::Interface::full_setup(
		int n_particles,
		const char *geom_filename,
		const char *ic_filename,
		const char *model_name)
{
	if(this->particles_created || this->geometry_created)
		throw retQSS::Exception("Geometry or particles already initialized!");

	this->geometry_set_up(geom_filename);
	this->particle_set_up(n_particles, ic_filename, model_name);

	return this->geometry_created && this->particles_created;
}

void retQSS::Interface::abort()
{
	std::cout << "Aborting retQSS simulation.\n";
	std::abort();
}

double retQSS::Interface::particle_next_crossing_time(Internal_ParticleID p)
{
	tracker()->check_ID(p);

	auto particle = tracker()->get_particle_unsafe(p);
	double initial_time = particle->initial_time();
	double current_time = tracker()->current_time_unsafe(p);
	double scheduled_exit_time = tracker()->next_exit_time_unsafe(p);
	double next_exit_time = scheduled_exit_time;

	// If current time is beyond the exit time previously calculated, we need
	// to update geometry data (e.g. current volume and its entry face) and
	// stop immediately returning this exit time to catch the event.
	if(current_time > initial_time && current_time >= scheduled_exit_time)
	{
		tracker()->update_state(p);
#ifdef RETQSS_STATS
		this->_stats->actual_crossings++;
#endif
	}
	else
	{
#ifdef RETQSS_STATS
		struct timespec t0, t1;
		RETQSS_GET_TIME(t0);
#endif
		auto trajectory = particle->trajectory();
		trajectory->update();

		// Compute future intersections only when:
		//  1. The underlying particle trajectory has changed (i.e., at least
		//  one the approximation polynomials was recently updated), or
		//  2. The particle has just entered a new volume (i.e., its exit time
		//  is set to infinity).
		// In any other case, we just return the precomputed exit time.
		if(
			scheduled_exit_time >= tracker()->infinity() ||
			trajectory->updated())
		{
			auto face_data = crossing_algorithm()->find_crossed_face(particle);
			tracker()->set_exit_state(p, face_data);
			next_exit_time = std::get<0>(face_data);
#ifdef RETQSS_STATS
			this->_stats->intersections_computed++;
#endif
		}
#ifdef RETQSS_STATS
		RETQSS_GET_TIME(t1);
		this->_stats->intersection_time += RETQSS_TIME_SECS(t0, t1);
#endif
	}
#ifdef RETQSS_STATS
		this->_stats->next_crossing_time_calls++;
#endif

	return next_exit_time;
}

int retQSS::Interface::geometry_count_volumes()
{
	return geometry()->num_polyhedral_volumes();
}

int retQSS::Interface::geometry_count_vertices()
{
	return geometry()->num_vertices();
}

int retQSS::Interface::geometry_count_faces()
{
	return geometry()->num_logical_faces();
}

VolumeID retQSS::Interface::geometry_random_volume_ID()
{
	int n = geometry()->num_polyhedral_volumes();
	return static_cast<VolumeID>(retQSS::random_int(1, n));
}

VolumeID retQSS::Interface::particle_current_volume_ID(Internal_ParticleID p)
{
	auto vol = tracker()->current_volume(p);
	assert(vol != nullptr);

	return vol->get_ID();
}

VolumeID retQSS::Interface::particle_previous_volume_ID(Internal_ParticleID p)
{
	auto vol = tracker()->previous_volume(p);
	assert(vol != nullptr);

	return vol->get_ID();
}

VolumeID retQSS::Interface::particle_next_volume_ID(Internal_ParticleID p)
{
	// To calculate next boundary crossing.
	this->particle_next_crossing_time(p);
	auto vol = tracker()->next_volume(p);
	assert(vol != nullptr);

	return vol->get_ID();
}

retQSS::Internal_LogicalFaceID
retQSS::Interface::particle_crossed_face_ID(Internal_ParticleID p)
{
	auto face = tracker()->exit_face(p);
	assert(face != nullptr);

	return face->logical_face()->get_ID();
}

int retQSS::Interface::volume_count_particles_inside(VolumeID v)
{
	auto vol = geometry()->get_volume(v);

	return vol->particles_inside().size();
}

retQSS::Internal_ParticleID
retQSS::Interface::volume_ID_of_particle_inside(
		VolumeID v,
		Internal_Index i)
{
	auto vol = geometry()->get_volume(v);
	auto particle_ids = vol->particles_inside();

	assert(i >= 0 && i < particle_ids.size());

	auto it = particle_ids.begin();
	for(Internal_Index j = 0; j < i; j++) it++;

	return *it;
}

void retQSS::Interface::volume_centroid(
		VolumeID v,
		int *rv, double *x, double *y, double *z)
{
	auto vol = geometry()->get_polyhedral_volume(v);
	auto centroid = vol->centroid();

	this->set_output_from(centroid,x,y,z);
}

retQSS::Internal_VertexID
retQSS::Interface::volume_vertex_ID(
		VolumeID v,
		Internal_Index i)
{
	auto vol = geometry()->get_volume(v);
	return vol->get_vertex(i)->get_ID();
}

retQSS::Internal_LogicalFaceID
retQSS::Interface::volume_face_ID(
		VolumeID v,
		Internal_Index i)
{
	auto vol = geometry()->get_volume(v);
	auto face = vol->get_face(i);
	assert(face->logical_face() != nullptr);
	return face->logical_face()->get_ID();
}

PropertyValue retQSS::Interface::volume_get_vertex_property(
		VolumeID v,
		Internal_Index i,
		PropertyTag tag)
{
	auto vol = geometry()->get_volume(v);
	return vol->get_vertex(i)->get_property(tag);
}

PropertyValue retQSS::Interface::volume_set_vertex_property(
		VolumeID v,
		Internal_Index i,
		PropertyTag tag,
		PropertyValue value)
{
	auto vol = geometry()->get_volume(v);

	vol->get_vertex(i)->set_property(tag, value);

	return value;
}

void retQSS::Interface::volume_vertex_coordinates(
		VolumeID v, Internal_Index i,
		int *rv, double *x, double *y, double *z)
{
	auto vol = geometry()->get_volume(v);
	auto point = vol->get_vertex(i)->point();

	this->set_output_from(point,x,y,z);
}

int retQSS::Interface::volume_count_vertices(VolumeID v)
{
	auto vol = geometry()->get_volume(v);
	return vol->num_vertices();
}

int retQSS::Interface::volume_count_faces(VolumeID v)
{
	auto vol = geometry()->get_volume(v);
	return vol->num_faces();
}

double retQSS::Interface::volume_capacity(VolumeID v)
{
	auto vol = geometry()->get_polyhedral_volume(v);
	return vol->capacity();
}

bool retQSS::Interface::volume_is_on_boundary(VolumeID v)
{
	auto vol = geometry()->get_polyhedral_volume(v);

	return vol->is_on_boundary();
}

bool retQSS::Interface::volume_has_property(
		VolumeID v,
		const char *prop)
{
	auto vol = geometry()->get_volume(v);

	return vol->has_property(prop);
}

PropertyValue retQSS::Interface::volume_get_property(
		VolumeID v,
		PropertyTag tag)
{
	auto vol = geometry()->get_volume(v);

	return vol->get_property(tag);
}

void retQSS::Interface::volume_get_property_vector(
		VolumeID v,
		PropertyTag tag,
		int *rv, double *x, double *y, double *z)
{
	auto vol = geometry()->get_volume(v);
	this->get_object_property_vector(vol,tag,x,y,z);
}

PropertyValue retQSS::Interface::volume_set_property(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value)
{
	auto vol = geometry()->get_volume(v);

	vol->set_property(tag, value);

	return value;
}

bool retQSS::Interface::volume_set_property_vector(
		VolumeID v,
		PropertyTag tag,
		const Vector_3 &values)
{
	auto vol = geometry()->get_volume(v);
	vol->set_property(tag, values);
	return true;
}

double retQSS::Interface::volume_distance_to_point(
		VolumeID v,
		const Point_3 &point)
{
	auto vol = geometry()->get_polyhedral_volume(v);
	return vol->distance_to(point);
}

void retQSS::Interface::volume_ray_intersection(
		VolumeID v, const Ray_3 &ray,
		int *rv, double *x, double *y, double *z)
{
	auto vol = geometry()->get_polyhedral_volume(v);

	auto face_and_point = vol->crossed_face(ray);
	auto face = std::get<0>(face_and_point);
	auto point = std::get<1>(face_and_point);

	if(face == nullptr)
		throw retQSS::Exception("Ray intersection failed");

	this->set_output_from(point,x,y,z);
}

VolumeID retQSS::Interface::volume_adjacent_volume_ID(
		VolumeID v,
		const Ray_3 &ray)
{
	auto vol = geometry()->get_volume(v);

	auto source = ray.source();
	auto dir = ray.direction();
	auto location = vol->locate_point(source);

	if(location == retQSS::POINT_OUTSIDE)
		throw retQSS::Exception("Ray source is not inside volume");
	else if(location == retQSS::POINT_ON_BOUNDARY)
	{
		auto eps = vol->is_polyhedral() ? 1e-5 : -1e-5;
		source += eps * dir.to_vector();
	}

	Ray_3 new_ray(source, dir);

	auto face_and_point = vol->crossed_face(new_ray);
	auto face = std::get<0>(face_and_point);
	auto point = std::get<1>(face_and_point);

	if(face == nullptr)
		return vol->get_ID();

	auto adjacent_vol = geometry()->adjacent_volume(vol, face, point);

	return adjacent_vol->get_ID();
}

VolumeID retQSS::Interface::volume_neighborhood_neighbor_ID(
		VolumeID v,
		Internal_Index i)
{
	auto vol = geometry()->get_volume(v);

	return neighborhood()->get_neighbor(vol, i)->get_ID();
}

int retQSS::Interface::volume_neighborhood_count_volumes(VolumeID v)
{
	auto vol = geometry()->get_volume(v);

	return neighborhood()->num_neighbors(vol);
}

PropertyValue retQSS::Interface::volume_neighborhood_get_neighbor_property(
		VolumeID v,
		Internal_Index i,
		PropertyTag tag)
{
	auto vol = geometry()->get_volume(v);
	auto neighbor = neighborhood()->get_neighbor(vol, i);

	return neighbor->get_property(tag);
}

PropertyValue retQSS::Interface::volume_neighborhood_set_property(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value)
{
	auto vol = geometry()->get_volume(v);
	auto &neighbors = neighborhood()->neighbors(vol);

	for(auto neighbor : neighbors)
	{
		auto volume = neighbor->neighbor_volume();
		volume->set_property(tag, value);
	}

	return value;
}

bool retQSS::Interface::volume_neighborhood_to_default()
{
	neighborhood()->switch_to_default();
	return true;
}

bool retQSS::Interface::volume_neighborhood_to_file(
		const std::string &filename)
{
	neighborhood()->switch_to_file(filename);
	return true;
}

bool retQSS::Interface::volume_neighborhood_to_radial(double radius)
{
	neighborhood()->switch_to_radial(radius);
	return true;
}

bool retQSS::Interface::volume_neighborhood_to_periodic_radial(double radius)
{
	neighborhood()->switch_to_periodic_radial(radius);
	return true;
}

bool retQSS::Interface::volume_neighborhood_to_vertex_sharing()
{
	neighborhood()->switch_to_vertex_sharing();
	return true;
}

bool retQSS::Interface::volume_neighborhood_precompute_all()
{
	// Neighbor count will trigger computation of the neighborhood.
	for(auto vol : geometry()->volumes())
		neighborhood()->num_neighbors(vol);
	return true;
}

int retQSS::Interface::particle_neighborhood_count_particles(
		Internal_ParticleID p)
{
	int size = 0;
	auto particle_neighborhood = this->get_particle_neighborhood(p);

	for(auto _ : *particle_neighborhood)
		size++;

	return size;
}

void retQSS::Interface::particle_neighborhood_average_position(
		Internal_ParticleID p,
		double *rv, double *x, double *y, double *z)
{
	Vector_3 avg_pos(0,0,0);
	int count = 0;

	auto particle_neighborhood = this->get_particle_neighborhood(p);

	for(auto neighbor : *particle_neighborhood)
	{
		auto neigh_pos = neighbor->neighbor_position();
		avg_pos += Vector_3(neigh_pos.x(), neigh_pos.y(), neigh_pos.z());
		count++;
	}

	avg_pos /= count;

	this->set_output_from(avg_pos,x,y,z);

	*rv = count;
}

void retQSS::Interface::particle_neighborhood_average_velocity(
		Internal_ParticleID p,
		double *rv, double *x, double *y, double *z)
{
	Vector_3 avg_vel(0,0,0);
	int count = 0;

	auto particle_neighborhood = this->get_particle_neighborhood(p);

	for(auto neighbor : *particle_neighborhood)
	{
		avg_vel += neighbor->neighbor_velocity();;
		count++;
	}

	avg_vel /= count;

	this->set_output_from(avg_vel,x,y,z);

	*rv = count;
}

void retQSS::Interface::particle_neighborhood_repulsive_direction(
		Internal_ParticleID p,
		double *rv, double *x, double *y, double *z)
{
	Vector_3 dir(0,0,0);
	int count = 0;

	auto pos = particle_current_position(p);

	auto particle_neighborhood = this->get_particle_neighborhood(p);

	for(auto neighbor : *particle_neighborhood)
	{
		Vector_3 v = pos - neighbor->neighbor_position();
		dir += v / retQSS::vector_norm(v);
		count++;
	}

	dir /= retQSS::vector_norm(dir);

	this->set_output_from(dir,x,y,z);

	*rv = count;
}

void retQSS::Interface::particle_neighborhood_for_each_particle(
		Internal_ParticleID p,
		const char *function,
		const std::vector<double> &args,
		double *rv, double *x, double *y, double *z)
{
	auto func_ptr = this->retrieve_function(function);

	Vector_3 accum(0,0,0);
	int count = 0;
	bool status;

	auto particle_neighborhood = this->get_particle_neighborhood(p);

	for(auto neighbor : *particle_neighborhood)
	{
		Vector_3 result(0,0,0);

		if(args.empty())
		{
			auto func = (retQSS::FuncWithoutArgs)func_ptr;
			status = func(neighbor, result);
		}
		else
		{
			auto func = (retQSS::FuncWithArgs)func_ptr;
			status = func(neighbor, args, result);
		}

		if(status)
		{
			accum += result;
			count++;
		}
	}

	this->set_output_from(accum,x,y,z);

	*rv = count;
}

void retQSS::Interface::particle_neighborhood_evaluate_1(
		Internal_ParticleID p,
		const char *function,
		double arg,
		double *rv, double *x, double *y, double *z)
{
	auto func_ptr = this->retrieve_function(function);
	auto func = (retQSS::NeighborhoodFuncWith1Arg)func_ptr;

	auto particle_neighborhood = this->get_particle_neighborhood(p);

	auto result = func(particle_neighborhood, arg);
	this->set_output_from(result,x,y,z);
}

void retQSS::Interface::particle_neighborhood_evaluate_2(
		Internal_ParticleID p,
		const char *function,
		double arg1, double arg2,
		double *rv, double *x, double *y, double *z)
{
	auto func_ptr = this->retrieve_function(function);
	auto func = (retQSS::NeighborhoodFuncWith2Args)func_ptr;

	auto particle_neighborhood = this->get_particle_neighborhood(p);

	auto result = func(particle_neighborhood, arg1, arg2);
	this->set_output_from(result,x,y,z);
}

void *retQSS::Interface::retrieve_function(const char *function)
{
	void *func_ptr;
	auto it = this->functions_called.find(function);

	if(it != this->functions_called.end())
		func_ptr = it->second;
	else
	{
		void *handle = dlopen(NULL, 0);
		func_ptr = dlsym(handle, function);

		if(func_ptr == nullptr)
			throw retQSS::FunctionNotFoundException(function);

		this->functions_called[function] = func_ptr;
	}

	return func_ptr;
}

void retQSS::Interface::particle_crossed_face_centroid(
		Internal_ParticleID p,
		int *rv, double *x, double *y, double *z)
{
	auto face = tracker()->exit_face(p);
	assert(face != nullptr);

	auto centroid = face->centroid();
	this->set_output_from(centroid,x,y,z);
}

Vector_3 retQSS::Interface::get_crossed_face_normal(
		Internal_ParticleID p)
{
	auto face = tracker()->exit_face(p);
	assert(face != nullptr);
	return face->normal();
}

void retQSS::Interface::particle_crossed_face_normal(
		Internal_ParticleID p,
		int *rv, double *x, double *y, double *z)
{
	auto normal = this->get_crossed_face_normal(p);
	this->set_output_from(normal,x,y,z);
}

VolumeID retQSS::Interface::face_primary_volume_ID(Internal_LogicalFaceID f)
{
	auto face = geometry()->get_logical_face(f);
	auto vol = face->primary_face()->volume();

	return vol->get_ID();
}

Bool retQSS::Interface::face_is_on_boundary(Internal_LogicalFaceID f)
{
	auto face = geometry()->get_logical_face(f);

	return face->is_on_boundary();
}

void retQSS::Interface::face_normal(
		Internal_LogicalFaceID f,
		int *rv, double *x, double *y, double *z)
{
	auto face = geometry()->get_logical_face(f);
	auto normal = face->normal();

	this->set_output_from(normal,x,y,z);
}

void retQSS::Interface::face_centroid(
		Internal_LogicalFaceID f,
		int *rv, double *x, double *y, double *z)
{
	auto face = geometry()->get_logical_face(f);
	auto centroid = face->centroid();

	this->set_output_from(centroid,x,y,z);
}

bool retQSS::Interface::face_is_normal_to(
		Internal_LogicalFaceID f,
		const Vector_3 &n)
{
	auto face = geometry()->get_logical_face(f);
	auto normal = face->normal();
	auto cross = CGAL::cross_product(n, normal);

	return retQSS::numbers_are_equal(cross.squared_length(), 0);
}

retQSS::Internal_LogicalFaceID
retQSS::Interface::face_opposite_face_ID(
		Internal_LogicalFaceID f)
{
	auto logical_face = geometry()->get_logical_face(f);
	auto face = logical_face->primary_face();

	auto face_and_point = geometry()->opposite_intersection(face);
	auto crossed_face = std::get<0>(face_and_point);

	return crossed_face->logical_face()->get_ID();
}

VolumeID retQSS::Interface::face_opposite_volume_ID(Internal_LogicalFaceID f)
{
	auto logical_face = geometry()->get_logical_face(f);
	auto volume = geometry()->opposite_volume(logical_face->primary_face());

	return volume->get_ID();
}

bool retQSS::Interface::face_has_property(
		Internal_LogicalFaceID f,
		PropertyTag tag)
{
	auto face = geometry()->get_logical_face(f);

	return face->has_property(tag);
}

PropertyValue retQSS::Interface::face_get_property(
		Internal_LogicalFaceID f,
		PropertyTag tag)
{
	auto face = geometry()->get_logical_face(f);

	return face->get_property(tag);
}

void retQSS::Interface::face_get_property_vector(
		Internal_LogicalFaceID f,
		PropertyTag tag,
		int *rv, double *x, double *y, double *z)
{
	auto face = geometry()->get_logical_face(f);
	this->get_object_property_vector(face,tag,x,y,z);
}

PropertyValue retQSS::Interface::face_set_property(
		Internal_LogicalFaceID f,
		PropertyTag tag,
		PropertyValue value)
{
	auto face = geometry()->get_logical_face(f);
	face->set_property(tag, value);

	return value;
}

bool retQSS::Interface::face_set_property_vector(
		Internal_LogicalFaceID f,
		PropertyTag tag,
		const Vector_3 &values)
{
	auto face = geometry()->get_logical_face(f);
	face->set_property(tag, values);
	return true;
}

bool retQSS::Interface::vertex_has_property(
		Internal_VertexID u,
		PropertyTag tag)
{
	auto vertex = geometry()->get_vertex(u);

	return vertex->has_property(tag);
}

PropertyValue retQSS::Interface::vertex_get_property(
		Internal_VertexID u,
		PropertyTag tag)
{
	auto vertex = geometry()->get_vertex(u);

	return vertex->get_property(tag);
}

void retQSS::Interface::vertex_get_property_vector(
		Internal_VertexID u,
		PropertyTag tag,
		int *rv, double *x, double *y, double *z)
{
	auto vertex = geometry()->get_vertex(u);
	this->get_object_property_vector(vertex,tag,x,y,z);
}

PropertyValue retQSS::Interface::vertex_set_property(
		Internal_VertexID u,
		PropertyTag tag,
		PropertyValue value)
{
	auto vertex = geometry()->get_vertex(u);
	vertex->set_property(tag, value);

	return value;
}

bool retQSS::Interface::vertex_set_property_vector(
		Internal_VertexID u,
		PropertyTag tag,
		const Vector_3 &values)
{
	auto vertex = geometry()->get_vertex(u);
	vertex->set_property(tag, values);
	return true;
}

void retQSS::Interface::vertex_coordinates(
		Internal_VertexID u,
		int *rv, double *x, double *y, double *z)
{
	auto vertex = geometry()->get_vertex(u);
	auto point = vertex->point();

	this->set_output_from(point,x,y,z);
}

bool retQSS::Interface::particle_has_property(
		Internal_ParticleID p,
		PropertyTag tag)
{
	auto particle = tracker()->get_particle(p);

	return particle->has_property(tag);
}

PropertyValue retQSS::Interface::particle_get_property(
		Internal_ParticleID p,
		PropertyTag tag)
{
	auto particle = tracker()->get_particle(p);

	return particle->get_property(tag);
}

void retQSS::Interface::particle_get_property_vector(
		Internal_ParticleID p,
		PropertyTag tag,
		int *rv, double *x, double *y, double *z)
{
	auto particle = tracker()->get_particle(p);
	this->get_object_property_vector(particle,tag,x,y,z);
}

PropertyValue retQSS::Interface::particle_set_property(
		Internal_ParticleID p,
		PropertyTag tag,
		PropertyValue value)
{
	auto particle = tracker()->get_particle(p);
	particle->set_property(tag, value);
	return value;
}

bool retQSS::Interface::particle_set_property_vector(
		Internal_ParticleID p,
		PropertyTag tag,
		const Vector_3 &values)
{
	auto particle = tracker()->get_particle(p);
	particle->set_property(tag, values);
	return true;
}

VolumeID retQSS::Interface::particle_relocate(
		Internal_ParticleID p,
		const Point_3 &position,
		const Vector_3 &velocity)
{
	auto vol = geometry()->locate(position, velocity);

	tracker()->relocate(p, vol);

	return vol->get_ID();
}

bool retQSS::Interface::particle_kill(Internal_ParticleID p)
{
	tracker()->kill(p);
	return true;
}

bool retQSS::Interface::particle_is_alive(Internal_ParticleID p)
{
	return tracker()->is_alive(p);
}

void retQSS::Interface::particle_initial_position(
		Internal_ParticleID p,
		int *rv, double *x, double *y, double *z)
{
	auto pos = tracker()->get_particle(p)->initial_position();
	this->set_output_from(pos,x,y,z);
}

void retQSS::Interface::particle_initial_velocity(
		Internal_ParticleID p,
		int *rv, double *vx, double *vy, double *vz)
{
	auto vel = tracker()->get_particle(p)->initial_velocity();
	this->set_output_from(vel,vx,vy,vz);
}

bool retQSS::Interface::particle_check_position(Internal_ParticleID p)
{
	bool inside;
	auto vol = tracker()->current_volume(p);

	auto pos = this->particle_current_position(p);

	inside = this->point_in_current_volume(p, pos.x(), pos.y(), pos.z());
	if(!inside)
		std::cout
			<< "retQSS warning: particle not inside current volume! (time: "
			<< tracker()->current_time(p)
			<< " - particle no.: "
			<< p
			<< " - volume: "
			<< vol->get_name()
			<< std::endl;

#ifdef RETQSS_DEBUG
	auto time = tracker()->current_time(p);
	tracker()->add_trajectory_point(p, pos, time);
#endif

	return inside;
}

bool retQSS::Interface::geometry_dump_surface(const char *filename)
{
	return geometry()->dump_volume(0, filename);
}

bool retQSS::Interface::geometry_dump_volume(VolumeID v, const char *filename)
{
	return geometry()->dump_volume(v, filename);
}

bool retQSS::Interface::geometry_dump_stats(const char *filename)
{
	return geometry()->dump_stats(filename);
}

double retQSS::Interface::particle_squared_distance_between(
		Internal_ParticleID p1,
		Internal_ParticleID p2)
{
	auto pos1 = this->particle_current_position(p1);
	auto pos2 = this->particle_current_position(p2);
	Vector_3 v(pos1, pos2);

	return v.squared_length();
}

double retQSS::Interface::particle_angle_between(
		Internal_ParticleID p1,
		Internal_ParticleID p2)
{
	auto pos1 = this->particle_current_position(p1);
	auto pos2 = this->particle_current_position(p2);
	auto v1 = this->particle_current_velocity(p1);

	Vector_3 v2(pos1, pos2);

	return retQSS::angle_between(v1, v2);
}

void retQSS::Interface::particle_reflective_bounce(
		Internal_ParticleID p,
		int *rv, double *vx, double *vy, double *vz,
		double speed)
{
	auto vel = tracker()->current_velocity(p);
	auto normal = this->get_crossed_face_normal(p);
	double dot = normal*vel;

	vel -= 2*dot*normal;

	if(speed > 0)
	{
		double norm = retQSS::vector_norm(vel);
		vel *= speed/norm;
	}

	this->set_output_from(vel,vx,vy,vz);
}

void retQSS::Interface::particle_opposite_position(
		Internal_ParticleID p,
		int *rv, double *x, double *y, double *z)
{
	auto face = tracker()->exit_face(p);
	assert(face != nullptr);
	auto pos = this->particle_current_position(p);

	auto face_and_point = geometry()->opposite_intersection(face, pos);
	auto int_point = std::get<1>(face_and_point);

	// Compute the new position of the particle, moving the intersection
	// point and respecting the distance from the original position to f.
	auto prev_vol = tracker()->previous_volume(p);
	auto dist = prev_vol->distance_to(pos);

	auto normal = face->normal();
	auto norm = retQSS::vector_norm(normal);
	if(prev_vol->is_polyhedral())
		normal *= -1;

	normal *= dist/norm;
	int_point += normal;

	this->set_output_from(int_point,x,y,z);
}

bool retQSS::Interface::particle_dump_initial_conditions(
		Internal_ParticleID p,
		const std::string &filename)
{
	tracker()->dump_initial_conditions(p, filename);
	return true;
}

bool retQSS::Interface::particle_dump_all_initial_conditions(
		const std::string &filename)
{
	tracker()->dump_initial_conditions(filename);
	return true;
}

bool retQSS::Interface::particle_dump_all_current_values(
		const std::string &filename,
		int index)
{
	std::stringstream ss;
	ss << index;
	auto new_filename = std::regex_replace(filename, std::regex("%d"), ss.str());
	tracker()->dump_current_values(new_filename);
	return true;
}

Vector_3 retQSS::Interface::particle_vector_to(
		Internal_ParticleID p,
		Internal_ParticleID q)
{
	auto p_pos = this->particle_current_position(p);
	auto q_pos = this->particle_current_position(q);

	return Vector_3(p_pos, q_pos);
}

void retQSS::Interface::volume_random_point(
		VolumeID v,
		int *rv, double *x, double *y, double *z)
{
	auto vol = geometry()->get_polyhedral_volume(v);
	auto point = vol->random_point();

	this->set_output_from(point,x,y,z);
}

void retQSS::Interface::vector_with_norm(
		double x, double y, double z,
		double norm,
		int *rv, double *vx, double *vy, double *vz)
{
	Vector_3 vector(x,y,z);
	auto current_norm = retQSS::vector_norm(vector);
	if(current_norm != 0)
		vector *= norm/current_norm;

	this->set_output_from(vector,vx,vy,vz);
}

void retQSS::Interface::random_vector(
		int *rv, double *vx, double *vy, double *vz)
{
	*vx = retQSS::random_double(-1,1);
	*vy = retQSS::random_double(-1,1);
	*vz = retQSS::random_double(-1,1);
}

void retQSS::Interface::random_vector_with_norm(
		double new_norm,
		int *rv, double *vx, double *vy, double *vz)
{
	this->random_vector(rv, vx, vy, vz);
	auto norm = retQSS::vector_norm(Vector_3(*vx, *vy, *vz));

	*vx *= new_norm/norm;
	*vy *= new_norm/norm;
	*vz *= new_norm/norm;
}

bool retQSS::Interface::point_in_current_volume(
		Internal_ParticleID p,
		double x, double y, double z)
{
	Point_3 position = Point_3(x, y, z);

	auto vol = tracker()->current_volume(p);
	auto location = vol->locate_point(position);

	return location != retQSS::PointLocation::POINT_OUTSIDE;
}

Point_3 retQSS::Interface::particle_current_position(Internal_ParticleID p)
{
	return tracker()->current_position(p);
}

Vector_3 retQSS::Interface::particle_current_velocity(Internal_ParticleID p)

{
	return tracker()->current_velocity(p);
}

bool retQSS::Interface::get_object_property_vector(
		retQSS::Object *obj,
		PropertyTag tag,
		double *x, double *y, double *z)
{
	auto values = obj->get_property_array(tag);
	assert(!values.empty());
	while(values.size() < 3)
		values.push_back(values[0]);
	this->set_output_from(values,x,y,z);
	return true;
}

// TODO: IMPORTANT - may not work in parallel simulations.
retQSS::ParticleNeighborhood*
retQSS::Interface::get_particle_neighborhood(Internal_ParticleID p)
{
	auto particle = tracker()->get_particle(p);

	auto particle_neighborhood = particle->neighborhood();
	particle_neighborhood->reset(neighborhood());

	return particle_neighborhood;
}

void retQSS::Interface::set_output_from(
		const Point_3 &point,
		double *x, double *y, double *z)
{
	*x = point.x();
	*y = point.y();
	*z = point.z();
}

void retQSS::Interface::set_output_from(
		const Vector_3 &vector,
		double *x, double *y, double *z)
{
	*x = vector.x();
	*y = vector.y();
	*z = vector.z();
}

void retQSS::Interface::set_output_from(
		const std::vector<double> &vector,
		double *x, double *y, double *z)
{
	*x = vector[0];
	*y = vector[1];
	*z = vector[2];
}

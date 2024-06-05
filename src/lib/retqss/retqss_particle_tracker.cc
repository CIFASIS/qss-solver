#include "retqss_particle_tracker.hh"
#include "retqss_geometry.hh"
#include "retqss_trajectory.hh"
#include "retqss_particle_neighborhood.hh"
#ifdef RETQSS_DEBUG
#include <iostream>
#include "retqss_debug.h"
#endif


retQSS::ParticleTracker::ParticleTracker(
		retQSS::SimulatorInterface *simulator_interface,
		retQSS::Geometry *geometry) :
	sim_interface(simulator_interface),
	current_id(0),
	geometry(geometry)
{
	size_t num_particles = simulator_interface->num_particles();
	this->particles.resize(num_particles);
}

retQSS::ParticleTracker::~ParticleTracker()
{
	for(auto pair : this->particles)
	{
#ifdef RETQSS_DEBUG
		auto &traversed_vols = pair.second->traversed_vols;
		auto &trajectory = pair.second->points;
		auto id = pair.first->get_ID();

		int index = 1;
		typename retQSS::TraversedVolumes::iterator it1, it1_end;
		typename retQSS::PointLists::iterator it2, it2_end;

		it1 = traversed_vols.begin();
		it1_end = traversed_vols.end();

		it2 = trajectory.begin();
		it2_end = trajectory.end();

		for(; it1 != it1_end && it2 != it2_end; it1++, it2++, index++)
			retQSS::trajectory_to_geomview(
					id,
					index,
					it1->second,
					*it2,
					"trajectories");

		this->save_trajectory_to_vtk(id, trajectory);

		for(auto list : trajectory)
			delete list;
#endif
		delete pair.first;
		delete pair.second;
	}

	// IMPORTANT: particle tracker takes ownership of this object.
	delete this->sim_interface;
}

retQSS::Particle *retQSS::ParticleTracker::new_particle(
		const retQSS::InitialCondition &ic)
{
	double time = this->sim_interface->get_initial_time();
	auto p = this->current_id++;

	auto state = new retQSS::ParticleState;
	auto particle = new retQSS::Particle(p, time, ic);
	this->new_trajectory(particle);
	this->new_neighborhood(particle);

	state->alive = true;

	state->current_vol = nullptr;
	state->entry_face = nullptr;
	state->entry_time = this->sim_interface->infinity();

	state->prev_vol = nullptr;
	state->prev_exit_face = nullptr;

	state->next_exit_time = this->sim_interface->infinity();
	state->next_exit_face = nullptr;

	this->particles[p] = {particle, state};

#ifdef RETQSS_DEBUG
	auto &point_lists = state->points;
	point_lists.push_back(new retQSS::PointList);

	this->add_trajectory_point(
			p,
			Point_3(ic.x, ic.y, ic.z),
			time);
#endif

	return particle;
}

void retQSS::ParticleTracker::new_trajectory(
		retQSS::Particle *particle) const
{
	auto p = particle->get_ID();
	auto trajectory = this->sim_interface->get_particle_trajectory(p);
	particle->set_trajectory(trajectory);
}

void retQSS::ParticleTracker::new_neighborhood(
		retQSS::Particle *particle) const
{
	auto neighborhood = new retQSS::ParticleNeighborhood(this, particle);
	particle->set_neighborhood(neighborhood);
}

void retQSS::ParticleTracker::create_particles(
		const retQSS::InitialConditionArray &initial_conditions,
		const std::string &model_name)
{
	for(size_t i = 0; i < initial_conditions.size(); i++)
	{
		auto particle = this->new_particle(initial_conditions[i]);
		auto p = particle->get_ID();

		auto position = particle->initial_position();
		this->sim_interface->set_position(p, position);

		auto velocity = particle->initial_velocity();
		this->sim_interface->set_velocity(p, velocity);

		auto volume = (initial_conditions[i].volumeID > 0)
		    ? this->geometry->get_volume(initial_conditions[i].volumeID)
		    : this->geometry->locate(position, velocity);
		this->relocate(p, volume);
	}

	this->sim_interface->particle_set_up(model_name);
}

void retQSS::ParticleTracker::update_state(Internal_ParticleID p)
{
	retQSS::Lock lock(this->mutex);

	auto state = this->get_state(p);

	if(state->next_exit_time < this->sim_interface->infinity())
	{
		retQSS::Volume
			*prev_vol = state->current_vol,
			*current_vol;

		retQSS::PolyhedronFace *entry_face;

		current_vol = this->next_volume(p, &entry_face);

		state->current_vol = current_vol;
		state->entry_face = entry_face;
		state->entry_time = state->next_exit_time;

		state->prev_vol = prev_vol;
		state->prev_exit_face = state->next_exit_face;
		state->prev_exit_point = state->next_exit_point;

		// Best guess is to assume that the particle will not leave the volume.
		// If not, its state will be updated accordingly in the future.
		state->next_exit_time = this->sim_interface->infinity();
		state->next_exit_face = nullptr;

		// Mark entering/exiting particle.
		prev_vol->particle_exiting(p);
		current_vol->particle_entering(p);

#ifdef RETQSS_DEBUG
		if(
			state->prev_exit_face != nullptr &&
			state->prev_exit_face->is_point_on_boundary(
					state->prev_exit_point))
		{
			std::cout
				<< "WARNING: intersection point on face boundary!\n";
			std::cout
				<< "Exit volume: "
				<< prev_vol->get_name()
				<< std::endl;
			std::cout
				<< "Intersection point: "
				<< state->prev_exit_point
				<< std::endl;
		}

		auto &traversed_vols = state->traversed_vols;
		auto &point_lists = state->points;

		this->add_trajectory_point(
				p,
				state->prev_exit_point,
				state->entry_time);

		traversed_vols.push_back({state->entry_time, current_vol});
		point_lists.push_back(new retQSS::PointList);
#endif
	}
}

retQSS::Volume *retQSS::ParticleTracker::next_volume(
		Internal_ParticleID p,
		retQSS::PolyhedronFace **entry_face_ptr) const
{
	auto state = this->get_state(p);

	return this->geometry->adjacent_volume(
			state->current_vol,
			state->next_exit_face,
			state->next_exit_point,
			entry_face_ptr);
}

void retQSS::ParticleTracker::set_exit_state(
		Internal_ParticleID p,
		retQSS::CrossedFaceData &face_data)
{
	auto state = this->get_state(p);

	state->next_exit_time = std::get<0>(face_data);
	state->next_exit_face = std::get<1>(face_data);
	state->next_exit_point = std::get<2>(face_data);
}

void retQSS::ParticleTracker::relocate(
		Internal_ParticleID p,
		retQSS::Volume *volume)
{
	retQSS::Lock lock(this->mutex);

	auto state = this->get_state(p);
	auto time = this->sim_interface->get_simulator_time(p);

	state->prev_vol = state->current_vol;
	state->prev_exit_face = nullptr;

	state->current_vol = volume;
	state->entry_face = nullptr;
	state->entry_time = time;

	state->next_exit_face = nullptr;
	state->next_exit_time = this->sim_interface->infinity();

	// Mark entering/exiting particle.
	if(state->prev_vol != nullptr)
		state->prev_vol->particle_exiting(p);
	state->current_vol->particle_entering(p);
}

void retQSS::ParticleTracker::dump_initial_conditions(
		Internal_ParticleID p,
		const std::string &filename) const
{
	auto particle = this->get_particle(p);

	std::ofstream file(filename);
	if(!file.good())
		throw retQSS::FileAccessException(filename);

	this->dump_initial_conditions_to(particle, file);
}

void retQSS::ParticleTracker::dump_initial_conditions(
		const std::string &filename) const
{
	std::ofstream file(filename);
	if(!file.good())
		throw retQSS::FileAccessException(filename);

	for(auto pair : this->particles)
	{
		auto particle = pair.first;
		this->dump_initial_conditions_to(particle, file);
	}
}

void retQSS::ParticleTracker::dump_current_values(
		const std::string &filename) const
{
	std::ofstream file(filename);
	if(!file.good())
		throw retQSS::FileAccessException(filename);

	file.precision(12);

	for(Internal_ParticleID p = 0; p < this->particles.size(); p++)
	{
		auto pos = this->current_position(p);
		auto vel = this->current_velocity(p);

		file << pos.x() << " " << pos.y() << " " << pos.z() << std::endl;
		file << vel.x() << " " << vel.y() << " " << vel.z() << std::endl;
	}

	file.close();
}

void retQSS::ParticleTracker::dump_initial_conditions_to(
		retQSS::Particle *particle,
		std::ostream &out) const
{
	auto pos = particle->initial_position();
	auto vel = particle->initial_velocity();

	out.precision(12);
	out
		<< pos.x() << " "
		<< pos.y() << " "
		<< pos.z() << " "
		<< vel.x() << " "
		<< vel.y() << " "
		<< vel.z() << std::endl;
}

#ifdef RETQSS_DEBUG
void retQSS::ParticleTracker::add_trajectory_point(
		Internal_ParticleID p,
		const Point_3 &point,
		double time)
{
	auto &point_lists = this->get_state(p)->points;
	if(point_lists.back())
		point_lists.back()->push_back({time,point});
}

void retQSS::ParticleTracker::save_trajectory_to_vtk(
		Internal_ParticleID p,
		const retQSS::PointLists &trajectory)
{
	std::stringstream filename, points;
	filename << "retQSS_trajectory-" << p << ".vtk";
	std::ofstream out(filename.str());
	int n_points = 0;

	for(auto list: trajectory)
		for(auto time_point: *list)
			{
				points << time_point.second << "\n";
				n_points++;
			}

	out << "# vtk DataFile Version 2.0\n";
	out << "retQSS trajectory #" << p << "\n";
	out << "ASCII\n";
	out << "DATASET POLYDATA\n";
	out << "POINTS " << n_points << " double\n";
	out << points.str() << "\n";

	out << "LINES 1 " << n_points+1 << "\n";
	out << n_points << "\n";
	for(int i = 0; i < n_points; i++)
		out << i << "\n";

	out.close();
}
#endif

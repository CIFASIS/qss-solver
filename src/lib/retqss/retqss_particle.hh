/**
 * \file retqss_particle.hh
 */

#ifndef _RETQSS_PARTICLE_H_
#define _RETQSS_PARTICLE_H_

#include "retqss_object.hh"
#include "retqss_types.hh"
#include "retqss_trajectory.hh"
#include "retqss_cgal_main_types.hh"


namespace retQSS
{

class ParticleTracker;
class ParticleNeighborhood;

/**
 * \brief Representation of a retQSS particle
 *
 * This class provides an implementation for particles, which are a special
 * kind of retQSS object bound the abstract concept of particle present in
 * Modelica models. There, a \em particle is just a group of six state
 * variables in an ODE system that models the trajectory they follow in a 3D
 * space. An instance of this class materializes this concept and allows the
 * modeler to actively interact with particles, assigning properties to them
 * and querying the system for several other actions.
 *
 * Note, however, that particle objects ignore their current position and
 * velocity, as these come from state variables managed by the simulation
 * engine. Instead, retQSS can issue backward queries to the engine in order
 * to retrieve these values. Likewise, geometrical information (e.g., current
 * particle volume, next volume exit time, etc.) is handled by the particle
 * tracker object (retQSS::ParticleTracker).
 */
class Particle : public retQSS::Object
{
	friend class retQSS::ParticleTracker;

private:
	double t0;  //!< Initial time (i.e., time of birth)
	Point_3 initial_pos; //!< Initial position
	Vector_3 initial_vel; //!< Initial velocity

	retQSS::Trajectory *trajectory_obj; //!< Trajectory object
	retQSS::ParticleNeighborhood *neighborhood_obj; //!< Neighborhood

	inline void set_trajectory(retQSS::Trajectory *trajectory)
	{
		this->trajectory_obj = trajectory;
	}

	inline void set_neighborhood(
			retQSS::ParticleNeighborhood *neighborhood)
	{
		this->neighborhood_obj = neighborhood;
	}

public:
	Particle(
			Internal_ParticleID p,
			double t0,
			const retQSS::InitialCondition &ic);

	virtual ~Particle();

	inline double initial_time() const
	{
		return this->t0;
	}

	inline const Point_3 &initial_position() const
	{
		return this->initial_pos;
	}

	inline const Vector_3 &initial_velocity() const
	{
		return this->initial_vel;
	}

	inline retQSS::Trajectory *trajectory() const
	{
		return this->trajectory_obj;
	}

	inline retQSS::ParticleNeighborhood *neighborhood() const
	{
		return this->neighborhood_obj;
	}
};

}

#endif

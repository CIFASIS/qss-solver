/**
 * \file retqss_simulator_interface.hh
 */

#ifndef _RETQSS_SIMULATOR_INTERFACE_H_
#define _RETQSS_SIMULATOR_INTERFACE_H_

#include "retqss_types.hh"
#include "retqss_cgal_main_types.hh"
#include "retqss_trajectory.hh"


namespace retQSS
{

class Interface;

/**
 * \brief Abstract interface to simulation engines
 *
 * This class defines an abstract API to access arbitrary simulation engines
 * from retQSS, so that e.g. particle-related state variables can be queried.
 *
 * Currently, the only concrete implementation of this class is the QSS Solver
 * interface (retQSS::QSSSolverInterface). Future implementations should
 * provide custom behavior for the following set of abstract methods that
 * conform the API to the underlying simulation engine. In particular,
 *  - retQSS::SimulatorInterface::crossing_tolerance()
 *   - To retrieve the tolerance error for boundary crossings of a particle.
 *  - retQSS::SimulatorInterface::get_simulator_time()
 *   - To get the current time of the simulator assigned to a particle.
 *  - retQSS::SimulatorInterface::get_initial_time()
 *   - To get the initial simulation time.
 *  - retQSS::SimulatorInterface::get_particle_trajectory()
 *   - To assemble a trajectory object with polynomial approximations of the
 *   particle positional state variables.
 *  - retQSS::SimulatorInterface::get_position_at()
 *  - retQSS::SimulatorInterface::get_velocity_at()
 *   - To get the position/velocity of a particle at a given time.
 *  - retQSS::SimulatorInterface::set_position()
 *  - retQSS::SimulatorInterface::set_velocity()
 *   - To set the position/velocity of a particle.
 *  - retQSS::SimulatorInterface::infinity()
 *   - To get the value accepted as infinity by the simulation engine.
 */
class SimulatorInterface
{
protected:
	size_t _num_particles; //!< Number of particles in the model

public:
	/**
	 * \brief Create concrete simulator interface
	 *
	 * Create a concrete simulator interface to access a specific simulation
	 * engine.
	 *
	 * \note This method should be modified to create proper interfaces to
	 * future simulation engines.
	 */
	static SimulatorInterface *create_for(
			retQSS::Interface *interface,
			int n_particles);

	SimulatorInterface(size_t num_particles) :
		_num_particles(num_particles) {};

	virtual ~SimulatorInterface() {};

	/**
	 * \brief Retrieve number of particles in the model
	 */
	inline size_t num_particles() const
	{
		return this->_num_particles;
	}

	/**
	 * \brief Custom actions to perform while initializing particles
	 *
	 * Simulator-dependent behavior to be implemented on demand. Default action
	 * is doing nothing.
	 */
	virtual void particle_set_up(const std::string &model_name) {};

	Point_3 current_position(Internal_ParticleID p);

	Vector_3 current_velocity(Internal_ParticleID p);

	/**
	 * \brief Retrieve order of numerical method
	 */
	virtual int get_method_order() = 0;

	/**
	 * \brief Boundary crossing tolerance
	 *
	 * Backward query to the simulation engine to retrieve the desired
	 * tolerance for boundary crossings of particle \f$p\f$.
	 */
	virtual double crossing_tolerance(Internal_ParticleID p) = 0;

	/**
	 * \brief Retrieve current simulator time
	 *
	 * Backward query to the simulation engine to retrieve the current time of
	 * the simulator assigned to particle \f$p\f$.
	 */
	virtual double get_simulator_time(Internal_ParticleID p) = 0;

	/**
	 * \brief Retrieve initial simulation time
	 *
	 * Backward query to the simulation engine to retrieve the initial
	 * simulation time.
	 */
	virtual double get_initial_time() = 0;

	/**
	 * \brief Assemble particle trajectory
	 *
	 * Assemble particle \f$p\f$ trajectory by using the approximation
	 * polynomials provided by the simulation engine.
	 */
	virtual retQSS::Trajectory *get_particle_trajectory(
			Internal_ParticleID p) = 0;

	/**
	 * \brief Retrieve particle position at time
	 *
	 * Backward query to the simulation engine to retrieve particle \f$p\f$
	 * position at time \f$t\f$.
	 */
	virtual void get_position_at(
			Internal_ParticleID p,
			double t,
			double *x, double *y, double *z) = 0;
	/**
	 * \brief Retrieve particle velocity at time
	 *
	 * Backward query to the simulation engine to retrieve particle \f$p\f$
	 * velocity at time \f$t\f$.
	 */
	virtual void get_velocity_at(
			Internal_ParticleID p,
			double t,
			double *vx, double *vy, double *vz) = 0;

	/**
	 * \brief Set particle position
	 *
	 * Backward query to the simulation engine to set particle \f$p\f$
	 * position.
	 *
	 * \note Should not be used for safety purposes (in fact, it is only called
	 * during particle initialization in case the initial conditions are read
	 * from a file).
	 */
	virtual void set_position(
			Internal_ParticleID p,
			const Point_3 &position) = 0;

	/**
	 * \brief Set particle velocity
	 *
	 * Backward query to the simulation engine to set particle \f$p\f$ velocity.
	 *
	 * \note Should not be used for safety purposes (in fact, it is only called
	 * during particle initialization in case the initial conditions are read
	 * from a file).
	 */
	virtual void set_velocity(
			Internal_ParticleID p,
			const Vector_3 &velocity) = 0;

	/**
	 * \brief Retrieve infinity value used by the simulation engine
	 *
	 * Backward query to retrieve the infinity value used by the simulation
	 * engine.
	 */
	virtual double infinity() = 0;
};

}

#endif

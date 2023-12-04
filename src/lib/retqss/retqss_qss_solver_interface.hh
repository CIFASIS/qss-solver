/**
 * \file retqss_qss_solver_interface.hh
 */

#ifndef _RETQSS_QSS_SOLVER_INTERFACE_H_
#define _RETQSS_QSS_SOLVER_INTERFACE_H_

#include <vector>

#include "retqss_simulator_interface.hh"

extern "C"
{
#define bool QSS_BOOL
#include "qss/qss_simulator.h"
#include "qss/qss_commands.h"
#undef bool
}

#define RETQSS_X 0 //<! Index of x state variable in \f$\uMod\f$ models.
#define RETQSS_Y 1 //<! Index of y state variable in \f$\uMod\f$ models.
#define RETQSS_Z 2 //<! Index of z state variable in \f$\uMod\f$ models.
#define RETQSS_VX 3 //<! Index of vx state variable in \f$\uMod\f$ models.
#define RETQSS_VY 4 //<! Index of vy state variable in \f$\uMod\f$ models.
#define RETQSS_VZ 5 //<! Index of vz state variable in \f$\uMod\f$ models.

/**
 * Index into the x state variable of particle \f$p\f$.
 */
#define RETQSS_X_IDX_N(p_idx, n)	(RETQSS_X*n + p_idx)
#define RETQSS_Y_IDX_N(p_idx, n)	(RETQSS_Y*n + p_idx)
#define RETQSS_Z_IDX_N(p_idx, n)	(RETQSS_Z*n + p_idx)
#define RETQSS_VX_IDX_N(p_idx, n)	(RETQSS_VX*n + p_idx)
#define RETQSS_VY_IDX_N(p_idx, n)	(RETQSS_VY*n + p_idx)
#define RETQSS_VZ_IDX_N(p_idx, n)	(RETQSS_VZ*n + p_idx)


namespace retQSS
{

/**
 * \brief Interface to QSS Solver
 *
 * This class implements the simulator interface API to access QSS Solver
 * primitives and data structures from retQSS. \f$\uMod\f$ models in QSS Solver
 * are expected to comply with the following requirements in order to use
 * retQSS:
 *  - Particle state variables for position and velocity should be declared
 *  first, starting with position in the \f$x\f$, \f$y\f$ and \f$z\f$ axes,
 *  and following with velocity in the same order.
 *  - If parallel simulations are used,
 *    -# Manual partitioning should be chosen as partition strategy. retQSS
 *    will automatically export a partition file where particles are
 *    uniformly distributed over the chosen amount of LPs.
 *    -# Event handlers for boundary crossings of particles should be
 *    declared first, before any other event handler.
 */
class QSSSolverInterface : public retQSS::SimulatorInterface
{
private:
	QSS_simulator *simulators; //!< Global array declared in QSS Solver which
	                            //!< is used to retrieve the values of particle
	                            //!< state variables.

	bool parallel;        //!< Boolean flag to indicate if parallel mode is
	                      //!< enabled.
	std::vector<Internal_ParticleID> lps; //!< Array that maps a particle ID to
	                                      //!< a LP (for parallel simulations).

	inline QSS_simulator get_ith_simulator(size_t i)
	{
		return this->simulators[i];
	}

	inline QSS_simulator get_root_simulator()
	{
		return this->get_ith_simulator(0);
	}

	inline QSS_simulator get_particle_simulator(ParticleID p)
	{
		QSS_simulator sim = this->get_root_simulator();
		if(this->parallel)
			sim = this->get_ith_simulator(1+this->lps[p]);
		return sim;
	}

	void get_simulator_values(
			QSS_simulator,
			double,
			int, int, int,
			double*, double*, double*);

	/**
	 * \brief Export partition file for parallel simulations
	 *
	 * Export manual partition for parallel simulations, where particle state
	 * variables and event handlers are uniformly mapped to the available
	 * number of LPs.
	 */
	void set_particle_partition(const std::string &model_name);

public:
	/**
	 * \brief Interface constructor
	 *
	 * After calling the base class constructor, the global variable declared
	 * by QSS Solver with the simulator data structures is copied into the
	 * attribute retQSS::SolverInterface::simulators.
	 */
	QSSSolverInterface(size_t num_particles);

	virtual ~QSSSolverInterface() {};

	/**
	 * \brief Custom actions to perform while initializing particles
	 *
	 * In case parallel mode is enabled,
	 *  - It is verified that manual partitioning is chosen as partition
	 *  strategy.
	 *  - Manual partition is created and exported to a file.
	 */
	virtual void particle_set_up(const std::string &model_name);

	/**
	 * \brief Retrieve order of numerical method
	 */
	virtual int get_method_order();

	/**
	 * \brief Boundary crossing tolerance
	 *
	 * Backward query to QSS Solver to retrieve the desired tolerance for
	 * boundary crossings of particle \f$p\f$. It is set to the \f$\dQMin\f$
	 * value chosen for the \f$\uMod\f$ model.
	 */
	virtual double crossing_tolerance(Internal_ParticleID p);

	/**
	 * \brief Retrieve current simulator time
	 *
	 * Backward query to QSS Solver to retrieve the current time of the
	 * simulator assigned to particle \f$p\f$.
	 */
	virtual double get_simulator_time(Internal_ParticleID p);

	/**
	 * \brief Retrieve initial simulation time
	 *
	 * Backward query to QSS Solver to retrieve the initial simulation time.
	 */
	virtual double get_initial_time();

	/**
	 * \brief Assemble particle trajectory
	 *
	 * Assemble particle \f$p\f$ trajectory by using the approximation
	 * polynomials provided by QSS Solver.
	 */
	virtual retQSS::Trajectory *get_particle_trajectory(
			Internal_ParticleID p);

	/**
	 * \brief Retrieve particle position at time
	 *
	 * Backward query to QSS Solver to retrieve particle \f$p\f$ position
	 * at time \f$t\f$.
	 */
	virtual void get_position_at(
			Internal_ParticleID p,
			double t,
			double *x, double *y, double *z);

	/**
	 * \brief Retrieve particle velocity at time
	 *
	 * Backward query to QSS Solver to retrieve particle \f$p\f$ velocity
	 * at time \f$t\f$.
	 */
	virtual void get_velocity_at(
			Internal_ParticleID p,
			double t,
			double *vx, double *vy, double *vz);

	/**
	 * \brief Set particle position
	 *
	 * Backward query to QSS Solver to set particle \f$p\f$ position.
	 *
	 * \note Should not be used for safety purposes (in fact, it is only called
	 * during particle initialization in case the initial conditions are read
	 * from a file).
	 */
	virtual void set_position(
			Internal_ParticleID p,
			const Point_3 &position);

	/**
	 * \brief Set particle velocity
	 *
	 * Backward query to QSS Solver to set particle \f$p\f$ velocity.
	 *
	 * \note Should not be used for safety purposes (in fact, it is only called
	 * during particle initialization in case the initial conditions are read
	 * from a file).
	 */
	virtual void set_velocity(
			Internal_ParticleID p,
			const Vector_3 &velocity);

	/**
	 * \brief Retrieve infinity value used by the simulation engine
	 *
	 * Backward query to QSS Solver to retrieve the infinity value used by the
	 * simulation engine.
	 */
	virtual double infinity();
};

}

#endif

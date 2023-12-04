/**
 * \file retqss_trajectory.hh
 */

#ifndef _RETQSS_TRAJECTORY_H_
#define _RETQSS_TRAJECTORY_H_

#include "retqss_types.hh"
#include "retqss_cgal_main_types.hh"
#include "retqss_utilities.hh"


#define _TRAJECTORY_EVAL_X(t)\
		(retQSS::evaluate_poly(0, t, this->px, this->_order))

#define _TRAJECTORY_EVAL_Y(t)\
		(retQSS::evaluate_poly(0, t, this->py, this->_order))

#define _TRAJECTORY_EVAL_Z(t)\
		(retQSS::evaluate_poly(0, t, this->pz, this->_order))


namespace retQSS
{

/**
 * \brief Representation of particle trajectories
 *
 * This class provides an implementation of particle trajectories as
 * functions of time by wrapping their polynomial approximations computed by
 * the underlying simulation engine. Access to the positional state variables
 * and their approximations should be supplied by a proper implementation of
 * the simulator API (in particular, by method
 * retQSS::SimulatorInterface::get_particle_trajectory()).
 */
class Trajectory
{
protected:
	double *x; //!< Polynomial approximation of x coordinate as a function of
	           //!< time
	double *y; //!< Polynomial approximation of y coordinate as a function of
	           //!< time
	double *z; //!< Polynomial approximation of z coordinate as a function of
	           //!< time

	double *t0; //!< Pointer to simulator time (in the simulation engine)
	double *tx; //!< Pointer to last time of change of x polynomial (in the
	            //!< simulation engine)
	double *ty; //!< Pointer to last time of change of y polynomial (in the
	            //!< simulation engine)
	double *tz; //!< Pointer to last time of change of z polynomial (in the
	            //!< simulation engine)

	double last_tx; //!< Last time of change of x polynomial
	double last_ty; //!< Last time of change of y polynomial
	double last_tz; //!< Last time of change of z polynomial

	double current_t0; //!< Current start time of the trajectory

	double _x0_coeff;
	double _y0_coeff;
	double _z0_coeff;

	double _x1_coeff;
	double _y1_coeff;
	double _z1_coeff;

	double _x2_coeff;
	double _y2_coeff;
	double _z2_coeff;

	double _x3_coeff;
	double _y3_coeff;
	double _z3_coeff;

	bool was_updated; //!< Whether this trajectory has been updated by the last
	                  //!< udpate call.

public:
	static Trajectory *for_order(
			int order,
			double *px, double *py, double *pz,
			double *tx, double *ty, double *tz,
			double *t0);

	/**
	 * \brief Constructor
	 */
	Trajectory(
			double *px, double *py, double *pz,
			double *tx, double *ty, double *tz,
			double *t0);

	virtual ~Trajectory();

	/**
	 * \brief Update trajectory if polynomials have changed
	 *
	 * Update trajectory components whose approximation polynomials have been
	 * recently changed by the simulation engine. Should be explicitly called
	 * every time the trajectory is about to be used.
	 */
	virtual bool update() = 0;

	/**
	 * \brief Evaluate trajectory
	 *
	 * Evaluate trajectory by computing the approximated point at the given time.
	 */
	Point_3 operator()(double time) const;

	/**
	 * \brief Evaluate trajectory
	 *
	 * Evaluate trajectory by computing the approximated point at the given time.
	 */
	virtual void at(
			double time,
			double *x,
			double *y,
			double *z) const = 0;

	/**
	 * \brief Retrieve degree of the underlying polynomials
	 */
	virtual int order() const = 0;

	/**
	 * \brief Retrieve start time, \f$t_0\f$
	 */
	inline double start_time() const
	{
		return this->current_t0;
	}

	/**
	 * \brief Check whether time is the domain of the trajectory
	 */
	inline bool in_domain(double t) const
	{
		return t >= this->start_time();
	}

	inline double x0_coeff() const
	{
		return this->_x0_coeff;
	}

	inline double y0_coeff() const
	{
		return this->_y0_coeff;
	}

	inline double z0_coeff() const
	{
		return this->_z0_coeff;
	}

	inline double x1_coeff() const
	{
		return this->_x1_coeff;
	}

	inline double y1_coeff() const
	{
		return this->_y1_coeff;
	}

	inline double z1_coeff() const
	{
		return this->_z1_coeff;
	}

	inline double x2_coeff() const
	{
		return this->_x2_coeff;
	}

	inline double y2_coeff() const
	{
		return this->_y2_coeff;
	}

	inline double z2_coeff() const
	{
		return this->_z2_coeff;
	}

	inline double x3_coeff() const
	{
		return this->_x3_coeff;
	}

	inline double y3_coeff() const
	{
		return this->_y3_coeff;
	}

	inline double z3_coeff() const
	{
		return this->_z3_coeff;
	}

	/**
	 * \brief Check whether the trajectory was recently updated
	 *
	 * Check whether at least one of the underlying approximation polynomials
	 * was recently updated by the simulation engine.
	 */
	inline bool updated() const
	{
		return this->was_updated;
	}
};

/**
 * \brief Trajectories approximated by polynomials of degree 2
 */
class Order2Trajectory : public retQSS::Trajectory
{
private:
	/**
	 * \brief Evaluate degree 2 polynomial
	 *
	 * Evaluate polynomial \f$p\f$ at point \f$dt\f$ using Horner's method.
	 */
	inline double evaluate_poly(double dt, double *p) const
	{
		return p[0] + dt * (p[1] + dt * p[2]);
	}

public:
	/**
	 * \brief Constructor
	 */
	Order2Trajectory(
			double *px, double *py, double *pz,
			double *tx, double *ty, double *tz,
			double *t0);

	/**
	 * \brief Update trajectory if polynomials have changed
	 *
	 * Update trajectory components whose approximation polynomials have been
	 * recently changed by the simulation engine. Should be explicitly called
	 * every time the trajectory is about to be used.
	 */
	virtual bool update();

	/**
	 * \brief Evaluate trajectory
	 *
	 * Evaluate trajectory by computing the approximated point at the given time.
	 */
	virtual void at(
			double time,
			double *x,
			double *y,
			double *z) const;

	/**
	 * \brief Retrieve degree of the underlying polynomials
	 */
	virtual int order() const;
};

/**
 * \brief Trajectories approximated by polynomials of degree 3
 */
class Order3Trajectory : public retQSS::Trajectory
{
private:
	/**
	 * \brief Evaluate degree 3 polynomial
	 *
	 * Evaluate polynomial \f$p\f$ at point \f$dt\f$ using Horner's method.
	 */
	inline double evaluate_poly(double dt, double *p) const
	{
		return p[0] + dt * (p[1] + dt * (p[2] + dt * p[3]));
	}

public:
	/**
	 * \brief Constructor
	 */
	Order3Trajectory(
			double *px, double *py, double *pz,
			double *tx, double *ty, double *tz,
			double *t0);

	/**
	 * \brief Update trajectory if polynomials have changed
	 *
	 * Update trajectory components whose approximation polynomials have been
	 * recently changed by the simulation engine. Should be explicitly called
	 * every time the trajectory is about to be used.
	 */
	virtual bool update();

	/**
	 * \brief Evaluate trajectory
	 *
	 * Evaluate trajectory by computing the approximated point at the given time.
	 */
	virtual void at(
			double time,
			double *x,
			double *y,
			double *z) const;

	/**
	 * \brief Retrieve degree of the underlying polynomials
	 */
	virtual int order() const;
};

}

#endif

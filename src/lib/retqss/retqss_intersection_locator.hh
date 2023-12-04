/**
 * \file retqss_intersection_locator.hh
 * \brief Detection of intersections between trajectories and faces
 */

#ifndef _RETQSS_INTERSECTION_LOCATOR_H_
#define _RETQSS_INTERSECTION_LOCATOR_H_

#include "retqss_cgal_main_types.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_trajectory.hh"


namespace retQSS
{

/**
 * \brief Output data of intersection queries
 *
 * An instance of this structure is returned as a result of a call to the
 * intersection locator object. It contains the time of the upcoming
 * intersection and the 3D point where the trajectory meets the face.
 */
class IntersectionData
{
private:
	double t;
	double x, y, z;
	bool valid;

public:
	IntersectionData() : valid(false) {};

	inline void set(double time, double x, double y, double z)
	{
		this->t = time;
		this->x = x;
		this->y = y;
		this->z = z;
		this->valid = true;
	}

	inline void invalidate()
	{
		this->valid = false;
	}

	inline double time() const
	{
		return this->t;
	}

	inline Point_3 point() const
	{
		assert(this->is_valid());
		return Point_3(this->x, this->y, this->z);
	};

	inline bool is_valid() const
	{
		return this->valid;
	}
};


/**
 * \brief Implementation of the algorithm to detect intersections
 *
 * This class encapsulates the algorithm to detect intersections between
 * particle trajectories (retQSS::Trajectory) and polyhedron faces
 * (retQSS::PolyhedronFace). Each of the three components of a trajectory is
 * approximated by a polynomial, and thus the underlying algorithm basically
 * solves a polynomial equation involving also the equation of the plane
 * spanned by the face.
 */
class IntersectionLocator
{
protected:
	// TODO: IMPORTANT - will not work in parallel mode!
	// Either lock access to intersection computation or add a pool of
	// available intersection objects.
	retQSS::IntersectionData intersection;

	void evaluate_roots(
			const retQSS::Trajectory &trajectory,
			const retQSS::PolyhedronFace &face,
			double *roots,
			int n_roots);

public:
	static IntersectionLocator* for_order(int order);

	IntersectionLocator() {};
	virtual ~IntersectionLocator() {};

	/**
	 * \brief Detect the first upcoming intersection of a trajectory and a face
	 *
	 * Detect the first upcoming intersection of the given particle trajectory
	 * and the given polyhedron face. The algorithm follows these steps:
	 *  - The first key concept is that the underlying positional state
	 *  variables that model the trajectory are approximated by polynomials.
	 *  Thus, let \f$\tilde{\PartPos{p}}(t) = (p_x(t), p_y(t), p_z(t))\f$
	 *  be the polynomial approximation of the given particle trajectory,
	 *  \f$\PartPos{p}(t)\f$.
	 *  - Let \f$\vec{n} = (n_x, n_y, n_z)\f$ be the normal vector of the given
	 *  face. Then, the equation of the underlying plane is given by
	 *  \f[\vec{n} \cdot \left(\vec{v} - \vec{v_0}\right) = 0 \f]
	 *  where \f$\vec{v}\f$ is the position vector of a target point and
	 *  \f$\vec{v_0}\f$ is the position vector of a known point in the plane.
	 *    -# We instanciate \f$\vec{v_0}\f$ using the origin point in the face,
	 *    which is its first vertex.
	 *    -# Thus, the dot product \f$\vec{n} \cdot \vec{v_0}\f$ is already
	 *    precomputed in an attribute of the face object (see
	 *    retQSS::PolyhedronFace).
	 *  - The target vector \f$\vec{v}\f$ is constructed using
	 * \f$\tilde{\PartPos{p}}(t)\f$.
	 *    -# If e.g. we use a second-order approximation (i.e., grade 2
	 *    polynomials), we have that
	 *    \f[p_x(t) = a_0 + a_1 (t-t_k) + a_2 (t-t_k)^2 \f]
	 *    with \f$t_k\f$ the time at which this polynomial was last updated.
	 *    Then, the first term of \f$\vec{n} \cdot \vec{v}\f$ is given by
	 *    \f[n_x a_0 + n_x a_1 (t-t_k) + n_x a_2 (t-t_k)^2 \f]
	 *  - After including all three positional variables,
	 * distributing and grouping, we get a polynomial equation in \f$t\f$.
	 *  - The roots of this equation are computed following the well-known
	 * mathematical formulas for polynomial root finding (see
	 * retQSS::poly_roots()).
	 *  - For each root \f$r\f$ (sorted in increasing order) we compute the
	 *  candidate point \f$P = \tilde{\PartPos{p}}(t_k + r)\f$.
	 *  - We finally test if \f$P\f$ is inside our target face
	 * (see retQSS::PolyhedronFace::is_point_inside()) and return immediately
	 * if this test succeeds.
	 *  - If no root satisfies this, then there is no intersection, which is
	 * signaled by a null pointer as return value.
	 */
	virtual const IntersectionData &intersection_of(
			const retQSS::Trajectory &trajectory,
			const retQSS::PolyhedronFace &face) = 0;
};

class Order2IntersectionLocator : public retQSS::IntersectionLocator
{
public:
	virtual const IntersectionData &intersection_of(
			const retQSS::Trajectory &trajectory,
			const retQSS::PolyhedronFace &face);

};

class Order3IntersectionLocator : public retQSS::IntersectionLocator
{
public:
	virtual const IntersectionData &intersection_of(
			const retQSS::Trajectory &trajectory,
			const retQSS::PolyhedronFace &face);

};

}

#endif

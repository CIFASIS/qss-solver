/**
 * \file retqss_utilities.hh
 * \brief Assorted functions & utilities
 */

#ifndef _RETQSS_UTILITIES_H_
#define _RETQSS_UTILITIES_H_

#include <string>

#include "retqss_types.hh"
#include "retqss_cgal_main_types.hh"

/**
 * Tolerance for equality comparisons (points & numbers)
 */
#define RETQSS_EQ_TOLERANCE 5e-8

/**
 * Convert public object ID or index (i.e., model/user visibility) to internal
 * (i.e., developer visibility)
 */
#define RETQSS_TO_INTERNAL(id) (id-1)

/**
 * Convert internal object ID or index (i.e., developer visibility) to public
 * (i.e., model/user visibility)
 */
#define RETQSS_TO_PUBLIC(id)   (id+1)


namespace retQSS
{

/**
 * Hash implementation for CGAL points so that they can be used as keys of
 * C++ unordered containers (e.g., <tt>unordered_map</tt>)
 */
struct PointHash : public std::unary_function<Point_3, std::size_t>
{
	std::size_t operator()(const Point_3 &point) const
	{
		return
			std::hash<double>()(point.x()) ^
			std::hash<double>()(point.y()) ^
			std::hash<double>()(point.z());
	}
};

/**
 * \brief Read particle initial conditions from file
 *
 * Read and extract from the given file initial position and velocity for every
 * particle in the simulation. For each particle, six double-precision floating
 * point values are expected, corresponding to the 3D position and the velocity
 * vector, respectively.
 */
retQSS::InitialConditionArray *read_initial_conditions(
		const std::string &filename,
		int num_particles);

/**
 * \brief Compute roots of a polynomial
 *
 * Compute all real roots of a polynomial of degree 2 or 3.
 *
 * \return Array <tt>roots</tt> containing exactly <tt>n_roots</tt> sorted
 * entries, corresponding to the real roots of <tt>poly</tt>.
 */
void poly_roots(
		double *poly,
		double *roots,
		int *n_roots) __attribute__((hot));

/**
 * \brief Compute roots of a qudratic polynomial
 *
 * Compute all real roots of a quadratic polynomial.
 *
 * \sa retQSS::poly_roots
 */
void poly_roots_sq(
		double *coeff,
		double *roots,
		int *n_roots) __attribute__((hot));

/**
 * \brief Compute angle between two 3D vectors
 *
 * Compute angle in radians between vectors \f$\vec{u}\f$ and \f$\vec{v}\f$.
 *
 * \return Angle \f$\alpha \in [0, \pi]\f$
 */
double angle_between(
		const Vector_3 &u,
		const Vector_3 &v);

/**
 * \brief Compute norm of a 3D vector
 *
 * Compute norm of vector \f$\vec{v} = (v_x, v_y, v_z)\f$,
 *
 * \[
 *  \sqrt{v_x^2 + v_y^2 + v_z^2}
 * \]
 */
inline double vector_norm(const Vector_3 &v)
{
	return std::sqrt(v.squared_length());
}

/**
 * \brief Test whether two 3D points are equal
 *
 * Test whether the given points are equal, comparing for this purpose their
 * three components as in retQSS::numbers_are_equal().
 */
inline bool points_are_equal(
		const Point_3 &p,
		const Point_3 &q)
{
	return
		std::abs(p.x() - q.x()) < RETQSS_EQ_TOLERANCE &&
		std::abs(p.y() - q.y()) < RETQSS_EQ_TOLERANCE &&
		std::abs(p.z() - q.z()) < RETQSS_EQ_TOLERANCE;
}

/**
 * \brief Test whether two 3D vectors are equal
 *
 * Test whether the given vectors are equal, comparing for this purpose their
 * three components as in retQSS::numbers_are_equal().
 */
inline bool vectors_are_equal(
		const Vector_3 &u,
		const Vector_3 &v)
{
	return
		std::abs(u.x() - v.x()) < RETQSS_EQ_TOLERANCE &&
		std::abs(u.y() - v.y()) < RETQSS_EQ_TOLERANCE &&
		std::abs(u.z() - v.z()) < RETQSS_EQ_TOLERANCE;
}

/**
 * \brief Test whether two numbers are equal
 *
 * Test whether \f$|x-y| < \epsilon\$, where \f$\epsilon\f$ is the predefined
 * tolerance for equality comparisons.
 */
inline bool numbers_are_equal(const double &x, const double &y)
{
	return std::abs(x-y) < RETQSS_EQ_TOLERANCE;
}

/**
 * \brief Change random seed
 */
bool random_reseed(int seed);

/**
 * \brief Return random double in the range \f$[\textrm{from}, \textrm{to}]\f$
 */
double random_double(double from, double to);

/**
 * \brief Return random double in lognormal distribution \f$[\textrm{m}, \textrm{s}]\f$
 */
double random_lognormal(double m, double s);

/**
 * \brief Return random double in normal distribution \f$[\textrm{m}, \textrm{s}]\f$
 */
double random_normal(double m, double s);

/**
 * \brief Return random integer in the range \f$[\textrm{from}, \textrm{to}]\f$
 */
int random_int(int from, int to);

}

#endif

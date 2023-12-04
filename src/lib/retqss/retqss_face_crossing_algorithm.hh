/**
 * \file retqss_face_crossing_algorithm.hh
 */

#ifndef _RETQSS_FACE_CROSSING_ALGORITHM_H_
#define _RETQSS_FACE_CROSSING_ALGORITHM_H_

#include "retqss_intersection_locator.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_trajectory.hh"
#include "retqss_volume.hh"
#include "retqss_types.hh"
#include "retqss_particle.hh"
#include "retqss_simulator_interface.hh"
#include "retqss_particle_tracker.hh"

/**
 * Below this number, every face of the volume will be queried for a possible
 * crossing. If it has more faces, the face neighborhood approach will be used
 * instead.
 *
 * \sa retQSS::FaceNeighborhood
 */
#define FACE_THRESH 50

/**
 * \namespace retQSS Main retQSS namespace
 */
namespace retQSS
{

/**
 * \brief Face crossing algorithm implementation
 *
 * This class implements the algorithm to detect the next upcoming volume
 * boundary crossing by a traveling particle \f$p\f$. It outputs a tuple
 * \f$(t, f, P)\f$ such that \f$t\f$ is the next crossing time, \f$f\f$ is
 * the face to be crossed and \f$P\f$ is the point where particle \f$p\f$'s
 * trajectory intersects face \f$f\f$ (see retQSS::CrossedFaceData).
 *
 * See method retQSS::FaceCrossingAlgorithm::find_crossed_face() for
 * further details.
 */
class FaceCrossingAlgorithm
{
private:
	retQSS::ParticleTracker *tracker;
	retQSS::IntersectionLocator *locator;
	unsigned int face_thresh;

	CrossedFaceData query_all_faces(
			Internal_ParticleID p,
			retQSS::Volume *volume,
			const retQSS::Trajectory &trajectory);

	CrossedFaceData query_target_face_neighborhood(
			Internal_ParticleID p,
			retQSS::Volume *volume,
			const retQSS::Trajectory &trajectory);

public:
	FaceCrossingAlgorithm(
			retQSS::ParticleTracker *tracker,
			unsigned int face_thresh =FACE_THRESH);

	~FaceCrossingAlgorithm();

	/**
	 * \brief Compute first upcoming intersection with the facets of \f$v\f$.
	 *
	 * Compute first upcoming intersection of particle \f$p\f$'s trajectory
	 * with any of the facets of volume \f$v\f$. The algorithm works as
	 * follows:
	 *   - First, a trajectory object is assembled by querying back the
	 *   simulation engine in order to retrieve the polynomial approximations
	 *   of the position state variables (see retQSS::Trajectory).
	 *   - Let \f$n_v\f$ be the number of faces of \f$v\f$ and let \f$T\f$ be
	 *   the threshold value <tt>face_thresh</tt>.
	 *   - If \f$n_v \leq T\f$,
	 *    -# Every face \f$f\f$ of \f$v\f$ is successively
	 *    tested for possible intersections. This is accomplished by the
	 *    intersection locator object (retQSS::IntersectionLocator), which
	 *    solves a polynomial equation involving the plane that contains
	 *    \f$f\f$ and the approximation of \f$p\f$'s trajectory.
	 *    -# If several faces satisfy this equation, the one having the least
	 *    intersection time is chosen.
	 *    -# If no face satisfies the equation, then there is no intersection
	 *    and a null face pointer is returned by the algorithm.
	 *   - When \f$n_v > T\f$,
	 *    -# A target face \f$f'\f$ is first found by intersecting a ray
	 *    \f$\vec{r}\f$ with volume \f$v\f$'s faces, using for this purpose the
	 *    underlying AABB tree of \f$v\f$ (see retQSS::AABBTree).
	 *    -# \f$\vec{r}\f$ starts at \f$\PartPos{p}\f$ and has direction
	 *    \f$\PartVel{p}\f$.
	 *    -# Then, for each face \f$h\f$ in a neighborhood of \f$f'\f$ (see
	 *    retQSS::FaceNeighborhood), the intersection locator is used to solve
	 *    the plane equation mentioned above. As soon as the equation is
	 *    successfully solved, the algorithm stops and returns the intersection
	 *    just found. The underlying assumption is that \f$p\f$'s trajectory
	 *    cannot have several crossing points. This assumption is important.
	 *    -# In case no face in the neighborhood satisfies this, we assume
	 *    there is no intersection and a null face pointer is returned.
	 *
	 * \param v If pointer is null, it is set to \f$\Vol{p}\f$.
	 */
	CrossedFaceData find_crossed_face(
			retQSS::Particle *particle,
			retQSS::Volume *v =nullptr);
};

}

#endif

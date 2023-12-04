/**
 * \file retqss_face_neighborhood.hh
 * \brief Implementation of neighborhoods of volume faces
 */

#ifndef _RETQSS_FACE_NEIGHBORHOOD_H_
#define _RETQSS_FACE_NEIGHBORHOOD_H_

#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <utility>

#include "retqss_polyhedron_face.hh"

/**
 * Depth of the face neighborhood. It determines the maximum number of faces
 * between the starting face and any other face in the neighborhood.
 */
#define _NEIGHBORHOOD_DEPTH 5


/**
 * \namespace retQSS Main retQSS namespace
 */
namespace retQSS
{

class FaceNeighborhoodIterator;
class Volume;

/**
 * \brief Implementation of volume face neighborhoods
 *
 * This class provides an implementation of face neighborhoods that is used
 * by the face crossing algorithm (retQSS::FaceCrossingAlgorithm) to test for
 * possible intersections in faces that are close to an initial target face.
 * Given such a face \f$f\f$, a face \f$f'\f$ is considered neighbor of \f$f\f$
 * iff there is a sequence of \f$1 \leq k \leq d\f$ faces
 * \f$f = f_1,\dots,f_k = f'\f$ such that \f$f_{i+1}\f$ is  adjacent to
 * \f$f_i\f$ (i.e., they both share one edge), \f$1 \leq i < k\f$. Here,
 * \f$d\f$ is the \em neighborhood \em depth, represented by attribute
 * retQSS::FaceNeighborhood::depth.
 */
class FaceNeighborhood
{
	friend class retQSS::PolyhedronFace;
	friend class retQSS::FaceNeighborhoodIterator;

protected:
	retQSS::Volume *vol;
	retQSS::PolyhedronFace *face;
	/**
	 * Face neighborhood depth
	 */
	int depth;

public:
	typedef retQSS::FaceNeighborhoodIterator iterator;

	FaceNeighborhood();
	FaceNeighborhood(retQSS::Volume *v, retQSS::PolyhedronFace *f, int depth);
	virtual ~FaceNeighborhood() {};

	iterator begin();
	iterator end();

	/**
	 * \brief Compute immediate neighbors of \f$f\f$
	 *
	 * Compute immediate face neighbors of face \f$f\f$. In case \f$v\f$ is a
	 * polyhedral volume (retQSS::PolyhedralVolume), \f$f\f$'s neighbors are
	 * just those faces that share one edge with \f$f\f$ in \f$v\f$. If \f$v\f$
	 * is an open volume (retQSS::OpenVolume), these neighbors might come from
	 * different polyhedral volumes, as they are faces on the geometry surface.
	 * These different types of immediate face neighbors are precomputed for
	 * each face upon initializing the geometry (see retQSS::GeometryParser).
	 */
	const std::vector<retQSS::PolyhedronFace*>&
	get_face_neighbors(retQSS::PolyhedronFace *f) const;
};

/**
 * \brief C++ iterator of face neighborhoods
 *
 * This class implements a C++ iterator to facilitate traversal through a face
 * neighborhood. Refer to retQSS::FaceNeighborhoodIterator::operator++() for
 * further details on the underlying algorithm.
 */
class FaceNeighborhoodIterator :
		public std::iterator<std::forward_iterator_tag, PolyhedronFace>
{
	friend class FaceNeighborhood;

private:
	/**
	 * Face neighborhood object that is being iterated by this iterator.
	 */
	retQSS::FaceNeighborhood *owner;
	/**
	 * Current face pointed by the iterator.
	 */
	retQSS::PolyhedronFace *current_face;
	int face_depth, depth;
	bool ended;

	/**
	 * Neighbor faces pending to be visited. The second argument in the queued
	 * pair is the current depth of the face.
	 */
	std::queue<std::pair<retQSS::PolyhedronFace*,int>> faces;
	/**
	 * Neighbor faces already visited, so that we do not repeat them.
	 */
	std::set<retQSS::PolyhedronFace*> seen;

	FaceNeighborhoodIterator();
	FaceNeighborhoodIterator(retQSS::FaceNeighborhood *f, int depth);

public:
	/**
	 * \brief Get current face
	 */
	PolyhedronFace *operator*();

	/**
	 * \brief Move iterator forward
	 *
	 * Update current face following these steps:
	 *  - Let \f$f\f$ be the current face (initially, this is set to the target
	 *  face passed as argument to the face neighborhood constructor).
	 *  - Compute the immediate face neighbors of \f$f\f$
	 *  (see retQSS::FaceNeighborhood::get_face_neighbors()), and for each one
	 *  of them, add it to the pending queue in case they were not already
	 *  visited and in case their depth is below the threshold. Increment
	 *  \f$f\f$'s depth to set this depth. Finally, mark them as seen.
	 *  - If there are no pending faces enqueued, mark the iterator as
	 *  finished.
	 *  - Otherwise, set as current face the next upcoming face in the queue.
	 */
	const FaceNeighborhoodIterator& operator++();

	/**
	 * \brief Iterator equality
	 */
	bool operator!=(const FaceNeighborhoodIterator&) const;
};

}

#endif

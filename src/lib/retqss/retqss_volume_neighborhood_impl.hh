/**
 * \file retqss_volume_neighborhood_impl.hh
 */

#ifndef _RETQSS_VOLUME_NEIGHBORHOOD_IMPL_H_
#define _RETQSS_VOLUME_NEIGHBORHOOD_IMPL_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "retqss_geometry.hh"
#include "retqss_volume.hh"
#include "retqss_volume_neighbor.hh"
#include "retqss_types.hh"


namespace retQSS
{

/**
 * \brief Base class for neighborhood computation algorithms
 *
 * This class defines an interface for concrete algorithms to compute volume
 * neighborhoods. In particular, such algorithms must define their behavior
 * by providing an implementation of method
 * retQSS::NeighborhoodImplementation::compute_neighbors(), which computes
 * the array of neighbors for a given volume.
 */
class VolumeNeighborhoodImplementation
{
protected:
	typedef std::unordered_set<const retQSS::Volume*> VolumeSet;

	const retQSS::Geometry *geometry; //!< Geometry to retrieve and query
	                                  //!< volume objects

	/**
	 * \brief Create new neighbor object
	 *
	 * Create new neighbor object for the given volume.
	 */
	retQSS::VolumeNeighbor *new_neighbor(
			const retQSS::Volume *volume,
			retQSS::Volume *neighbor) const;

	/**
	 * \brief Sort neighbors by ID
	 */
	void sort_by_ID(retQSS::VolumeNeighborArray &volumes) const;

	/**
	 * \brief Sort neighbors by distance
	 *
	 * Sort neighbors in the given array by their distance to the volume
	 * centroid. Use volume IDs to break ties.
	 */
	void sort_by_distance(
			retQSS::VolumeNeighborArray &volumes) const;

public:
	VolumeNeighborhoodImplementation(const retQSS::Geometry*);

	virtual ~VolumeNeighborhoodImplementation() {};

	/**
	 * \brief Compute neighbors
	 *
	 * Compute neighbors of the given volume. Subclasses must implement this
	 * method.
	 */
	virtual retQSS::VolumeNeighborArray compute_neighbors(
			const retQSS::Volume *volume) = 0;
};

/**
 * \brief Default volume neighborhood
 *
 * In default neighborhoods, a volume \f$w\f$ is considered neighbor of \f$v\f$
 * iff they share a common facet. These neighborhoods are defined upon
 * initializing geometry data structures, and thus there are no extra
 * performance penalties in computing them.
 */
class DefaultNeighborhood : public VolumeNeighborhoodImplementation
{
public:
	DefaultNeighborhood(const retQSS::Geometry *geometry)
		: VolumeNeighborhoodImplementation(geometry) {};

	virtual retQSS::VolumeNeighborArray compute_neighbors(
			const retQSS::Volume *volume);
};

/**
 * \brief File-based volume neighborhood
 *
 * A text file is used to define the neighbors of each volume. This
 * user-supplied file is expected to define one neighborhood per line, starting
 * with a polyehdral volume ID \f$v\f$ followed by every neighbor ID of \f$v\f$
 * separated by an arbitrary number of spaces. In case a volume is not included,
 * its neighborhood will be empty.
 */
class FileNeighborhood : public VolumeNeighborhoodImplementation
{
private:
	std::unordered_map<
		const retQSS::Volume*,
		retQSS::VolumeNeighborArray*> neighborhood;

public:
	FileNeighborhood(const retQSS::Geometry*, const std::string&);
	virtual ~FileNeighborhood();

	virtual retQSS::VolumeNeighborArray compute_neighbors(
			const retQSS::Volume *volume);
};

/**
 * \brief Radial volume neighborhood
 *
 * In radial neighborhoods, a volume \f$w\f$ is considered neighbor of volume
 * \f$v\f$ iff the Euclidean distance from \f$v\f$'s centroid to \f$w\f$ is not
 * greater than \f$r\f$. Neighborhoods are sorted increasingly by this distance
 * (i.e., neighbor at index 1 is the closest to \f$v\f$'s centroid).
 */
class RadialNeighborhood : public VolumeNeighborhoodImplementation
{
protected:
	typedef std::queue<
				std::tuple<
					const retQSS::Volume*,
					Point_3,
					double,
					int
				>
			> RadialQueue;

	typedef std::pair<const retQSS::Volume*, double> VolumeDist;

	double radius;

	/**
	 * \brief Create new neighbor object
	 *
	 * Create new neighbor object for the given volume.
	 */
	retQSS::VolumeNeighbor *new_neighbor(
			const retQSS::Volume *volume,
			const retQSS::Volume *neighbor,
			const Point_3 &centroid,
			double distance) const;

	/**
	 * \brief Take actions for volumes on the geometry boundary
	 *
	 * Will do nothing for standard radial neighborhoods. For further
	 * information, see retQSS::PeriodicRadialNeighborhood.
	 */
	virtual void on_boundary_neighbor(
			retQSS::PolyhedronFace *face,
			const Point_3 &centroid,
			int jumps,
			RadialQueue &queue,
			VolumeSet &seen) const;

public:
	RadialNeighborhood(const retQSS::Geometry*, double);

	virtual retQSS::VolumeNeighborArray compute_neighbors(
			const retQSS::Volume *volume);
};

/**
 * \brief Periodic radial volume neighborhood
 *
 * This is a special kind of radial neighborhood in which volumes on the
 * geometry boundary are considered periodic. In these cases, the periodic
 * radial neighborhood translates \f$v\f$'s centroid using the periodic
 * boundary face inward normal and resumes the computation in the opposite
 * side of the geometry.
 */
class PeriodicRadialNeighborhood : public RadialNeighborhood
{
protected:

	/**
	 * \brief Take actions for volumes on the geometry boundary
	 *
	 * Add potential neighbors in the opposite side of the geometry, taking
	 * the inward normal of the given face and retrieving the corresponding
	 * boundary volume intersected in the opposite side. The original centroid
	 * is also moved to a new location using the vector obtained to map the
	 * volume on the opposite side.
	 */
	virtual void on_boundary_neighbor(
			retQSS::PolyhedronFace *face,
			const Point_3 &centroid,
			int jumps,
			RadialQueue &queue,
			VolumeSet &seen) const;

private:

	/**
	 * \brief Translate centroid to opposite side of the geometry
	 *
	 * \sa retQSS::PeriodicRadialNeighborhood::on_boundary_neighbor()
	 */
	Point_3 move_centroid(
			const Point_3 &centroid,
			retQSS::PolyhedronFace *face) const;

public:
	PeriodicRadialNeighborhood(
			const retQSS::Geometry *geometry,
			double radius)
			: retQSS::RadialNeighborhood(geometry, radius) {};
};

/**
 * \brief Vertex-sharing volume neighborhood
 *
 * In a vertex-sharing volume neighborhood, a volume \f$w\f$ is considered
 * neighbor of volume \f$v\f$ iff they both share at least one common vertex.
 */
class VertexSharingNeighborhood : public VolumeNeighborhoodImplementation
{
public:
	VertexSharingNeighborhood(const retQSS::Geometry *geometry)
		: VolumeNeighborhoodImplementation(geometry) {};

	virtual retQSS::VolumeNeighborArray compute_neighbors(
			const retQSS::Volume *volume);
};

}

#endif

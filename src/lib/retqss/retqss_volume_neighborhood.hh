/**
 * \file retqss_volume_neighborhood.hh
 */

#ifndef _RETQSS_VOLUME_NEIGHBORHOOD_H_
#define _RETQSS_VOLUME_NEIGHBORHOOD_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "retqss_geometry.hh"
#include "retqss_volume.hh"
#include "retqss_volume_neighborhood_impl.hh"
#include "retqss_volume_neighbor.hh"
#include "retqss_types.hh"


namespace retQSS
{

/**
 * \brief Volume neighborhood implementation
 *
 * Given a volume \f$v\f$, its \em volume \em neighborhood is a (possibly empty)
 * subset of the polyhedral volumes in the geometry. This class provides an
 * implementation of volume neighborhoods exposing a API to transparently
 * access and use different neighborhood computation algorithms. In fact, these
 * algorithms can be easily changed on runtime during the simulation.
 *
 * \paragraph On-demand computation
 * For efficiency purposes, neighborhoods are lazily computed, meaning that the
 * algorithm to compute the neighbors of a given volume will be executed exactly
 * when it is required. Once this is done, the neighborhood will be cached for
 * future reference.
 *
 * \paragraph Particle neighborhoods
 * Particle neighborhoods (retQSS::ParticleNeighborhood) are strongly dependent
 * on the volume neighborhood at hand.
 */
class VolumeNeighborhood
{
private:
	retQSS::VolumeNeighborhoodImplementation *impl; //!< Current algorithm

	const retQSS::Geometry *geometry; //!< Geometry to retrieve and query
	                                  //!< volume objects

	std::unordered_map<
		const retQSS::Volume*,
		retQSS::VolumeNeighborArray>
			_neighbors; //!< Cache of neighborhoods previously computed

	std::mutex mutex; //!< Mutex to protect against concurrent access to
	                  //!< neighborhoods (for parallel simulations)

	/**
	 * \brief Delete current algorithm and clear cache
	 */
	void clear_impl();


public:
	/**
	 * \brief Constructor
	 *
	 * Create new volume neighborhood
	 */
	VolumeNeighborhood(retQSS::Geometry *geometry);

	~VolumeNeighborhood();

	/**
	 * \brief Retrieve neighbor
	 *
	 * Retrieve the \f$i\f$-th neighbor of the given volume.
	 */
	retQSS::Volume *get_neighbor(
			const retQSS::Volume *volume,
			Internal_Index i);

	/**
	 * \brief Count volumes in neighborhood
	 *
	 * Count number of volumes in the neighborhood of the given volume.
	 */
	size_t num_neighbors(const retQSS::Volume *volume);

	/**
	 * \brief Get array of neighbors
	 *
	 * Get array of neighbors of the given volume.
	 */
	const retQSS::VolumeNeighborArray &neighbors(
			const retQSS::Volume *volume);

	/**
	 * \brief Switch to default volume neigborhood
	 *
	 * \sa retQSS::DefaultNeighborhood
	 */
	void switch_to_default();

	/**
	 * \brief Switch to file-based volume neigborhood
	 *
	 * \sa retQSS::FileNeighborhood
	 */
	void switch_to_file(const std::string &filename);

	/**
	 * \brief Switch to radial volume neigborhood
	 *
	 * \sa retQSS::RadialNeighborhood
	 */
	void switch_to_radial(double radius);

	/**
	 * \brief Switch to periodic radial volume neigborhood
	 *
	 * \sa retQSS::PeriodicRadialNeighborhood
	 */
	void switch_to_periodic_radial(double radius);

	/**
	 * \brief Switch to vertex sharing volume neigborhood
	 *
	 * \sa retQSS::VertexSharingNeighborhood
	 */
	void switch_to_vertex_sharing();
};

}

#endif

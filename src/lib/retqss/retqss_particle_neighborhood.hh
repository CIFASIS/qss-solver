/**
 * \file retqss_particle_neighborhood.hh
 */

#ifndef _RETQSS_PARTICLE_NEIGHBORHOOD_H_
#define _RETQSS_PARTICLE_NEIGHBORHOOD_H_

#include <iterator>

#include "retqss_particle.hh"
#include "retqss_particle_neighbor.hh"
#include "retqss_particle_tracker.hh"
#include "retqss_volume.hh"
#include "retqss_volume_neighborhood.hh"
#include "retqss_types.hh"


namespace retQSS
{

class ParticleNeighborhoodIterator;

/**
 * \brief Particle neighborhood implementation
 *
 * Given a particle \f$p\f$, its \em particle \em neighborhood is the set of
 * particles that are inside every neighboring volume of \f$\Vol{p}\f$. This
 * class provides an API to easily access and traverse particles in
 * neighborhoods.
 *
 * \note Only live particles are neighborhood members. If a particle is killed
 * during the simulation, it will not be included in any furhter particle
 * neighborhoods.
 *
 * \note Particle neighborhoods have a strong dependence with volume
 * neighborhoods (retQSS::VolumeNeighborhood). In fact, if the volume
 * neighborhood algorithm is changed, so is every particle neighborhood.
 * Consider also that neighboring particles may not be neighbors from a
 * geometrical point of view (e.g., for periodic or user-defined volume
 * neighborhoods).
 */
class ParticleNeighborhood
{
	friend class retQSS::ParticleNeighborhoodIterator;

private:
	const retQSS::ParticleTracker
		*tracker; //!< Particle tracker to access particles
	retQSS::Particle *particle; //!< Source particle
	retQSS::ParticleNeighbor
		*neighbor; //!< Particle neighbor object (single instance that is reset
	               //!< for every neighboring particle)
	retQSS::Volume *volume; //!< Volume of source particle
	retQSS::VolumeNeighborArray
		vol_neighbors; //!< Precomputed neighbors of source volume

public:
	typedef retQSS::ParticleNeighborhoodIterator iterator;

	ParticleNeighborhood() = delete;

	/**
	 * \brief Constructor
	 *
	 * Create new particle neighborhood for the given particle
	 */
	ParticleNeighborhood(
			const retQSS::ParticleTracker *tracker,
			retQSS::Particle *particle);

	~ParticleNeighborhood();

	/**
	 * \brief Reset particle neighborhood
	 *
	 * Reset particle neighborhood
	 */
	void reset(retQSS::VolumeNeighborhood *volume_neighborhood);

	/**
	 * \brief Get iterator positioned at first particle in neighborhood
	 */
	iterator begin();

	/**
	 * \brief Get iterator positioned at the end of the neighborhood
	 */
	iterator end();
};


/**
 * \brief Iterator of particle neighborhoods
 *
 * This class provides a C++-compatible iterator of particle neighborhoods.
 */
class ParticleNeighborhoodIterator :
		public std::iterator<std::forward_iterator_tag, retQSS::ParticleNeighbor>
{
	friend class ParticleNeighborhood;

private:
	retQSS::ParticleNeighborhood *owner; //!< Particle neighborhood that is
	                                     //!< being iterated

	retQSS::ParticleIDSet::const_iterator
		current_particle_idx; //!< Index of pointed particle inside its
	                          //!< corresponding volume (i.e., an index into
	                          //!< <tt>current_particles</tt>

	size_t current_volume_idx; //!< Index of current volume being traversed

	const retQSS::ParticleIDSet
		*current_particles; //!< Particles inside current volume

	bool ended; //!< Whether the iterator has reached the end of the
	            //!< neighborhood

	/**
	 * \brief Constructor
	 *
	 * Create new iterator for the given particle neighborhood.
	 */
	ParticleNeighborhoodIterator(
			retQSS::ParticleNeighborhood *neighborhood,
			bool has_ended);

public:
	/**
	 * \brief Get current neighbor
	 *
	 * Get particle neighbor currently pointed at by the iterator.
	 */
	retQSS::ParticleNeighbor *operator*();

	/**
	 * \brief Advance to next neighbor
	 *
	 * Move iterator forward to point to the next particle.
	 *
	 * \note Dead particles are skipped.
	 */
	const ParticleNeighborhoodIterator& operator++();

	/**
	 * \brief Check whether two iterators are equivalent
	 *
	 * Two iterators of the same particle neighborhood are considered equal if
	 * both ended or otherwise if both point the same particle in the same
	 * volume.
	 */
	bool operator!=(const ParticleNeighborhoodIterator &iterator) const;
};

}

#endif

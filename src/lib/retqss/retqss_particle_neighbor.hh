/**
 * \file retqss_particle_neighbor.hh
 */

#ifndef _RETQSS_PARTICLE_NEIGHBOR_H_
#define _RETQSS_PARTICLE_NEIGHBOR_H_

#include "retqss_particle.hh"
#include "retqss_particle_tracker.hh"
#include "retqss_volume_neighbor.hh"
#include "retqss_cgal_main_types.hh"


namespace retQSS
{

class ParticleNeighborhood;
class ParticleNeighborhoodIterator;

/**
 * \brief Particle neighbor
 *
 * This class implements the concept of neighboring particles, consolidating
 * information about the \em source particle and the \em neighbor particle.
 * It is instanciated by particle neighborhood objects
 * (retQSS::ParticleNeighborhood) and fed into user-supplied functions for
 * iterating such neighborhoods.
 *
 * When using periodic volume neighborhoods, the Euclidean distance and the
 * \em virtual distance between the source and neighbor particles might not
 * match. A particle neighbor object solves this problem by suppling
 * convenience methods to properly and transparently calculate these distances
 * along with their corresponding vectors from the source particle to its
 * neighbor.
 */
class ParticleNeighbor
{
	friend class retQSS::ParticleNeighborhood;
	friend class retQSS::ParticleNeighborhoodIterator;

private:
	retQSS::Particle *source; //!< Source particle
	Point_3 source_pos; //!< Computed position of source particle
	Vector_3 source_vel; //!< Computed velocity of source particle
	bool source_pos_computed; //!< Whether source_pos is already computed
	bool source_vel_computed; //!< Whether source_vel is already computed

	retQSS::Particle *neighbor; //!< Neighbor particle
	Point_3 neighbor_pos; //!< Computed position of neighbor particle
	Vector_3 neighbor_vel; //!< Computed velocity of neighbor particle
	bool neighbor_pos_computed; //!< Whether neighbor_pos is already computed
	bool neighbor_vel_computed; //!< Whether neighbor_vel is already computed

	retQSS::VolumeNeighbor *volume_neighbor; //!< Volume neighbor object

	double dist; //!< Distance between source and neighbor
	double euclidean_dist; //!< Euclidean distance between source and neighbor

	Vector_3 shortest_vec; //!< Shortest vector between source and neighbor
	Vector_3 euclidean_vec; //!< Euclidean vector between source and neighbor

	const retQSS::ParticleTracker *tracker; //!< Particle tracker

	/**
	 * \brief Compute distance between source and neighbor
	 *
	 * Compute distance between the particles. The Euclidean distance may not
	 * be this distance e.g. if periodic volume neighborhoods are used.
	 */
	void compute_distance();

	/**
	 * \brief Compute Euclidean distance between source and neighbor
	 */
	void compute_euclidean_distance();

	/**
	 * \brief Constructor
	 */
	ParticleNeighbor(
			retQSS::Particle *source,
			const retQSS::ParticleTracker *tracker);

	void reset();

	void update(
			retQSS::Internal_ParticleID q,
			retQSS::VolumeNeighbor *volume_neighbor);

public:
	ParticleNeighbor() = delete;

	~ParticleNeighbor() {};

	/**
	 * \brief Retrieve source particle
	 */
	inline retQSS::Particle *source_particle() const
	{
		return this->source;
	}

	/**
	 * \brief Retrieve neighbor particle
	 */
	inline retQSS::Particle *neighbor_particle() const
	{
		return this->neighbor;
	}

	/**
	 * \brief Retrieve position of source particle
	 */
	inline Point_3 source_position()
	{
		if(!this->source_pos_computed)
		{
			this->source_pos =
					this->tracker->current_position(this->source->get_ID());
			this->source_pos_computed = true;
		}
		return this->source_pos;
	}

	/**
	 * \brief Retrieve position of neighbor particle
	 */
	inline Point_3 neighbor_position()
	{
		if(!this->neighbor_pos_computed)
		{
			this->neighbor_pos =
					this->tracker->current_position(this->neighbor->get_ID());
			this->neighbor_pos_computed = true;
		}
		return this->neighbor_pos;
	}

	/**
	 * \brief Retrieve velocity of source particle
	 */
	inline Vector_3 source_velocity()
	{
		if(!this->source_vel_computed)
		{
			this->source_vel =
					this->tracker->current_velocity(this->source->get_ID());
			this->source_vel_computed = true;
		}
		return this->source_vel;
	}

	/**
	 * \brief Retrieve velocity of neighbor particle
	 */
	inline Vector_3 neighbor_velocity()
	{
		if(!this->neighbor_vel_computed)
		{
			this->neighbor_vel =
					this->tracker->current_velocity(this->neighbor->get_ID());
			this->neighbor_vel_computed = true;
		}
		return this->neighbor_vel;
	}

	/**
	 * \brief Retrieve volume of source particle
	 */
	inline retQSS::Volume *source_volume() const
	{
		return this->volume_neighbor->source_volume();
	}

	/**
	 * \brief Retrieve volume of neighbor particle
	 */
	inline retQSS::Volume *neighbor_volume() const
	{
		return this->volume_neighbor->neighbor_volume();
	}

	/**
	 * \brief Retrieve shortest vector from source to neighbor
	 */
	inline Vector_3 shortest_vector()
	{
		if(this->dist < 0)
			this->compute_distance();
		return this->shortest_vec;
	}

	/**
	 * \brief Retrieve Euclidean vector from source to neighbor
	 */
	inline Vector_3 euclidean_vector()
	{
		if(this->euclidean_dist < 0)
			this->compute_euclidean_distance();
		return this->euclidean_vec;
	}

	/**
	 * \brief Retrieve distance from source to neighbor
	 *
	 * \note The Euclidean distance may not be this distance e.g. if periodic
	 *  volume neighborhoods are used.
	 */
	inline double distance()
	{
		if(this->dist < 0)
			this->compute_distance();
		return this->dist;
	}

	/**
	 * \brief Retrieve Euclidean distance from source to neighbor
	 */
	inline double euclidean_distance()
	{
		if(this->euclidean_dist < 0)
			this->compute_euclidean_distance();
		return this->euclidean_dist;
	}
};

}

#endif

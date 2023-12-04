#include "retqss_particle_neighbor.hh"
#include "retqss_polyhedral_volume.hh"
#include "retqss_utilities.hh"


retQSS::ParticleNeighbor::ParticleNeighbor(
		retQSS::Particle *source,
		const retQSS::ParticleTracker *tracker) :
		source(source),
		source_pos_computed(false),
		source_vel_computed(false),
		tracker(tracker)
{
}

void retQSS::ParticleNeighbor::reset()
{
	this->source_pos_computed = false;
	this->source_vel_computed = false;
}

void retQSS::ParticleNeighbor::update(
		retQSS::Internal_ParticleID q,
		retQSS::VolumeNeighbor *volume_neighbor)
{
	// IMPORTANT: this particle ID can be trusted --it is not supplied by the
	// user.
	this->neighbor = this->tracker->get_particle_unsafe(q);
	this->volume_neighbor = volume_neighbor;
	this->dist = -1;
	this->euclidean_dist = -1;
	this->neighbor_pos_computed = false;
	this->neighbor_vel_computed = false;
}

void retQSS::ParticleNeighbor::compute_distance()
{
	// Dynamic cast bypassed for efficiency purposes.
	auto source_vol =
			(retQSS::PolyhedralVolume*)
			this->volume_neighbor->source_volume();
	auto source_centroid = source_vol->centroid();
	auto used_centroid = this->volume_neighbor->centroid();

	if(!retQSS::points_are_equal(source_centroid, used_centroid))
	{
		// Centroid was moved during volume neighbor computation due to
		// periodic neighborhoods. Shortest and Euclidean distances
		// may not match.

		// 1. Compute vector from source centroid to its translation.
		Vector_3 centroid_v(source_centroid, used_centroid);

		// 2. Translate particle position.
		Point_3 translated_pos = this->source_position() + centroid_v;

		// 3. Compute vector to target position.
		this->shortest_vec = Vector_3(translated_pos, this->neighbor_position());
		this->dist = retQSS::vector_norm(this->shortest_vec);
	}
	else
	{
		this->euclidean_distance();
		this->shortest_vec = this->euclidean_vec;
		this->dist = this->euclidean_dist;
	}
}

void retQSS::ParticleNeighbor::compute_euclidean_distance()
{
	this->euclidean_vec =
			Vector_3(this->source_position(), this->neighbor_position());
	this->euclidean_dist = retQSS::vector_norm(this->euclidean_vec);
}

#include "retqss_particle_neighborhood.hh"


retQSS::ParticleNeighborhood::ParticleNeighborhood(
		const retQSS::ParticleTracker *tracker,
		retQSS::Particle *particle) :
		tracker(tracker),
		particle(particle),
		volume(nullptr)
{
	this->neighbor = new retQSS::ParticleNeighbor(particle, tracker);
}

retQSS::ParticleNeighborhood::~ParticleNeighborhood()
{
	delete this->neighbor;
}

void retQSS::ParticleNeighborhood::reset(
		retQSS::VolumeNeighborhood *vol_neighborhood)
{
	// IMPORTANT: this particle ID can be trusted --it is not supplied by the
	// user.
	this->volume =
			this->tracker->current_volume_unsafe(
					this->particle->get_ID());
	this->vol_neighbors = vol_neighborhood->neighbors(this->volume);
	this->neighbor->reset();
}

retQSS::ParticleNeighborhood::iterator
retQSS::ParticleNeighborhood::begin()
{
	return retQSS::ParticleNeighborhood::iterator(this, false);
}

retQSS::ParticleNeighborhood::iterator
retQSS::ParticleNeighborhood::end()
{
	return retQSS::ParticleNeighborhood::iterator(this, true);
}

retQSS::ParticleNeighborhoodIterator::ParticleNeighborhoodIterator(
		retQSS::ParticleNeighborhood *owner,
		bool ended) :
		owner(owner),
		current_volume_idx(0),
		ended(ended)
{
	if(this->ended)
		return;

	auto id = this->owner->particle->get_ID();
	auto vol = this->owner->volume;
	if(!vol->is_polyhedral())
	{
		this->ended = true;
		return;
	}

	while(true)
	{
		// TODO: fix for parallel simulations.
		this->current_particles = &vol->particles_inside_ref();
		this->current_particle_idx = this->current_particles->begin();

		for(auto pid : *this->current_particles)
		{
			if(pid != id && this->owner->tracker->is_alive(pid))
				return;
			this->current_particle_idx++;
		}

		this->current_volume_idx++;
		if(this->current_volume_idx >= this->owner->vol_neighbors.size())
		{
			this->ended = true;
			return;
		}

		auto neighbor = this->owner->vol_neighbors[this->current_volume_idx];
		vol = neighbor->neighbor_volume();
	}
}

retQSS::ParticleNeighbor *retQSS::ParticleNeighborhoodIterator::operator*()
{
	auto q = *this->current_particle_idx;
	auto vol_neighbor = this->owner->vol_neighbors[this->current_volume_idx];

	this->owner->neighbor->update(q, vol_neighbor);

	return this->owner->neighbor;
}

const retQSS::ParticleNeighborhoodIterator
	&retQSS::ParticleNeighborhoodIterator::operator++()
{
	while(!this->ended)
	{
		this->current_particle_idx++;

		while(this->current_particle_idx == this->current_particles->end())
		{
			this->current_volume_idx++;
			if(this->current_volume_idx >= this->owner->vol_neighbors.size())
			{
				this->ended = true;
				return *this;
			}

			auto neighbor = this->owner->vol_neighbors[this->current_volume_idx];
			auto vol = neighbor->neighbor_volume();
			// TODO: fix for parallel simulations.
			this->current_particles = &vol->particles_inside_ref();
			this->current_particle_idx = this->current_particles->begin();
		}

		auto pid = *this->current_particle_idx;
		if(
			pid != this->owner->particle->get_ID() &&
			this->owner->tracker->is_alive(pid))
			break;
	}

	return *this;
}

bool retQSS::ParticleNeighborhoodIterator::operator!=(
		const retQSS::ParticleNeighborhoodIterator& iter) const
{
	if(this->owner != iter.owner)
		return true;

	if(this->ended && iter.ended)
		return false;

	return
			this->ended != iter.ended ||
			this->current_volume_idx != iter.current_volume_idx ||
			this->current_particle_idx != iter.current_particle_idx;
}

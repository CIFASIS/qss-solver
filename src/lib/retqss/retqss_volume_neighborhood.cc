#include "retqss_volume_neighborhood.hh"
#include "retqss_geometry.hh"
#include "retqss_exceptions.hh"


retQSS::VolumeNeighborhood::VolumeNeighborhood(
		retQSS::Geometry *geometry) :
		impl(nullptr),
		geometry(geometry)
{
	this->switch_to_default();
}

retQSS::VolumeNeighborhood::~VolumeNeighborhood()
{
	this->clear_impl();
}

void retQSS::VolumeNeighborhood::clear_impl()
{
	for(auto it : this->_neighbors)
	{
		for(auto neigh : it.second)
			delete neigh;
	}

	this->_neighbors.clear();

	if(this->impl != nullptr)
		delete this->impl;
}

retQSS::Volume *retQSS::VolumeNeighborhood::get_neighbor(
		const retQSS::Volume *volume,
		Internal_Index i)
{
	auto &neighbors = this->neighbors(volume);

	if(i >= neighbors.size())
		throw retQSS::InvalidIndexException(volume, i, "neighbors");

	return neighbors[i]->neighbor_volume();
}

const retQSS::VolumeNeighborArray &retQSS::VolumeNeighborhood::neighbors(
		const retQSS::Volume *volume)
{
	retQSS::Lock lock(this->mutex);

	auto it = this->_neighbors.find(volume);

	if(it == this->_neighbors.end())
	{
		auto vol_neighbors = this->impl->compute_neighbors(volume);
		auto pair = this->_neighbors.insert({volume, vol_neighbors});
		it = pair.first;
	}

	return it->second;
}

size_t retQSS::VolumeNeighborhood::num_neighbors(
		const retQSS::Volume *volume)
{
	auto &neighbors = this->neighbors(volume);
	return neighbors.size();
}

void retQSS::VolumeNeighborhood::switch_to_default()
{
	retQSS::Lock lock(this->mutex);

	this->clear_impl();
	this->impl = new retQSS::DefaultNeighborhood(this->geometry);
}

void retQSS::VolumeNeighborhood::switch_to_file(
		const std::string &filename)
{
	retQSS::Lock lock(this->mutex);

	this->clear_impl();
	this->impl = new retQSS::FileNeighborhood(this->geometry, filename);
}

void retQSS::VolumeNeighborhood::switch_to_radial(
		double radius)
{
	retQSS::Lock lock(this->mutex);

	this->clear_impl();
	this->impl = new retQSS::RadialNeighborhood(this->geometry, radius);
}

void retQSS::VolumeNeighborhood::switch_to_periodic_radial(
		double radius)
{
	retQSS::Lock lock(this->mutex);

	this->clear_impl();
	this->impl = new retQSS::PeriodicRadialNeighborhood(this->geometry, radius);
}

void retQSS::VolumeNeighborhood::switch_to_vertex_sharing()
{
	retQSS::Lock lock(this->mutex);

	this->clear_impl();
	this->impl = new retQSS::VertexSharingNeighborhood(this->geometry);
}

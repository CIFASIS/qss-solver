#include "retqss_volume_neighbor.hh"
#include "retqss_polyhedral_volume.hh"


retQSS::VolumeNeighbor::VolumeNeighbor(
		retQSS::Volume *volume,
		retQSS::Volume *neighbor) :
		vol(volume),
		neigh(neighbor)
{
	auto pvol = dynamic_cast<retQSS::PolyhedralVolume*>(vol);
	this->cent = pvol->centroid();
	this->dist = -1;
}

retQSS::VolumeNeighbor::VolumeNeighbor(
		retQSS::Volume *volume,
		retQSS::Volume *neighbor,
		const Point_3 &centroid,
		double distance) :
		vol(volume),
		neigh(neighbor),
		cent(centroid),
		dist(distance)
{
}

void retQSS::VolumeNeighbor::compute_distance()
{
	this->dist = this->neigh->distance_to(this->cent);
}

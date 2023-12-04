/**
 * \file retqss_volume_neighbor.hh
 */

#ifndef _RETQSS_VOLUME_NEIGHBOR_H_
#define _RETQSS_VOLUME_NEIGHBOR_H_

#include "retqss_volume.hh"
#include "retqss_cgal_main_types.hh"


namespace retQSS
{

/**
 * \brief
 */
class VolumeNeighbor
{
private:
	retQSS::Volume *vol;
	retQSS::Volume *neigh;

	Point_3 cent;
	double dist;

	void compute_distance();


public:
	VolumeNeighbor() = delete;

	VolumeNeighbor(
			retQSS::Volume *volume,
			retQSS::Volume *neighbor);

	VolumeNeighbor(
			retQSS::Volume *volume,
			retQSS::Volume *neighbor,
			const Point_3 &centroid,
			double distance =-1);

	~VolumeNeighbor() {};

	inline retQSS::Volume *source_volume() const
	{
		return this->vol;
	}

	inline retQSS::Volume *neighbor_volume() const
	{
		return this->neigh;
	}

	inline Point_3 centroid() const
	{
		return this->cent;
	}

	inline double distance()
	{
		if(this->dist < 0)
			this->compute_distance();
		return this->dist;
	}
};

}

#endif

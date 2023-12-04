#include "retqss_volume_neighborhood_impl.hh"


retQSS::VolumeNeighborhoodImplementation::VolumeNeighborhoodImplementation(
		const retQSS::Geometry *geometry)
	: geometry(geometry)
{
}

retQSS::VolumeNeighbor*
retQSS::VolumeNeighborhoodImplementation::new_neighbor(
		const retQSS::Volume *volume,
		retQSS::Volume *neighbor) const
{
	return new retQSS::VolumeNeighbor(
			const_cast<retQSS::Volume*>(volume), neighbor);
}

void retQSS::VolumeNeighborhoodImplementation::sort_by_ID(
		retQSS::VolumeNeighborArray &neighbors) const
{
	std::sort(
		neighbors.begin(),
		neighbors.end(),
		[](retQSS::VolumeNeighbor *neigh1, retQSS::VolumeNeighbor *neigh2)
		{
			auto vol1 = neigh1->neighbor_volume();
			auto vol2 = neigh2->neighbor_volume();

			return vol1->get_ID() < vol2->get_ID();
		});
}

void retQSS::VolumeNeighborhoodImplementation::sort_by_distance(
		VolumeNeighborArray &neighbors) const
{
	std::sort(
		neighbors.begin(),
		neighbors.end(),
		[&](
			retQSS::VolumeNeighbor *neigh1,
			retQSS::VolumeNeighbor *neigh2)
		{
			auto *vol1 = neigh1->neighbor_volume();
			double d1 = neigh1->distance();

			auto *vol2 = neigh2->neighbor_volume();
			double d2 = neigh2->distance();

			return
				d1 < d2 ||
				(d1 == d2 && vol1->get_ID() < vol2->get_ID());
		});
}

retQSS::VolumeNeighborArray retQSS::DefaultNeighborhood::compute_neighbors(
		const retQSS::Volume *volume)
{
	retQSS::VolumeNeighborArray neighbors;

	for(size_t i = 0; i < volume->num_adjacent_volumes(); i++)
		if(volume->is_polyhedral())
		{
			auto neighbor = volume->get_adjacent_volume(i);
			neighbors.push_back(this->new_neighbor(volume,neighbor));
		}

	return neighbors;
}

retQSS::FileNeighborhood::FileNeighborhood(
		const retQSS::Geometry *geometry,
		const std::string &filename)
	: retQSS::VolumeNeighborhoodImplementation(geometry)
{
	std::ifstream file(filename);
	std::string line;
	int id;

	if(!file.good())
		throw retQSS::FileAccessException(filename);

	while(file.good())
	{
		std::getline(file, line);
		std::stringstream line_stream(line);
		if(line_stream >> id)
		{
			auto volume = this->geometry->get_polyhedral_volume(id);
			retQSS::VolumeNeighborArray *neighbors =
					new retQSS::VolumeNeighborArray;

			while(line_stream >> id)
			{
				auto neighbor = this->geometry->get_volume(id);
				neighbors->push_back(this->new_neighbor(volume,neighbor));
			}

			this->neighborhood[volume] = neighbors;
		}
	}

	file.close();
}

retQSS::FileNeighborhood::~FileNeighborhood()
{
	for(auto it : this->neighborhood)
		delete it.second;
}

retQSS::VolumeNeighborArray retQSS::FileNeighborhood::compute_neighbors(
		const retQSS::Volume *volume)
{
	auto it = this->neighborhood.find(volume);
	if(it != this->neighborhood.end())
		return *it->second;
	else
		return retQSS::VolumeNeighborArray();
}

retQSS::RadialNeighborhood::RadialNeighborhood(
		const retQSS::Geometry *geometry,
		double radius)
	: retQSS::VolumeNeighborhoodImplementation(geometry),
	  radius(radius)
{
}

retQSS::VolumeNeighborArray retQSS::RadialNeighborhood::compute_neighbors(
		const retQSS::Volume *volume)
{
	auto pvol = dynamic_cast<const retQSS::PolyhedralVolume*>(volume);
	if(pvol == nullptr)
		return {};

	auto vol_centroid = pvol->centroid();
	VolumeSet seen;
	RadialQueue queue;
	retQSS::VolumeNeighborArray neighbors;

	queue.push(std::make_tuple(pvol,vol_centroid,0,0));

	while(!queue.empty())
	{
		auto tuple = queue.front();
		queue.pop();

		auto neighbor = std::get<0>(tuple);
		auto centroid = std::get<1>(tuple);
		auto neighbor_dist = std::get<2>(tuple);
		auto jumps = std::get<3>(tuple);

		if(seen.find(neighbor) != seen.end())
			continue;

		for(auto face : neighbor->faces())
		{
			if(!face->is_on_boundary())
			{
				auto adjacent_vols = neighbor->get_volumes_adjacent_to(face);
				for(auto adjacent_vol : adjacent_vols)
					if(seen.find(adjacent_vol) == seen.end())
					{
						double dist = adjacent_vol->distance_to(centroid);
						if(dist <= this->radius)
							queue.push(std::make_tuple(
												adjacent_vol,
												centroid,
												dist,
												jumps));
					}
			}
			else
				this->on_boundary_neighbor(face, centroid, jumps, queue, seen);
		}

		auto neighbor_obj =
				this->new_neighbor(
						volume,
						neighbor,
						centroid,
						neighbor_dist);
		neighbors.push_back(neighbor_obj);

		seen.insert(neighbor);
	}

	this->sort_by_distance(neighbors);

	return neighbors;
}

retQSS::VolumeNeighbor *retQSS::RadialNeighborhood::new_neighbor(
		const retQSS::Volume *volume,
		const retQSS::Volume *neighbor,
		const Point_3 &centroid,
		double distance) const
{
	return new retQSS::VolumeNeighbor(
			const_cast<retQSS::Volume*>(volume),
			const_cast<retQSS::Volume*>(neighbor),
			centroid,
			distance);
}

void retQSS::RadialNeighborhood::on_boundary_neighbor(
		retQSS::PolyhedronFace*,
		const Point_3&,
		int,
		RadialQueue&,
		VolumeSet&) const
{
	// Nothing to do: boundary faces do not add neighbors in standard radial
	// neighborhoods.
}

void retQSS::PeriodicRadialNeighborhood::on_boundary_neighbor(
		retQSS::PolyhedronFace *face,
		const Point_3 &centroid,
		int jumps,
		RadialQueue &queue,
		VolumeSet &seen) const
{
	// TODO
	if(jumps >= 3)
		return;

	auto opposite_vol = this->geometry->opposite_volume(face);
	if(seen.find(opposite_vol) == seen.end())
	{
		auto new_centroid = this->move_centroid(centroid, face);
		double dist = opposite_vol->distance_to(new_centroid);
		if(dist <= this->radius)
			queue.push(std::make_tuple(
						opposite_vol,
						new_centroid,
						dist,
						jumps+1));
	}
}

Point_3 retQSS::PeriodicRadialNeighborhood::move_centroid(
		const Point_3 &centroid,
		retQSS::PolyhedronFace *face) const
{
	auto face_centroid = face->centroid();
	auto face_and_point = this->geometry->opposite_intersection(
											face,
											face_centroid);
	auto int_point = std::get<1>(face_and_point);
	auto vector = Vector_3(face_centroid, int_point);
	return centroid + vector;
}

retQSS::VolumeNeighborArray
retQSS::VertexSharingNeighborhood::compute_neighbors(
		const retQSS::Volume *volume)
{
	retQSS::VolumeNeighborArray neighbors;
	VolumeSet seen;
	seen.insert(volume);

	for(auto vertex : volume->vertices())
	{
		auto vols = this->geometry->get_vertex_volumes(vertex);
		for(auto neighbor : vols)
		{
			if(seen.find(neighbor) == seen.end())
				neighbors.push_back(this->new_neighbor(volume,neighbor));
			seen.insert(neighbor);
		}
	}

	this->sort_by_ID(neighbors);

	return neighbors;
}

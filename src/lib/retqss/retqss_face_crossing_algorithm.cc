#include "retqss_face_crossing_algorithm.hh"


retQSS::FaceCrossingAlgorithm::FaceCrossingAlgorithm(
		retQSS::ParticleTracker *tracker,
		unsigned int face_thresh)
		: tracker(tracker),
		  face_thresh(face_thresh)
{
	int order = tracker->order();
	this->locator = retQSS::IntersectionLocator::for_order(order);
}

retQSS::FaceCrossingAlgorithm::~FaceCrossingAlgorithm()
{
	delete this->locator;
}

retQSS::CrossedFaceData
retQSS::FaceCrossingAlgorithm::find_crossed_face(
		retQSS::Particle *particle,
		retQSS::Volume *vol)
{
	auto trajectory = particle->trajectory();
	auto p = particle->get_ID();

	if(vol == nullptr)
		vol = this->tracker->current_volume(p);

	if(vol->num_faces() <= this->face_thresh)
		return this->query_all_faces(p, vol, *trajectory);
	else
		return this->query_target_face_neighborhood(p, vol, *trajectory);
}

retQSS::CrossedFaceData
retQSS::FaceCrossingAlgorithm::query_all_faces(
		Internal_ParticleID p,
		retQSS::Volume *vol,
		const retQSS::Trajectory &trajectory)
{
	Point_3 exit_point;

	double
		delta,
		entry_time = this->tracker->entry_time(p),
		exit_time = this->tracker->infinity(),
		crossing_tolerance = this->tracker->crossing_tolerance(p);

	retQSS::PolyhedronFace
			*entry_face = this->tracker->entry_face(p),
			*exit_face = nullptr;

	for(auto face : vol->faces())
	{
		auto intersection_data =
				this->locator->intersection_of(trajectory, *face);

		if(intersection_data.is_valid())
		{
			delta = intersection_data.time() - entry_time;

			// Dismiss boundary crossings that:
			//  * Happen in the same face used to enter the volume,
			//  * in a very short time after entering the volume, and
			//  * for which the direction of the particle points towards the
			//    interior of the volume.
			// This condition can occasionally occur due to numerical
			// errors.
			if((entry_face == nullptr || entry_face == face) &&
				delta < crossing_tolerance)
			{
				auto vel = this->tracker->current_velocity(p);
				if(!vol->is_exit_direction(face, vel))
					continue;
			}

			if(intersection_data.time() < exit_time)
			{
				exit_time = intersection_data.time();
				exit_point = intersection_data.point();
				exit_face = face;
			}
		}
	}

	return std::make_tuple(exit_time, exit_face, exit_point);
}

retQSS::CrossedFaceData
retQSS::FaceCrossingAlgorithm::query_target_face_neighborhood(
		Internal_ParticleID p,
		retQSS::Volume *vol,
		const retQSS::Trajectory &trajectory)
{
	Point_3 exit_point;

	double exit_time = this->tracker->infinity();

	retQSS::PolyhedronFace
			*entry_face = this->tracker->entry_face(p),
			*exit_face = nullptr;

	// Find face crossed by linear segment.
	auto pos = this->tracker->current_position(p);
	auto vel = this->tracker->current_velocity(p);

	Ray_3 ray(pos, vel);

	auto face_and_point = vol->crossed_face(ray);
	retQSS::PolyhedronFace *face = std::get<0>(face_and_point);

	if(
		face != nullptr &&
			((entry_face != nullptr && entry_face != face) ||
			(vol->is_exit_direction(face, vel))))
	{
		for(auto neighbor: vol->face_neighborhood(face))
		{
			auto intersection_data = this->locator->intersection_of(
										trajectory,
										*neighbor);

			if(intersection_data.is_valid())
			{
				exit_time = intersection_data.time();
				exit_point = intersection_data.point();
				exit_face = neighbor;
				break;
			}
		}
	}

	return std::make_tuple(exit_time, exit_face, exit_point);
}

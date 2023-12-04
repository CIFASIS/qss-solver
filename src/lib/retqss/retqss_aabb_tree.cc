#include "retqss_aabb_tree.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_exceptions.hh"


retQSS::AABBTreeForSinglePolyhedron::AABBTreeForSinglePolyhedron(
		retQSS::Polyhedron *poly) :
		poly(poly),
		locator(nullptr)
{
	this->tree.insert(
			CGAL::faces(*this->poly).first,
			CGAL::faces(*this->poly).second,
			*this->poly);
}

void retQSS::AABBTree::check_query_result(void *ptr) const
{
	if(ptr == nullptr)
	{
		std::stringstream ss_msg;
		ss_msg
			<< "CGAL intersection query returned null pointer!\n"
			<< "This is possibly due to a bug in the CGAL library. "
			<< "Simulation cannot continue for safety reasons.\n";

		throw retQSS::Exception(ss_msg.str());
	}
}

retQSS::FaceAndPoint retQSS::AABBTreeForSinglePolyhedron::crossed_face(
		const Ray_3 &ray,
		retQSS::PolyhedronFace*)
{
	PolyhedronFace *face = nullptr;
	Point_3 point;

	auto intersection = this->tree.first_intersection(ray);
	if(intersection)
	{
		auto point_ptr = boost::get<Point_3>(&(intersection->first));
		this->check_query_result(point_ptr);
		point = *point_ptr;
		face = this->poly->get_face(intersection->second);
	}

	return std::make_tuple(face, point);
}

double retQSS::AABBTreeForSinglePolyhedron::squared_distance_to(
		const Point_3 &point)
{
	return this->tree.squared_distance(point);
}

retQSS::FaceAndPoint retQSS::AABBTreeForMultiplePolyhedrons::crossed_face(
		const Ray_3 &ray,
		retQSS::PolyhedronFace *face_to_skip)
{
	PolyhedronFace *face = nullptr;
	Point_3 point;
	Ray_intersection_for_custom_tree intersection;

	if(face_to_skip == nullptr)
		intersection = this->tree.first_intersection(ray);
	else
		intersection = this->tree.first_intersection(
							ray,
							retQSS::FaceSkippingFunctor(face_to_skip));

	if(intersection)
	{
		auto point_ptr = boost::get<Point_3>(&(intersection->first));
		this->check_query_result(point_ptr);
		point = *point_ptr;

		FaceIdx face_idx = intersection->second.first;
		retQSS::Polyhedron *poly = const_cast<retQSS::Polyhedron*>(intersection->second.second);
		face = poly->get_face(face_idx);
	}

	return std::make_tuple(face, point);
}

double retQSS::AABBTreeForMultiplePolyhedrons::squared_distance_to(
		const Point_3 &point)
{
	return this->tree.squared_distance(point);
}

void retQSS::AABBTreeForMultiplePolyhedrons::insert(
		retQSS::PolyhedronFace *face)
{
	auto poly = face->polyhedron();

	for(auto subface_idx : face->subfaces())
	{
		FaceIdx copy(subface_idx);
		copy++;

		this->tree.insert(subface_idx, copy, *poly);
	}
}

double retQSS::AABBTree::distance_to(const Point_3 &point)
{
	return std::sqrt(this->squared_distance_to(point));
}

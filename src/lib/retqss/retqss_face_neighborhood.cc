#include "retqss_face_neighborhood.hh"
#include "retqss_volume.hh"


retQSS::FaceNeighborhood::FaceNeighborhood(
		retQSS::Volume *vol,
		retQSS::PolyhedronFace *face,
		int depth) :vol(vol), face(face), depth(depth)
{
}

retQSS::FaceNeighborhood::iterator
retQSS::FaceNeighborhood::begin()
{
	return retQSS::FaceNeighborhood::iterator(this, this->depth);
}

retQSS::FaceNeighborhood::iterator
retQSS::FaceNeighborhood::end()
{
	return retQSS::FaceNeighborhood::iterator();
}

const retQSS::FaceNeighbors&
retQSS::FaceNeighborhood::get_face_neighbors(retQSS::PolyhedronFace *face) const
{
	return this->vol->get_face_neighbors(face);
}

retQSS::FaceNeighborhoodIterator::FaceNeighborhoodIterator() :
		current_face(nullptr),
		face_depth(0),
		depth(0),
		ended(true)
{
}

retQSS::FaceNeighborhoodIterator::FaceNeighborhoodIterator(
		retQSS::FaceNeighborhood *owner,
		int depth) :
		owner(owner),
		current_face(owner->face),
		face_depth(0),
		depth(depth),
		ended(false)
{
	this->faces.push({this->current_face, this->face_depth});
	this->seen.insert(this->current_face);
}

retQSS::PolyhedronFace *retQSS::FaceNeighborhoodIterator::operator*()
{
	return this->current_face;
}

// TODO: address possible mismatch between face neighborhood computed
// with CGAL and its visualization.
const retQSS::FaceNeighborhoodIterator
	&retQSS::FaceNeighborhoodIterator::operator++()
{
	if(this->ended)
		return *this;

	for(auto neighbor : this->owner->get_face_neighbors(this->current_face))
	{
		if(	this->face_depth < this->depth &&
			this->seen.find(neighbor) == this->seen.end())
		{
			this->faces.push({neighbor, this->face_depth+1});
			this->seen.insert(neighbor);
		}
	}

	this->faces.pop();

	if(this->faces.empty())
	{
		this->ended = true;
		this->current_face = nullptr;
	}
	else
	{
		this->current_face = std::get<0>(this->faces.front());
		this->face_depth = std::get<1>(this->faces.front());
	}

	return *this;
}

bool retQSS::FaceNeighborhoodIterator::operator!=(
		const retQSS::FaceNeighborhoodIterator& iter) const
{
	return
			this->ended != iter.ended ||
			this->current_face != iter.current_face;
}

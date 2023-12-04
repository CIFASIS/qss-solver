#include <algorithm>
#include <fstream>
#include <math.h>

#include "retqss_volume.hh"
#include "retqss_polyhedral_volume.hh"


retQSS::Volume::Volume(
		VolumeID v,
		const std::string &name) :
		retQSS::Object(v, name),
		tree(nullptr)
{
}

retQSS::Volume::~Volume()
{
}

void retQSS::Volume::check_neighbor(retQSS::Volume *vol)
{
	for(auto face1 : this->_faces)
		for(auto face2 : vol->_faces)
			if(*face1 == *face2)
				this->add_neighbor(face1, face2);
}

void retQSS::Volume::add_neighbor(
		retQSS::PolyhedronFace *face1,
		retQSS::PolyhedronFace *face2,
		retQSS::Volume *vol)
{
	if(vol == nullptr)
		vol = face2->volume();

	if(this->face_to_face.find(face1) == this->face_to_face.end())
		this->face_to_face[face1] = FaceNeighbors();
	this->face_to_face[face1].push_back(face2);

	if(vol->face_to_face.find(face2) == vol->face_to_face.end())
		vol->face_to_face[face2] = FaceNeighbors();
	vol->face_to_face[face2].push_back(face1);

	if(std::find(
			this->adjacent_volumes.begin(),
			this->adjacent_volumes.end(),
			vol) == this->adjacent_volumes.end())
		this->adjacent_volumes.push_back(vol);

	if(std::find(
			vol->adjacent_volumes.begin(),
			vol->adjacent_volumes.end(),
			this) == vol->adjacent_volumes.end())
		vol->adjacent_volumes.push_back(this);
}

retQSS::VolumeArray retQSS::Volume::get_volumes_adjacent_to(
		retQSS::PolyhedronFace *face) const
{
	retQSS::VolumeArray vols;
	auto it = this->face_to_face.find(face);

	if(it != this->face_to_face.end())
		for(auto neighbor : it->second)
			vols.push_back(neighbor->volume());

	return vols;
}

bool retQSS::Volume::is_polyhedral() const
{
	return false;
}

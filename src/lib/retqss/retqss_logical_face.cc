#include "retqss_logical_face.hh"


retQSS::LogicalFace::LogicalFace(
	Internal_LogicalFaceID f,
	retQSS::PolyhedronFace *primary_face)
	: retQSS::Object(f, "Logical face"),
	  _primary_face(primary_face)
{
	assert(this->_primary_face != nullptr);
	assert(this->_primary_face->_logical_face == nullptr);
	this->_primary_face->_logical_face = this;
}

void retQSS::LogicalFace::add_physical_face(retQSS::PolyhedronFace *face)
{
	this->_secondary_faces.push_back(face);
	assert(face->_logical_face == nullptr || face->_logical_face == this);
	face->_logical_face = this;
}

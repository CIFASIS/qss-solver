#include "retqss_exceptions.hh"
#include "retqss_object.hh"
#include "retqss_polyhedral_volume.hh"


retQSS::UndefinedPropertyException::UndefinedPropertyException(
	const retQSS::Object *obj,
	const std::string &tag)
{
	std::stringstream ss_msg;
	ss_msg
		<< "Property " << tag
		<< " not found! (object: " << obj->get_name()
		<< " - internal ID " << obj->get_ID()
		<< ")"
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::InvalidObjectIDException::InvalidObjectIDException(
	retQSS::ObjectID id,
	const std::string &type)
{
	std::stringstream ss_msg;
	ss_msg
		<< "Invalid " << type
		<< " ID: " << id
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::InvalidIndexException::InvalidIndexException(
	const retQSS::Object *obj,
	retQSS::Internal_Index i,
	const std::string &sequence)
{
	std::stringstream ss_msg;
	ss_msg
		<< "Invalid index " << i
		<< " when attempting to access " << sequence
		<< " (object "
		<< obj->get_name()
		<< " - ID "
		<< obj->get_ID() << ")"
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::UnsupportedMethodException::UnsupportedMethodException(
	int order)
{
	std::stringstream ss_msg;
	ss_msg
		<< "Unsupported QSS method of order "
		<< order
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::UnsupportedPartitioningMethodException::
	UnsupportedPartitioningMethodException()
{
	std::stringstream ss_msg;
	ss_msg
		<< "Unsupported partitioning method: "
		<< "only manual partitioning supported"
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::CGALTriangulationException::CGALTriangulationException(
	retQSS::Polyhedron *poly)
{
	std::stringstream ss_msg;
	ss_msg
		<< "CGAL triangulation failed: polyhedron supporting volume "
		<< poly->volume()->get_name()
		<< " (ID " << poly->volume()->get_ID()
		<< ") has at least one face that cannot be triangulated"
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::UnsupportedPolygonalFaceException::UnsupportedPolygonalFaceException(
		const retQSS::VertexArray &vertices)
{
	std::stringstream ss_msg;
	ss_msg
		<< "Polygonal faces with "
		<< vertices.size()
		<< " vertices are not supported"
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::FunctionNotFoundException::FunctionNotFoundException(
		const std::string &function)
{
	std::stringstream ss_msg;
	ss_msg
		<< "Function "
		<< function
		<< " not found"
		<< std::endl;

	this->msg = ss_msg.str();
}

retQSS::FileAccessException::FileAccessException(
		const std::string &filename)
{
	std::stringstream ss_msg;
	ss_msg
		<< "File "
		<< filename
		<< " cannot be accessed"
		<< std::endl;

	this->msg = ss_msg.str();
}

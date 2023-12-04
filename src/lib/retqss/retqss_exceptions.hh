/**
 * \file retqss_exceptions.hh
 */

#ifndef _RETQSS_EXCEPTIONS_H_
#define _RETQSS_EXCEPTIONS_H_

#include <exception>
#include <sstream>
#include <string>

#include "retqss_types.hh"
#include "retqss_polyhedron.hh"


namespace retQSS
{

class Object;


/**
 * \brief Abstract retQSS exception
 *
 * Root class of retQSS exceptions
 */
class Exception : public std::exception
{
protected:
	std::string msg;

public:
	Exception() {};
	Exception(const char *what) : msg(what) {};
	Exception(const std::string &what) : msg(what.c_str()) {};

	virtual ~Exception() {};

    virtual const char* what() const throw()
	{
       return this->msg.c_str();
    }
};

/**
 * \brief Parser exception
 *
 * Exception thrown by geometry parsers when detecting errors in the input
 * files.
 */
class GeometryParserException : public retQSS::Exception
{
public:
	GeometryParserException(const char *what) :
		retQSS::Exception(what) {};
};

/**
 * \brief Undefined property exception
 *
 * Exception thrown when attempting to retrieve undefined properties
 */
class UndefinedPropertyException : public retQSS::Exception
{
public:
	UndefinedPropertyException(
			const retQSS::Object *obj,
			const std::string &tag);
};

/**
 * \brief Invalid object ID exception
 *
 * Exception thrown when attempting to use an invalid object ID
 */
class InvalidObjectIDException : public retQSS::Exception
{
public:
	InvalidObjectIDException(
			retQSS::ObjectID id,
			const std::string &type);
};

/**
 * \brief Invalid index exception
 *
 * Exception thrown when attempting to access a sequence of objects with an
 * invalid index
 */
class InvalidIndexException : public retQSS::Exception
{
public:
	InvalidIndexException(
			const retQSS::Object *obj,
			retQSS::Internal_Index i,
			const std::string &sequence);
};

/**
 * \brief Unsupported QSS method exception
 *
 * Exception thrown when attempting to use an unsupported QSS method
 */
class UnsupportedMethodException : public retQSS::Exception
{
public:
	UnsupportedMethodException(
			int order);
};

/**
 * \brief Unsupported partitioning method exception
 *
 * Exception thrown when attempting to use an unsupported model partitioning
 * method in QSS Solver
 */
class UnsupportedPartitioningMethodException : public retQSS::Exception
{
public:
	UnsupportedPartitioningMethodException();
};

/**
 * \brief CGAL triangulation exception
 *
 * Exception thrown when CGAL cannot triangulate a polyhedron
 */
class CGALTriangulationException : public retQSS::Exception
{
public:
	CGALTriangulationException(
			retQSS::Polyhedron *poly);
};

/**
 * \brief Unsupported polygonal face exception
 *
 * Exception thrown when an unsupported polygonal face is found in the geometry
 */
class UnsupportedPolygonalFaceException : public retQSS::Exception
{
public:
	UnsupportedPolygonalFaceException(
			const retQSS::VertexArray &vertices);
};

/**
 * \brief Function not found exception
 *
 * Exception thrown when a function to execute in neighborhoods is not found
 */
class FunctionNotFoundException : public retQSS::Exception
{
public:
	FunctionNotFoundException(
			const std::string &function);
};

/**
 * \brief File access exception
 *
 * Exception thrown when a file cannot be accessed
 */
class FileAccessException : public retQSS::Exception
{
public:
	FileAccessException(
			const std::string &filename);
};

}

#endif

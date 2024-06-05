/**
 * \file retqss_types.hh
 * \brief Assorted type definitions
 */

#ifndef _RETQSS_TYPES_H_
#define _RETQSS_TYPES_H_

/**
 * \brief Particle ID (user visibility).
 *
 * Public type to uniquely identify particles, whose IDs range from
 * 1 to \f$\NPart\f$.
 */
typedef unsigned int ParticleID;

/**
 * \brief Logical face ID (user visibility).
 *
 * Public type to uniquely identify logical 2D faces, whose IDs range
 * from 1 to \f$\NFace\f$.
 */
typedef unsigned int LogicalFaceID;

/**
 * \brief Volume ID (user visibility).
 *
 * Public type to uniquely identify volumes. Polyhedral volume IDs range
 * from 1 to \f$\NVol\f$. ID 0 is reserved for a special type of volume
 * representing the open exterior of the geometry.
 */
typedef unsigned int VolumeID;

/**
 * \brief Vertex ID (user visibility).
 *
 * Public type to uniquely identify vertices, whose IDs range from
 * from 1 to \f$\NVert\f$.
 */
typedef unsigned int VertexID;

/**
 * \brief Index into a sequence of objects (user visibility).
 *
 * Public type to represent indices into sequence of objects. Since an index is
 * publicly visible, this type is compatible with Modelica indexation (i.e., a
 * valid index ranges from 1 to \f$n\f$, where \f$n\f$ is the length of the
 * underlying sequence).
 */
typedef unsigned int Index;

/**
 * \brief Property names (user visibility).
 *
 * Public type to represent names of arbitrary properties.
 */
typedef char const* PropertyTag;

/**
 * \brief Property values (user visibility).
 *
 * Public type to represent values of arbitrary properties.
 */
typedef double PropertyValue;

/**
 * \brief Boolean values (user visibility).
 *
 * Public type to represent boolean values.
 */
typedef char Bool;

#ifdef __cplusplus

#include <mutex>
#include <list>
#include <list>
#include <vector>
#include <tuple>
#include <utility>
#include <unordered_set>

#include "retqss_cgal_main_types.hh"


/**
 * \namespace retQSS Main retQSS namespace
 */
namespace retQSS
{

class Volume;
class PolyhedralVolume;
class Vertex;
class LogicalFace;
class PolyhedronFace;
class VolumeNeighbor;
class ParticleNeighbor;
class ParticleNeighborhood;

/**
 * \brief Initial condition of particles
 *
 * An initial condition of a particle, composed of a position \f$(x,y,z)\f$ and
 * a velocity \f$(v_x, v_y, v_z)\f$.
 */
struct InitialCondition
{
	double x, y, z;
	double vx, vy, vz;
	VolumeID volumeID = 0;
};

/**
 * \brief Object ID (internal visibility).
 *
 * Internal type to identify abstract retQSS objects.
 */
typedef unsigned int ObjectID;

/**
 * \brief Particle ID (internal visibility).
 *
 * Internal type to uniquely identify particles, whose IDs range from
 * 0 to \f$\NPart-1\f$.
 */
typedef ParticleID Internal_ParticleID;

/**
 * \brief Logical face ID (internal visibility).
 *
 * Internal type to uniquely identify logical 2D faces, whose IDs range
 * from 0 to \f$\NFace-1\f$.
 */
typedef LogicalFaceID Internal_LogicalFaceID;

/**
 * \brief Vertex ID (internal visibility).
 *
 * Internal type to uniquely identify vertices, whose IDs range
 * from 0 to \f$\NVert-1\f$.
 */
typedef VertexID Internal_VertexID;

/**
 * \brief Index into a sequence of objects (internal visibility).
 *
 * Internal type to represent indices into sequence of objects. These indices
 * range from 0 to \f$n-1\f$, where \f$n\f$ is the length of the underlying
 * sequence.
 */
typedef Index Internal_Index;

/**
 * Array of volumes
 */
typedef std::vector<retQSS::Volume*> VolumeArray;

/**
 * Array of polyhedral volumes
 */
typedef std::vector<retQSS::PolyhedralVolume*> PolyhedralVolumeArray;

/**
 * Array of volume neighbors (computed by volume neighborhood)
 */
typedef std::vector<retQSS::VolumeNeighbor*> VolumeNeighborArray;

/**
 * Array of vertices
 */
typedef std::vector<retQSS::Vertex*> VertexArray;

/**
 * Array of logical faces
 */
typedef std::vector<retQSS::LogicalFace*> LogicalFaceArray;

/**
 * Set of particle IDs
 */
typedef std::unordered_set<retQSS::Internal_ParticleID> ParticleIDSet;

/**
 * List of volumes traversed by a particle with entry time (debug)
 */
typedef std::list<std::pair<double, retQSS::Volume*>> TraversedVolumes;

/**
 * List of points (debug)
 */
typedef std::list<std::pair<double,Point_3>> PointList;

/**
 * Trajectories (sequence of lists of points) (debug)
 */
typedef std::list<retQSS::PointList*> PointLists;

/**
 * Arrangement of a polyhedron face and a point. Used as output of the
 * intersection locator (see retQSS::IntersectionLocator).
 */
typedef std::tuple<retQSS::PolyhedronFace*, Point_3> FaceAndPoint;

/**
 * Array of particle initial conditions.
 */
typedef std::vector<retQSS::InitialCondition> InitialConditionArray;

/**
 * Arrangement of time, a polyhedron face and a point. Used as output of the
 * face crossing algorithm (see retQSS::FaceCrossingAlgorithm).
 */
typedef std::tuple<double, retQSS::PolyhedronFace*, Point_3> CrossedFaceData;

/**
 * Lock (for parallel simulations).
 */
typedef std::unique_lock<std::mutex> Lock;

/*
 * Function type for particle neighborhood iteration.
 * \sa retQSS_particleNeighborhood_forEachParticle
 */
typedef bool (*FuncWithoutArgs)(retQSS::ParticleNeighbor*,Vector_3&);

/*
 * Function type for particle neighborhood iteration.
 * \sa retQSS_particleNeighborhood_forEachParticle_1,
 * retQSS_particleNeighborhood_forEachParticle_2
 */
typedef bool (*FuncWithArgs)(
				retQSS::ParticleNeighbor*,
				const std::vector<double>&,
				Vector_3&);

/*
 * Function type for particle neighborhood iteration.
 * \sa retQSS_particleNeighborhood_forEachParticle_1,
 * retQSS_particleNeighborhood_forEachParticle_2
 */
typedef Vector_3 (*NeighborhoodFuncWith1Arg)(
				retQSS::ParticleNeighborhood*,
				double);

/*
 * Function type for particle neighborhood iteration.
 * \sa retQSS_particleNeighborhood_forEachParticle_1,
 * retQSS_particleNeighborhood_forEachParticle_2
 */
typedef Vector_3 (*NeighborhoodFuncWith2Args)(
				retQSS::ParticleNeighborhood*,
				double, double);

/**
 * \brief Location of points w.r.t. volumes
 *
 * Internal type to represent location of points in volumes.
 */
typedef enum
{
	POINT_INSIDE,
	POINT_OUTSIDE,
	POINT_ON_BOUNDARY
} PointLocation;

/**
 * \brief VTK cell types
 *
 * Enumeration of all cell types defined in the VTK file format specification.
 */
typedef enum
{
	VTK_VERTEX=1,
	VTK_POLY_VERTEX,
	VTK_LINE,
	VTK_POLY_LINE,
	VTK_TRIANGLE,
	VTK_TRIANGLE_STRIP,
	VTK_POLYGON,
	VTK_PIXEL,
	VTK_QUAD,
	VTK_TETRA,
	VTK_VOXEL,
	VTK_HEXAHEDRON,
	VTK_WEDGE,
	VTK_PYRAMID
} VTKCellType;

}

#endif

#endif

/**
 * \file retqss_interface.hh
 */

#ifndef _RETQSS_INTERFACE_H_
#define _RETQSS_INTERFACE_H_

#include <memory>

#include "retqss_cgal_main_types.hh"
#include "retqss_particle_tracker.hh"
#include "retqss_trajectory.hh"
#include "retqss_types.hh"
#include "retqss_geometry.hh"
#include "retqss_volume.hh"
#include "retqss_volume_neighborhood.hh"
#include "retqss_particle_neighborhood.hh"
#include "retqss_object.hh"
#include "retqss_face_crossing_algorithm.hh"
#include "retqss_simulator_interface.hh"
#include "retqss_stats.hh"

#define RETQSS()	(retQSS::Interface::instance())


namespace retQSS
{

/**
 * \brief Interface to retQSS
 *
 * This class provides an interface to access retQSS from an arbitrary
 * simulation engine. It is the middle layer that mediates access to retQSS
 * objects from queries performed by models during their simulation. Indeed,
 * queries exported in the Modelica retQSS package have their actual
 * implementation in this interface.
 */
class Interface
{
private:
	static retQSS::Interface *interface; //!< Single instance of the interface.

	retQSS::Geometry *_geometry;
	retQSS::ParticleTracker *_tracker;
	retQSS::VolumeNeighborhood *_neighborhood;
	retQSS::FaceCrossingAlgorithm *_crossing_algorithm;
#ifdef RETQSS_STATS
	retQSS::Stats *_stats;
#endif

	bool geometry_created;  //!< Indicates if geometry was already created.
	bool particles_created; //!< Indicates if particles were already created.

	// ...
	double current_x, current_y, current_z;

	/**
	 * External particle iteration functions already called
	 */
	std::unordered_map<const char*, void*> functions_called;

	/**
	 * \brief Create internal objects that depend on the number of particles
	 */
	void create_particle_dependent_objects(int n_particles);

	/**
	 * \brief Retrieve initial particle conditions from the simulation engine
	 */
	retQSS::InitialConditionArray *get_initial_conditions(int Np);

	/**
	 * \brief Check if point is inside current volume of a particle
	 *
	 * Check if point \f$(x,y,z)\f$ is inside the current volume of particle
	 * \f$p\f$, \f$\Vol{p}\f$.
	 */
	bool point_in_current_volume(
			Internal_ParticleID p,
			double x, double y, double z);

	/**
	 * \brief Retrieve normal vector of last crossed face
	 *
	 * Retrive the outward normal vector of the last crossed face by particle
	 * \f$p\f$.
	 */
	Vector_3 get_crossed_face_normal(Internal_ParticleID p);

	void set_output_from(const Point_3&, double*, double*, double*);
	void set_output_from(const Vector_3&, double*, double*, double*);
	void set_output_from(const std::vector<double>&, double*, double*, double*);

	/**
	 * \brief Get property value as a 3D vector
	 */
	bool get_object_property_vector(
			retQSS::Object*,
			PropertyTag,
			double*, double*, double*);

	/**
	 * \brief Retrieve particle neighborhood
	 */
	retQSS::ParticleNeighborhood *get_particle_neighborhood(
			Internal_ParticleID p);

	void *retrieve_function(const char*);

public:
	Interface();
	~Interface();

	bool geometry_set_up(const char*);
	/**
         * \brief Sets up the geomtry as a 3D grid with a given cell length
         *
         * \sa retQSS_geometry_gridSetUp
         */
	bool geometry_grid_set_up(int,int,int,double);
	bool particle_set_up(int,const char*,const char*);
	bool full_setup(int,const char*,const char*,const char*);
	void abort();

	/**
	 * \brief Compute time of next volume boundary crossing.
	 *
	 * \sa retQSS_particle_nextCrossingTime
	 */
	double particle_next_crossing_time(Internal_ParticleID);

	/**
	 * \brief Get number of polyhedral volumes in geometry.
	 *
	 * \sa retQSS_geometry_countVolumes
	 */
	int geometry_count_volumes();

	/**
	 * \brief Get number of vertices in geometry.
	 *
	 * \sa retQSS_geometry_countVertices
	 */
	int geometry_count_vertices();

	/**
	 * \brief Get number of logical faces in geometry.
	 *
	 * \sa retQSS_geometry_countFaces
	 */
	int geometry_count_faces();

	/**
	 * \brief Compute a random polyhedral volume ID.
	 *
	 * \sa retQSS_geometry_randomVolumeID
	 */
	VolumeID geometry_random_volume_ID();

	/**
	 * \brief Retrieve ID of the current volume of a particle.
	 *
	 * \sa retQSS_particle_currentVolumeID
	 */
	VolumeID particle_current_volume_ID(Internal_ParticleID);

	/**
	 * \brief Retrieve ID of the previous volume visited by a particle.
	 *
	 * \sa retQSS_particle_previousVolumeID
	 */
	VolumeID particle_previous_volume_ID(Internal_ParticleID);

	/**
	 * \brief Retrieve ID of the volume of the next scheduled boundary
	 * crossing
	 *
	 * \sa retQSS_particle_nextVolumeID
	 */
	VolumeID particle_next_volume_ID(Internal_ParticleID);

	/**
	 * \brief Retrive ID of the last face crossed
	 *
	 * \sa retQSS_particle_crossedFaceID
	 */
	Internal_LogicalFaceID particle_crossed_face_ID(Internal_ParticleID);

	/**
	 * \brief Check if volume is on the geometry boundary.
	 *
	 * \sa retQSS_volume_isOnBoundary
	 */
	bool volume_is_on_boundary(VolumeID);

	/**
	 * \brief Compute number of particles inside a volume.
	 *
	 * \sa retQSS_volume_countParticlesInside
	 */
	int volume_count_particles_inside(VolumeID);

	/**
	 * \brief Retrieve the ID of a particle inside a volume.
	 *
	 * \sa retQSS_volume_IDOfParticleInside
	 */
	Internal_ParticleID volume_ID_of_particle_inside(VolumeID, Internal_Index);

	/**
	 * \brief Compute distance from a volume to a point.
	 *
	 * \sa retQSS_volume_distanceToPoint
	 */
	double volume_distance_to_point(VolumeID, const Point_3&);

	/**
	 * \brief Compute capacity of a volume.
	 *
	 * \sa retQSS_volume_capacity
	 */
	double volume_capacity(VolumeID);

	/**
	 * \brief Compute centroid of a volume.
	 *
	 * \sa retQSS_volume_centroid
	 */
	void volume_centroid(
			VolumeID,
			int*, double*, double*, double*);

	/**
	 * \brief Count number of vertices in a volume.
	 *
	 * \sa retQSS_volume_countVertices
	 */
	int volume_count_vertices(VolumeID);

	/**
	 * \brief Count number of faces in a volume.
	 *
	 * \sa retQSS_volume_countFaces
	 */
	int volume_count_faces(VolumeID);

	/**
	 * \brief Retrieve the ID of a vertex in a volume.
	 *
	 * \sa retQSS_volume_vertexID
	 */
	Internal_VertexID volume_vertex_ID(VolumeID, Internal_Index);

	/**
	 * \brief Retrieve the ID of a face in a volume.
	 *
	 * \sa retQSS_volume_faceID
	 */
	Internal_LogicalFaceID volume_face_ID(VolumeID, Internal_Index);

	/**
	 * \brief Retrieve value associated with a vertex property tag.
	 *
	 * \sa retQSS_volume_getVertexProperty
	 */
	PropertyValue volume_get_vertex_property(
			VolumeID,
			Internal_Index,
			PropertyTag);

	/**
	 * \brief Set value associated with a vertex property tag.
	 *
	 * \sa retQSS_volume_setVertexProperty
	 */
	PropertyValue volume_set_vertex_property(
			VolumeID,
			Internal_Index,
			PropertyTag,
			PropertyValue);

	/**
	 * \brief Retrieve coordinates of a vertex in a volume.
	 *
	 * \sa retQSS_volume_vertexCoordinates
	 */
	void volume_vertex_coordinates(
			VolumeID,Internal_Index,
			int *rv, double*,double*,double*);

	/**
	 * \brief Compute intersection of a ray in a volume.
	 *
	 * \sa retQSS_volume_rayIntersection
	 */
	void volume_ray_intersection(
			VolumeID, const Ray_3&,
			int*, double*, double*, double*);

	/**
	 * \brief Compute ID of adjacent volume in a given direction
	 *
	 * \sa retQSS_volume_adjacentVolumeID
	 */
	VolumeID volume_adjacent_volume_ID(VolumeID, const Ray_3&);

	/**
	 * \brief Check if property is defined in volume.
	 *
	 * \sa retQSS_volume_hasProperty
	 */
	bool volume_has_property(VolumeID, PropertyTag);

	/**
	 * \brief Retrieve value associated with a property tag.
	 *
	 * \sa retQSS_volume_getProperty
	 */
	PropertyValue volume_get_property(VolumeID, PropertyTag);

	/**
	 * \brief Retrieve vector associated with a property tag.
	 *
	 * \sa retQSS_volume_getPropertyVector
	 */
	void volume_get_property_vector(
			VolumeID,
			PropertyTag,
			int*, double*, double*, double*);

	/**
	 * \brief Set value associated with a property tag.
	 *
	 * \sa retQSS_volume_setProperty
	 */
	PropertyValue volume_set_property(VolumeID, PropertyTag, PropertyValue);

	/**
	 * \brief Set vector associated with a property tag.
	 *
	 * \sa retQSS_volume_setPropertyVector
	 */
	bool volume_set_property_vector(
			VolumeID,
			PropertyTag,
			const Vector_3&);

	/**
	 * \brief Set volume neighborhood computation algorithm to default.
	 *
	 * \sa retQSS_volumeNeighborhood_toDefault
	 */
	bool volume_neighborhood_to_default();

	/**
	 * \brief Set volume neighborhood computation algorithm to file-based.
	 *
	 * \sa retQSS_volumeNeighborhood_toFile
	 */
	bool volume_neighborhood_to_file(const std::string&);

	/**
	 * \brief Set volume neighborhood computation algorithm to radial.
	 *
	 * \sa retQSS_volumeNeighborhood_toRadial
	 */
	bool volume_neighborhood_to_radial(double);

	/**
	 * \brief Set volume neighborhood computation algorithm to periodic radial.
	 *
	 * \sa retQSS_volumeNeighborhood_toPeriodicRadial
	 */
	bool volume_neighborhood_to_periodic_radial(double);

	/**
	 * \brief Set volume neighborhood computation to algorithm vertex-sharing.
	 *
	 * \sa retQSS_volumeNeighborhood_toVertexSharing
	 */
	bool volume_neighborhood_to_vertex_sharing();

	/**
	 * \brief Precompute all volume neighborhoods
	 *
	 * \sa retQSS_volumeNeighborhood_precomputeAll
	 */
	bool volume_neighborhood_precompute_all();

	/**
	 * \brief Compute number of volumes in a volume neighborhood.
	 *
	 * \sa retQSS_volumeNeighborhood_countVolumes
	 */
	int volume_neighborhood_count_volumes(VolumeID);

	/**
	 * \brief Retrieve ID of a volume in a volume neighborhood.
	 *
	 * \sa retQSS_volumeNeighborhood_neighborID
	 */
	VolumeID volume_neighborhood_neighbor_ID(VolumeID, Internal_Index);

	/**
	 * \brief Retrieve a property value in a volume neighbor.
	 *
	 * \sa retQSS_volumeNeighborhood_getNeighborProperty
	 */
	PropertyValue volume_neighborhood_get_neighbor_property(
			VolumeID,
			Internal_Index,
			PropertyTag);

	/**
	 * \brief Set a property in a whole volume neighborhood.
	 *
	 * \sa retQSS_volumeNeighborhood_setProperty
	 */
	PropertyValue volume_neighborhood_set_property(
			VolumeID,
			PropertyTag,
			PropertyValue);

	/**
	 * \brief Count particles in neighborhood.
	 *
	 * \sa retQSS_particleNeighborhood_countParticles
	 */
	int particle_neighborhood_count_particles(Internal_ParticleID);

	/**
	 * \brief Compute average position of neighboring particles.
	 *
	 * \sa retQSS_particleNeighborhood_averagePosition
	 */
	void particle_neighborhood_average_position(
			Internal_ParticleID,
			double*, double*, double*, double*);

	/**
	 * \brief Compute average direction of neighboring particles.
	 *
	 * \sa retQSS_particleNeighborhood_averageVelocity
	 */
	void particle_neighborhood_average_velocity(
			Internal_ParticleID,
			double*, double*, double*, double*);

	/**
	 * \brief Compute repulsive direction of neighboring particles.
	 *
	 * \sa retQSS_particleNeighborhood_repulsiveDirection
	 */
	void particle_neighborhood_repulsive_direction(
			Internal_ParticleID,
			double*, double*, double*, double*);

	/**
	 * \brief Apply a function to each particle in neighborhood.
	 *
	 * \sa retQSS_particleNeighborhood_forEachParticle
	 */
	void particle_neighborhood_for_each_particle(
			Internal_ParticleID,
			const char*,
			const std::vector<double>&,
			double*, double*, double*, double*);

	/**
	 * \brief Evaluate a function over a particle neighborhood.
	 *
	 * \sa retQSS_particleNeighborhood_evaluate_1
	 */
	void particle_neighborhood_evaluate_1(
			Internal_ParticleID,
			const char*,
			double,
			double*, double*, double*, double*);

	/**
	 * \brief Evaluate a function over a particle neighborhood.
	 *
	 * \sa retQSS_particleNeighborhood_evaluate_2
	 */
	void particle_neighborhood_evaluate_2(
			Internal_ParticleID,
			const char*,
			double, double,
			double*, double*, double*, double*);

	/**
	 * \brief Compute centroid of last face crossed.
	 *
	 * \sa retQSS_particle_crossedFaceCentroid
	 */
	void particle_crossed_face_centroid(
			Internal_ParticleID,
			int*, double*, double*, double*);

	/**
	 * \brief Compute normal vector of last face crossed.
	 *
	 * \sa retQSS_particle_crossedFaceNormal
	 */
	void particle_crossed_face_normal(
			Internal_ParticleID,
			int*, double*, double*, double*);

	/**
	 * \brief Retrieve ID of the primary volume of a face
	 *
	 * \sa retQSS_face_primaryVolumeID
	 */
	VolumeID face_primary_volume_ID(Internal_LogicalFaceID);

	/**
	 * \brief Check whether a face is on the geometry surface
	 *
	 * \sa retQSS_face_isOnBoundary
	 */
	Bool face_is_on_boundary(Internal_LogicalFaceID);

	/**
	 * \brief  Retrieve a vector normal to a face
	 *
	 * \sa retQSS_face_normal
	 */
	void face_normal(
			Internal_LogicalFaceID,
			int*, double*, double*, double*);

	/**
	 * \brief  Retrieve the centroid of a face
	 *
	 * \sa retQSS_face_centroid
	 */
	void face_centroid(
			Internal_LogicalFaceID,
			int*, double*, double*, double*);

	/**
	 * \brief Check whether a face is normal to a vector
	 *
	 * \sa retQSS_face_isNormalTo
	 */
	bool face_is_normal_to(
			Internal_LogicalFaceID,
			const Vector_3&);

	/**
	 * \brief Retrieve ID of face in the opposite side of the geometry
	 *
	 * \sa retQSS_face_oppositeFaceID
	 */
	Internal_LogicalFaceID face_opposite_face_ID(Internal_LogicalFaceID);

	/**
	 * \brief Retrieve ID of volume in the opposite side of the geometry
	 *
	 * \sa retQSS_face_oppositeVolumeID
	 */
	VolumeID face_opposite_volume_ID(Internal_LogicalFaceID);

	/**
	 * \brief Check if property is defined in face.
	 *
	 * \sa retQSS_face_hasProperty
	 */
	bool face_has_property(Internal_LogicalFaceID, PropertyTag);

	/**
	 * \brief Retrieve value associated with a property tag.
	 *
	 * \sa retQSS_face_getProperty
	 */
	PropertyValue face_get_property(Internal_LogicalFaceID, PropertyTag);

	/**
	 * \brief Retrieve vector associated with a property tag.
	 *
	 * \sa retQSS_face_getPropertyVector
	 */
	void face_get_property_vector(
			Internal_LogicalFaceID,
			PropertyTag,
			int*, double*, double*, double*);

	/**
	 * \brief Set value associated with a property tag.
	 *
	 * \sa retQSS_face_setProperty
	 */
	PropertyValue face_set_property(
			Internal_LogicalFaceID,
			PropertyTag,
			PropertyValue);

	/**
	 * \brief Set vector associated with a property tag.
	 *
	 * \sa retQSS_face_setPropertyVector
	 */
	bool face_set_property_vector(
			Internal_LogicalFaceID,
			PropertyTag,
			const Vector_3&);

	/**
	 * \brief Check if property is defined in vertex.
	 *
	 * \sa retQSS_vertex_hasProperty
	 */
	bool vertex_has_property(Internal_VertexID, PropertyTag);

	/**
	 * \brief Retrieve value associated with a property tag.
	 *
	 * \sa retQSS_vertex_getProperty
	 */
	PropertyValue vertex_get_property(Internal_VertexID, PropertyTag);

	/**
	 * \brief Retrieve vector associated with a property tag.
	 *
	 * \sa retQSS_vertex_getPropertyVector
	 */
	void vertex_get_property_vector(
			Internal_VertexID,
			PropertyTag,
			int*, double*, double*, double*);

	/**
	 * \brief Set value associated with a property tag.
	 *
	 * \sa retQSS_vertex_setProperty
	 */
	PropertyValue vertex_set_property(
			Internal_VertexID,
			PropertyTag,
			PropertyValue);

	/**
	 * \brief Set vector associated with a property tag.
	 *
	 * \sa retQSS_vertex_setPropertyVector
	 */
	bool vertex_set_property_vector(
			Internal_VertexID,
			PropertyTag,
			const Vector_3&);

	/**
	 * \brief Retrieve vertex coordinates.
	 *
	 * \sa retQSS_vertex_coordinates
	 */
	void vertex_coordinates(
			Internal_VertexID,
			int*, double*, double*, double*);

	/**
	 * \brief Relocate particle in geometry.
	 *
	 * \sa retQSS_particle_relocate
	 */
	VolumeID particle_relocate(
			Internal_ParticleID,
			const Point_3&,
			const Vector_3&);

	/**
	 * \brief Kill particle
	 *
	 * \sa retQSS_particle_kill
	 */
	bool particle_kill(Internal_ParticleID);

	/**
	 * \brief Check whether particle is alive
	 *
	 * \sa retQSS_particle_isAlive
	 */
	bool particle_is_alive(Internal_ParticleID);

	/**
	 * \brief Get particle initial position
	 */
	void particle_initial_position(
			Internal_ParticleID,
			int*, double*, double*, double*);

	/**
	 * \brief Get particle initial velocity
	 */
	void particle_initial_velocity(
			Internal_ParticleID,
			int*, double*, double*, double*);

	/**
	 * \brief Check if property is defined in particle.
	 *
	 * \sa retQSS_particle_hasProperty
	 */
	bool particle_has_property(Internal_ParticleID, PropertyTag);

	/**
	 * \brief Retrieve value associated with a property tag.
	 *
	 * \sa retQSS_particle_getProperty
	 */
	PropertyValue particle_get_property(Internal_ParticleID, PropertyTag);

	/**
	 * \brief Retrieve vector associated with a property tag.
	 *
	 * \sa retQSS_particle_getPropertyVector
	 */
	void particle_get_property_vector(
			Internal_ParticleID,
			PropertyTag,
			int*, double*, double*, double*);

	/**
	 * \brief Set value associated with a property tag.
	 *
	 * \sa retQSS_particle_setProperty
	 */
	PropertyValue particle_set_property(
			Internal_ParticleID,
			PropertyTag,
			PropertyValue);

	/**
	 * \brief Set vector associated with a property tag.
	 *
	 * \sa retQSS_particle_setPropertyVector
	 */
	bool particle_set_property_vector(
			Internal_ParticleID,
			PropertyTag,
			const Vector_3&);

	/**
	 * \brief Compute squared distance between two particles.
	 *
	 * \sa retQSS_particle_squaredDistanceBetween
	 */
	double particle_squared_distance_between(
			Internal_ParticleID,
			Internal_ParticleID);

	/**
	 * \brief Compute angle between two particles.
	 *
	 * \sa retQSS_particle_angleBetween
	 */
	double particle_angle_between(Internal_ParticleID, Internal_ParticleID);

	/**
	 * \brief Make particle bounce following a reflective pattern
	 *
	 * \sa retQSS_particle_reflectiveBounce,
	 * retQSS_particle_reflectiveBounceWithSpeed
	 */
	void particle_reflective_bounce(
			Internal_ParticleID,
			int*, double*, double*, double*,
			double =0.);

	/**
	 * \brief Make particle pass through a periodic boundary
	 *
	 * \sa retQSS_particle_oppositePosition
	 */
	void particle_opposite_position(
			Internal_ParticleID,
			int*, double*, double*, double*);

	/**
	 * \brief Dump particle initial position and velocity to file
	 *
	 * \sa retQSS_particle_dumpInitialConditions
	 */
	bool particle_dump_initial_conditions(
			Internal_ParticleID,
			const std::string&);

	/**
	 * \brief Dump initial position and velocity of all particles to file
	 *
	 * \sa retQSS_particle_dumpAllInitialConditions
	 */
	bool particle_dump_all_initial_conditions(const std::string&);

	/**
	 * \brief TBD
	 *
	 * \sa retQSS_particle_dumpAllCurrentValues
	 */
	bool particle_dump_all_current_values(const std::string&, int);

	/**
	 * \brief Compute vector to particle
	 *
	 * \sa retQSS_particle_vectorTo
	 */
	Vector_3 particle_vector_to(
			Internal_ParticleID p,
			Internal_ParticleID q);

	/**
	 * \brief Compute random point inside volume.
	 *
	 * \sa retQSS_volume_randomPoint
	 */
	void volume_random_point(
			VolumeID,
			int*, double*, double*, double*);

	/**
	 * \brief Set vector.
	 *
	 * \sa retQSS_vector
	 */
	bool vector(double,double,double);

	/**
	 * \brief Set vector with norm.
	 *
	 * \sa retQSS_vectorWithNorm
	 */
	void vector_with_norm(
			double,double,double,
			double,
			int*, double*, double*, double*);

	/**
	 * \brief Set random vector.
	 *
	 * \sa retQSS_randomVector
	 */
	void random_vector(int*, double*, double*, double*);

	/**
	 * \brief Set random vector with norm.
	 *
	 * \sa retQSS_randomVectorWithNorm
	 */
	void random_vector_with_norm(
			double,
			int*, double*, double*, double*);

	/**
	 * \brief Check if particle position is in current volume.
	 *
	 * \sa retQSS_particle_checkPosition
	 */
	bool particle_check_position(Internal_ParticleID);

	/**
	 * \brief Dump geometry surface to file.
	 *
	 * \sa retQSS_geometry_dumpSurface
	 */
	bool geometry_dump_surface(const char*);

	/**
	 * \brief Dump volume to file.
	 *
	 * \sa retQSS_geometry_dumpVolume
	 */
	bool geometry_dump_volume(VolumeID,const char*);

	/**
	 * \brief Dump assorted geometry statistics to file.
	 *
	 * \sa retQSS_geometry_dumpStats
	 */
	bool geometry_dump_stats(const char*);

	/**
	 * \brief Retrieve current particle position.
	 *
	 * \sa retQSS_particle_currentPosition
	 */
	Point_3 particle_current_position(Internal_ParticleID);

	/**
	 * \brief Retrieve current particle velocity.
	 *
	 * \sa retQSS_particle_currentVelocity
	 */
	Vector_3 particle_current_velocity(Internal_ParticleID);

	inline retQSS::Geometry *geometry() const
	{
		return this->_geometry;
	}

	inline retQSS::ParticleTracker *tracker() const
	{
		return this->_tracker;
	}

	inline retQSS::VolumeNeighborhood *neighborhood() const
	{
		return this->_neighborhood;
	}

	inline retQSS::FaceCrossingAlgorithm *crossing_algorithm() const
	{
		return this->_crossing_algorithm;
	}

	/**
	 * \brief Access single retQSS instance
	 *
	 * Create or retrieve the singleton instance of the retQSS interface.
	 */
	static retQSS::Interface *instance();

	/**
	 * \brief Free retQSS resources
	 *
	 * Free the retQSS interface instance and every other retQSS object.
	 */
	static void clear_instance();
};

}

#endif

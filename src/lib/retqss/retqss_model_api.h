/**
 * \file retqss_model_api.h
 * \brief Public retQSS API accessible from Modelica models
 */

#ifndef _RETQSS_MODEL_API_H_
#define _RETQSS_MODEL_API_H_

#include "retqss_types.hh"

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup API Model API
 *  retQSS API
 */

/** \addtogroup Introduction
 *  \ingroup API
 *
 *  - \f$\NPart\f$ is the total number of particles in the model, and their
 *  IDs range from \f$1\f$ to \f$\NPart\f$.
 *  - \f$\NVol\f$ is the total number of polyhedral volumes in the geometry,
 *  and their IDs range from \f$1\f$ to \f$\NVol\f$.
 *  - \f$\NVert\f$ is the total number of vertices in the geometry, and their
 *  IDs range from \f$0\f$ to \f$\NVert-1\f$.
 *  - \f$\NFace\f$ is the total number of logical faces in the geometry, and
 *  their IDs range from \f$0\f$ to \f$\NFace-1\f$.
 *  - The open volume that represents the exterior of the geometry is
 *  referenced by volume ID 0.
 *  - \f$\PartPos{p}\f$ and \f$\PartVel{p}\f$ are the position and velocity,
 *    respectively, of particle \f$p\f$.
 *  - \f$\Vol{p}\f$ is the volume ID where particle \f$p\f$
 *    lies in.
 */

/** \addtogroup Bootstrapping Bootstrapping & error handling
 *  Bootstrapping & error handling
 *  \ingroup API
 * @{
 */

/**
 * \brief Initialize geometry.
 *
 * This function initializes the retQSS geometry from a given file. Geometry
 * formats supported are VTK (unstructured grids with tetrahedral, voxels or
 * hexahedrals 3D cells) and OFF. After calling this function, retQSS
 * is ready to answer geometry-related queries. Note that particles should
 * be initialized separately after.
 *
 * \note Calls retQSS_abort in case of failure. Return value should be ignored.
 *
 * \param geom_filename Path to the geometry file.
 */
Bool retQSS_geometry_setUp(const char *geom_filename);

/**
 * \brief Initialize geometry as a grid.
 *
 * This function initializes the retQSS geometry as even-divided grid with cube cells.
 *
 * \param xCount number of cells in the x axis
 * \param yCount number of cells in the y axis
 * \param zCount number of cells in the z axis
 * \param cellEdgeLength side of the x/y/z cell edge
 * \return Boolean status indicating success.
 */
Bool retQSS_geometry_gridSetUp(int xCount, int yCount, int zCount, double cellEdgeLength);

/**
 * \brief Initialize particles.
 *
 * This function creates and initializes \f$\NPart\f$ particle objects
 * following these steps:
 *  - Initial particle conditions (i.e., position and velocity) are
 *  retrieved from the state variables in the simulation engine.
 *  - For each one of them, a particle is registered in the Particle
 *  Tracker, whose task is to track particles as they travel through
 *  the geometry.
 *  - Finally, particles are located in the geometry.
 *
 * \note Geometry must be initialized before calling this function.
 * \note Calls retQSS_abort in case of failure. Return value should be ignored.
 *
 *  \sa retQSS_geometry_setUp, retQSS::Interface::find_volume_for
 *
 * \param Np Number of particles, \f$\NPart\f$.
 * \param model_name Model name, ignored in non-parallel simulations.
 * Otherwise, it should match the actual Modelica model name so that the
 * partition created is later found by the simulation engine.
 */
Bool retQSS_particle_setUp(int Np, const char *model_name);

/**
 * \brief Initialize particles from file.
 *
 * Initialize particles from user-specified initial conditions. For each
 * particle, six double-precision floating point values are expected,
 * corresponding to the 3D position and the velocity vector, respectively.
 * Once the file is parsed, particles are initialized as in
 * retQSS_particle_setUp().
 *
 * \note Geometry must be initialized before calling this function.
 * \note Calls retQSS_abort in case of failure. Return value should be ignored.
 *
 * \sa retQSS_particle_setUp, retQSS_geometry_setUp
 *
 * \param Np Number of particles, \f$\NPart\f$.
 * \param model_name Model name, ignored in non-parallel simulations.
 * Otherwise, it should match the actual Modelica model name so that the
 * partition created is later found by the simulation engine.
 * \param ic_filename Path to a text file containing the initial positions and
 * velocites for each of the \f$\NPart\f$ particles.
 */
Bool retQSS_particle_setUpFromFile(
		int Np,
		const char *model_name,
		const char *ic_filename);

/**
 * \brief Initialize geometry and particles.
 *
 * Initialize geometry as well as particles. The latter are initialized
 * retrieving their initial conditions from the simulation engine state
 * variables.
 *
 * \sa retQSS_geometry_setUp, retQSS_particle_setUp
 *
 * \note Calls retQSS_abort in case of failure. Return value should be ignored.
 *
 * \param Np Number of particles, \f$\NPart\f$.
 * \param geom_filename Path to the geometry file.
 * \param model_name Model name, ignored in non-parallel simulations.
 * Otherwise, it should match the actual Modelica model name so that the
 * partition created is later found by the simulation engine.
 */
Bool retQSS_fullSetUp(
		int Np,
		const char *geom_filename,
		const char *model_name);

/**
 * \brief Initialize geometry and particles.
 *
 * Initialize geometry as well as particles. The latter are initialized
 * retrieving their initial conditions from a user-specified file.
 *
 * \sa retQSS_geometry_setUp, retQSS_particle_setUpFromFile
 *
 * \note Calls retQSS_abort in case of failure. Return value should be ignored.
 *
 * \param Np Number of particles, \f$\NPart\f$.
 * \param geom_filename Path to the geometry file.
 * \param ic_filename Path to a text file containing the initial positions and
 * velocites for each of the \f$\NPart\f$ particles.
 * \param model_name Model name, ignored in non-parallel simulations.
 * Otherwise, it should match the actual Modelica model name so that the
 * partition created is later found by the simulation engine.
 */
Bool retQSS_fullSetUpWithInitialConditions(
		int Np,
		const char *geom_filename,
		const char *ic_filename,
		const char *model_name);

/**
 * \brief Abort retQSS simulation.
 *
 * Immediately abort the simulation. This function is typically called when
 * unrecoverable situations are detected during the simulation (e.g. failure to
 * read geometry or initializing particles).
 */
Bool retQSS_abort();
/**
 * @}
 */

/** \addtogroup Geometry General geometry queries
 *  General geometry queries
 *  \ingroup API
 * @{
 */

/**
 * \brief Retrieve number of polyehdral volumes.
 *
 * Get the total number of polyhedral volumes in the geometry,
 * \f$\NVol\f$.
 */
int retQSS_geometry_countVolumes();

/**
 * \brief Retrieve number of vertices.
 *
 * Get the total number of different vertices in the geometry, \f$\NVert\f$.
 */
int retQSS_geometry_countVertices();

/**
 * \brief Retrieve number of logical faces.
 *
 * Get the total number of logical in the geometry, \f$\NFace\f$.
 */
int retQSS_geometry_countFaces();

/**
 * \brief Compute a random volume ID.
 *
 * Compute a random integer \f$v\f$ such that \f$1 \leq v \leq \NVol\f$.
 */
VolumeID retQSS_geometry_randomVolumeID();

/**
 * \brief Dump geometry surface to file.
 *
 * Write the geometry surface in VTK format to a file. It creates an
 * unstructured grid from the boundary vertices and the 2D cells corresponding
 * to each face in the geometry boundary.
 *
 * \param filename Path to the output file.
 */
Bool retQSS_geometry_dumpSurface(const char *filename);

/**
 * \brief Dump volume to file.
 *
 * Write a volume in VTK format to a given file. It creates an
 * unstructured grid from the volume vertices and the 2D cells corresponding
 * to each face in the volume.
 *
 * \param filename Path to the output file.
 * \param v ID of the volume to be dumped.
 */
Bool retQSS_geometry_dumpVolume(VolumeID v, const char *filename);

/**
 * \brief Dump assorted geometry statistics to file.
 *
 * Write the following information to a text file:
 *  - Number of polyehdral volumes
 *  - Total number of vertices and number of boundary vertices
 *  - Centroid of the boundary vertices
 *  - Bounding box of the geometry (dimensions and coordinates)
 *  - Total volume capacity, average capacity per volume, minimum and
 *  maximum capacity and the dimensions of the boxes that bound their
 *  corresponding volumes.
 *
 * \param filename Path to the output file.
 */
Bool retQSS_geometry_dumpStats(const char *filename);

/**
 * @}
 */

/** \addtogroup Particle Particle queries
 *  Particle queries
 *  \ingroup API
 * @{
 */

/**
 * \brief Compute time of next volume boundary crossing.
 *
 * Using a polynomial approximation of the particle trajectory, this function
 * finds the first intersection point with one of the facets of \f$\Vol{p}\f$
 * and its corresponding time. This exit information is stored for future
 * reference, and the exit time is returned.
 *
 * \note When current time is greater than the exit time previously
 * computed, the Particle Tracker updates particle \f$p\f$ state so that
 * \f$\Vol{p}\f$ is set to the corresponding adjacent volume of the exit
 * face.
 * \note This function is exposed to Modelica with extra arguments (position
 * and velocity of particle \f$p\f$) so that boundary crossing events are
 * properly rescheduled when the trajectory changes.
 *
 * \sa retQSS::FaceCrossingAlgorithm::find_crossed_face,
 * retQSS::ParticleTracker
 */
double retQSS_particle_nextCrossingTime(ParticleID p);

/**
 * \brief Retrieve ID of the current volume of a particle.
 *
 * Retrieve the ID of the current volume of particle \f$p\f$, \f$\Vol{p}\f$.
 */
VolumeID retQSS_particle_currentVolumeID(ParticleID p);

/**
 * \brief Retrieve ID of the previous volume of a particle.
 *
 * Retrieve the ID of the volume previously visited by particle \f$p\f$
 * before entering \f$\Vol{p}\f$.
 */
VolumeID retQSS_particle_previousVolumeID(ParticleID p);

/**
 * \brief Retrieve ID of the volume of the next scheduled boundary crossing.
 *
 * Retrieve the ID of the volume to be visited next by particle \f$p\f$.
 *
 * \note If next boundary crossing time \f$t = \infty\f$, current volume ID
 * \f$\Vol{p}\f$ will be returned.
 */
VolumeID retQSS_particle_nextVolumeID(ParticleID p);

/**
 * \brief Retrive ID of the last face crossed
 *
 * Retrieve the ID of the last logical face crossed by particle \f$p\f$.
 *
 * \pre Particle \f$p\f$ crossed at least one volume boundary.
 */
LogicalFaceID retQSS_particle_crossedFaceID(ParticleID p);

/**
 * \brief Compute centroid of last face crossed.
 *
 * Compute the centroid of the face through which particle \f$p\f$ entered
 * \f$\Vol{p}\f$.
 *
 * \pre Particle \f$p\f$ crossed at least one volume boundary.
 */
void retQSS_particle_crossedFaceCentroid(
		ParticleID p,
		double *x, double *y, double *z);

/**
 * \brief Compute normal vector of last face crossed.
 *
 * Compute the inward normal vector (with respect to \f$\Vol{p}\f$) of the
 * face through which particle \f$p\f$ entered \f$\Vol{p}\f$.
 *
 * \pre Particle \f$p\f$ crossed at least one volume boundary.
 */
void retQSS_particle_crossedFaceNormal(
		ParticleID p,
		double *x, double *y, double *z);

/**
 * \brief Relocate particle in geometry.
 *
 * Using current position and velocity of particle \f$p\f$, the geometry is
 * explored in order to find the matching volume. In case the position is
 * on a volume boundary, the orientation given by the velocity is used to
 * decide which of the two adjacent volumes should be chosen.
 *
 * \sa retQSS::Interface::find_volume_for
 *
 * \return ID of the new volume.
 */
VolumeID retQSS_particle_relocate(
		ParticleID p,
		double x, double y, double z,
		double vx, double vy, double vz);

/**
 * \brief Kill particle
 *
 * Kill particle \f$p\f$. A dead particle will no longer be included in
 * particle neighborhoods.
 *
 * \note This does not alter or modify the underlying state variables of the
 * particle trajectory.
 */
Bool retQSS_particle_kill(ParticleID p);

/**
 * \brief Check whether particle is alive
 *
 * Check whether particle \f$p\f$ is alive. Only live particles are included in
 * particle neighborhoods.
 */
Bool retQSS_particle_isAlive(ParticleID p);

/**
 * \brief Retrieve current particle position.
 *
 * Retrieve the position of particle \f$p\f$ by means of a backward query to
 * the simulation engine.
 *
 * \note This function is not intended to be used in Modelica models, as
 * particle positions are trivially available there. However, it can be useful
 * for external function code supplied by the user.
 */
void retQSS_particle_currentPosition(
		ParticleID p,
		double *x, double *y, double *z);

/**
 * \brief Retrieve current particle velocity.
 *
 * Retrieve the velocity of particle \f$p\f$ by means of a backward query to
 * the simulation engine.
 *
 * \note This function is not intended to be used in Modelica models, as
 * particle velocities are trivially available there. However, it can be useful
 * for external function code supplied by the user.
 */
void retQSS_particle_currentVelocity(
		ParticleID p,
		double *vx, double *vy, double *vz);

/**
 * \brief Check if property is defined in a particle.
 *
 * Check if a given property is defined in a particle.
 *
 * \return Boolean value indicating whether the property is defined.
 */
Bool retQSS_particle_hasProperty(ParticleID p, PropertyTag tag);

/**
 * \brief Retrieve value associated with a property tag.
 *
 * Retrieve the value associated with a property tag in the
 * given particle.
 *
 * \pre Property must be defined in the object (see
 * retQSS_particle_hasProperty()).
 */
PropertyValue retQSS_particle_getProperty(ParticleID p, PropertyTag tag);

/**
 * \brief Retrieve 3D vector associated with a property tag.
 *
 * Retrieve the 3D vector associated with a property tag in the
 * given particle.
 *
 * \note In case this property was set with a single value, the vector returned
 * will contain three copies of this value.
 *
 * \pre Property must be defined in the object (see
 * retQSS_particle_hasProperty()).
 */
void retQSS_particle_getPropertyVector(
		ParticleID p,
		PropertyTag tag,
		double *x, double *y, double *z);

/**
 * \brief Set value associated with a property tag.
 *
 * Associate a value with a property tag in the given particle.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this new value.
 *
 * \return New property value set.
 */
PropertyValue retQSS_particle_setProperty(
		ParticleID p,
		PropertyTag tag,
		PropertyValue value);

/**
 * \brief Set 3D vector associated with a property tag.
 *
 * Associate a 3D vector with a property tag in the given particle.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this vector.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
Bool retQSS_particle_setPropertyVector(
		ParticleID p,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3);

/**
 * \brief Compute squared distance between two particles.
 *
 * Compute the squared Euclidean distance between particles \f$p_1\f$ and
 * \f$p_2\f$.
 */
double retQSS_particle_squaredDistanceBetween(ParticleID p1, ParticleID p2);

/**
 * \brief Compute angle between two particles.
 *
 * Compute the angle between vectors \f$\vec{u}\f$ and \f$\vec{v}\f$, where:
 *  - \f$\vec{u}\f$ is given by the orientation of particle \f$p_1\f$, and
 *  - \f$\vec{v} = \PartPos{p_2} - \PartPos{p_1}\f$
 *
 *  \return Angle \f$\alpha \in [0, \pi]\f$
 */
double retQSS_particle_angleBetween(ParticleID p1, ParticleID p2);

/**
 * \brief Make particle bounce following a reflective pattern
 *
 * Make particle \f$p\f$ bounce against the last crossed face \f$f\f$,
 * reflected against it. If \f$\vec{n}\f$ is the outward normal vector of
 * \f$f\f$, then \f$p\f$ bounces as described by the following equation:
 * \f[
 * 		\PartVel{p} := \PartVel{p} - 2 \PartVel{p} \cdot \vec{n}
 * \f]
 *
 * \note Should be used immediately after a volume crossing through a
 * reflective face is detected.
 */
void retQSS_particle_reflectiveBounce(
		ParticleID p,
		double *vx, double *vy, double *vz);

/**
 * \brief Make particle bounce following a reflective pattern
 *
 * Make particle \f$p\f$ bounce against the last crossed face \f$f\f$,
 * reflected against it, and setting its speed to a fixed value.
 *
 * \param speed Speed after bouncing
 *
 * \sa retQSS_particle_reflectiveBounce
 */
void retQSS_particle_reflectiveBounceWithSpeed(
		ParticleID p,
		double speed,
		double *vx, double *vy, double *vz);

/**
 * \brief Make particle pass through a periodic boundary
 *
 * Make particle \f$p\f$ pass through a boundary face \f$f\f$ logically
 * connected to a boundary face \f$f'\f$ in the opposite side of the geometry.
 * \f$f'\f$ is the first face on the geometry surface (other than \f$f\f$
 * itself) that is intersected by a ray with direction \f$\vec{n}\f$ (the
 * inward normal vector of \f$f\f$) and origin \f$\PartPos{p}\f$. This
 * function computes such intersection and returns the corresponding
 * position of \f$p\f$ in the opposite side of the geometry.
 *
 * \pre Particle must have crossed a volume boundary
 */
void retQSS_particle_oppositePosition(
		ParticleID p,
		double *x, double *y, double *z);

/**
 * \brief Check if particle position is in current volume.
 *
 * Perform a sanity check on particle \f$p\f$, testing if its current position
 * is indeed contained in \f$\Vol{p}\f$. A warning message is printed to the
 * standard output in case of failure. Also, when compiled in debug mode,
 * the position is added as a new point in the particle trajectory, which will
 * be written to a VTK file when the simulation ends.
 *
 * \note Should be used sparingly or otherwise for debugging purposes, as it
 * might have considerable performance penalties.
 *
 * \return Boolean value indicating success of the test.
 */
Bool retQSS_particle_checkPosition(ParticleID p);

/**
 * \brief Get particle initial position.
 */
void retQSS_particle_initialPosition(
		ParticleID p,
		double *x, double *y, double *z);

/**
 * \brief Get particle initial velocity.
 */
void retQSS_particle_initialVelocity(
		ParticleID p,
		double *vx, double *vy, double *vz);

/**
 * \brief Dump particle initial position and velocity to file
 *
 * Write particle \f$p\f$'s initial position and velocity to a file.
 */
Bool retQSS_particle_dumpInitialConditions(ParticleID p, const char *filename);

/**
 * \brief Dump initial position and velocity of all particles to file
 *
 * Write initial position and velocity of all particles to a file.
 */
Bool retQSS_particle_dumpAllInitialConditions(const char *filename);

/**
 * \brief Dump current position and velocity of all particles to file
 *
 * Write current position and velocity of all particles to a file.
 */
Bool retQSS_particle_dumpAllCurrentValues(const char *filename, int index);
/**
 * @}
 */

#ifdef __cplusplus
}

/**
 * \brief Compute vector to particle
 *
 * Compute 3D vector sourced at $\PartPos{p}$ and pointing to $\PartPos{q}$.
 *
 * \note This is a C++ function that is not exposed to Modelica.
 */
Vector_3 retQSS_particle_vectorTo(ParticleID p, ParticleID q);

extern "C"
{
#endif

/** \addtogroup Face Face queries
 *  Face queries
 *  \ingroup API
 * @{
 */

/**
 * \brief Retrieve ID of the primary volume of a face
 *
 * Retrieve the ID of the primary volume of face \f$f\f$. Recall that a
 * \em logical \em face represents the shared faces among different
 * polyhedrons in the geometry, whereas \em polyhedron \em faces are the actual
 * physical faces of each polyhedron. A logical face has a primary physical
 * face and an optional sequence of secondary physical faces assigned to it.
 * This function retrieves the volume ID that owns the primary phyiscal face.
 *
 * \note When \f$f\f$ is a boundary face, there is only one physical face
 * \f$f'\f$ represented by \f$f\f$, and thus its primary volume will be
 * \f$f'\f$'s volume.
 */
VolumeID retQSS_face_primaryVolumeID(LogicalFaceID f);

/**
 * \brief Check whether a face is on the geometry surface
 *
 * Check whether face \f$f\f$ is on the geometry surface.
 */
Bool retQSS_face_isOnBoundary(LogicalFaceID f);

/**
 * \brief Retrieve a vector normal to a face
 *
 * Retrieve a vector \f$\vec{n}\f$ that is normal to face \f$f\f$.
 *
 * \note Since \f$f\f$ is a logical face, shared by possibly different
 * polyhedrons, \f$\vec{n}\f$ has no inward or outward orientation.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
void retQSS_face_normal(
		LogicalFaceID f,
		double *x, double *y, double *z);

/**
 * \brief Retrieve the centroid of a face
 *
 * Retrieve the centroid of face \f$f\f$.
 */
void retQSS_face_centroid(
		LogicalFaceID f,
		double *x, double *y, double *z);

/**
 * \brief Check whether a face is normal to a vector
 *
 * Check whether face \f$f\f$ is normal to vector
 * \f$\vec{n} = (n_x, n_y, n_z)\f$.
 */
Bool retQSS_face_isNormalTo(
		LogicalFaceID f,
		double nx, double ny, double nz);

/**
 * \brief Retrieve ID of face in the opposite side of the geometry
 *
 * Retrieve the ID of the face \f$f'\f$ that is in the opposite side of the
 * geometry with respect to face \f$f\f$. Face \f$f'\f$ is the first face on
 * the geometry surface (other than \f$f\f$ itself) that is intersected by a
 * ray with direction \f$\vec{n}\f$ (the inward normal vector of \f$f\f$) and
 * origin inside \f$f\f$ (e.g., its centroid).
 *
 * \pre \f$f$ must be a boundary face
 */
LogicalFaceID retQSS_face_oppositeFaceID(LogicalFaceID f);

/**
 * \brief Retrieve ID of volume in the opposite side of the geometry
 *
 * Retrieve the ID of the volume that owns the face \f$f'\f$ that is in the
 * opposite side of the geometry with respect to face \f$f\f$.
 *
 * \pre \f$f$ must be a boundary face
 *
 * \sa retQSS_face_oppositeFaceID
 */
VolumeID retQSS_face_oppositeVolumeID(LogicalFaceID f);

/**
 * \brief Check if property is defined in a face.
 *
 * Check if a given property is defined in a face.
 *
 * \return Boolean value indicating whether the property is defined.
 */
Bool retQSS_face_hasProperty(LogicalFaceID f, PropertyTag tag);

/**
 * \brief Retrieve value associated with a property tag.
 *
 * Retrieve the value associated with a property tag in the
 * given face.
 *
 * \pre Property must be defined in the object (see
 * retQSS_face_hasProperty()).
 */
PropertyValue retQSS_face_getProperty(LogicalFaceID f, PropertyTag tag);

/**
 * \brief Retrieve 3D vector associated with a property tag.
 *
 * Retrieve the 3D vector associated with a property tag in the
 * given face.
 *
 * \note In case this property was set with a single value, the vector returned
 * will contain three copies of this value.
 *
 * \pre Property must be defined in the object (see
 * retQSS_particle_hasProperty()).
 *
* \return Boolean status indicating success (can be safely ignored).
 */
void retQSS_face_getPropertyVector(
		LogicalFaceID f,
		PropertyTag tag,
		double *x, double *y, double *z);

/**
 * \brief Set value associated with a property tag.
 *
 * Associate a value with a property tag in the given face.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this new value.
 *
 * \return Property value set.
 */
PropertyValue retQSS_face_setProperty(
		LogicalFaceID f,
		PropertyTag tag,
		PropertyValue value);

/**
 * \brief Set 3D vector associated with a property tag.
 *
 * Associate a 3D vector with a property tag in the given face.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this vector.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
Bool retQSS_face_setPropertyVector(
		LogicalFaceID f,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3);

/**
 * @}
 */

/** \addtogroup Volume Volume queries
 *  Volume queries
 *  \ingroup API
 * @{
 */

/**
 * \brief Check if volume is on the geometry boundary.
 *
 * Check if at least one of the facets of volume \f$v\f$ is on the geometry
 * surface (i.e., such facet leads to volume 0).
 */
Bool retQSS_volume_isOnBoundary(VolumeID v);

/**
 * \brief Compute number of particles inside a volume.
 *
 * Get number of particles currently inside volume \f$v\f$.
 */
int retQSS_volume_countParticlesInside(VolumeID v);

/**
 * \brief Compute a random point inside a volume.
 *
 * Compute a random point inside a volume.
 */
void retQSS_volume_randomPoint(
		VolumeID v,
		double *x, double *y, double *z);


/**
 * \brief Retrieve the ID of a particle inside a volume.
 *
 * Retrieve the ID of the \f$i\f$-th particle inside volume \f$v\f$.
 *
 * \note Particles follow a FIFO order.
 */
ParticleID retQSS_volume_IDOfParticleInside(VolumeID v, Index i);

/**
 * \brief Compute distance from a volume to a point.
 *
 * Compute the Euclidean distance from volume \f$v\f$ to point
 * \f$(x,y,z)\f$.
 */
double retQSS_volume_distanceToPoint(
		VolumeID v,
		double x, double y, double z);

/**
 * \brief Compute capacity of a volume.
 *
 * Compute capacity of volume \f$v\f$ (i.e., its volume).
 */
double retQSS_volume_capacity(VolumeID v);

/**
 * \brief Compute centroid of a volume.
 *
 * Compute the centroid of volume \f$v\f$.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
void retQSS_volume_centroid(
		VolumeID v,
		double *x, double *y, double *z);

/**
 * \brief Compute intersection of a ray in a volume.
 *
 * Compute intersection point of ray \f$\vec{r}\f$ with one of the faces
 * of polyhedral volume \f$v\f$, where \f$\vec{r}\f$ is given by point
 * \f$(x,y,z)\f$ and direction \f$(d_x, d_y, d_z)\f$.
 *
 * \pre Point \f$(x,y,z)\f$ is inside volume \f$v\f$.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
void retQSS_volume_rayIntersection(
		VolumeID v,
		double x, double y, double z,
		double dx, double dy, double dz,
		double *ix, double *iy, double *iz);

/**
 * \brief Compute ID of adjacent volume in a given direction
 *
 * Compute the ID of the adjacent volume of volume \f$v\f$ found by tracing a
 * ray \f$\vec{r}\f$ with origin \f$(x,y,z)\f$ and direction
 * \f$(d_x, d_y, d_z)\f$.
 *
 * \pre Point \f$(x,y,z)\f$ is inside volume \f$v\f$.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
VolumeID retQSS_volume_adjacentVolumeID(
		VolumeID v,
		double x, double y, double z,
		double dx, double dy, double dz);

/**
 * \brief Check if property is defined in a volume.
 *
 * Check if a given property is defined in a volume.
 *
 * \return Boolean value indicating whether the property is defined.
 */
Bool retQSS_volume_hasProperty(VolumeID v, PropertyTag tag);

/**
 * \brief Retrieve value associated with a property tag.
 *
 * Retrieve the value associated with a property tag in the
 * given volume.
 *
 * \pre Property must be defined in the object (see
 * retQSS_volume_hasProperty()).
 */
PropertyValue retQSS_volume_getProperty(VolumeID v, PropertyTag tag);

/**
 * \brief Retrieve 3D vector associated with a property tag.
 *
 * Retrieve the 3D vector associated with a property tag in the
 * given volume.
 *
 * \note In case this property was set with a single value, the vector returned
 * will contain three copies of this value.
 *
 * \pre Property must be defined in the object (see
 * retQSS_particle_hasProperty()).
 *
* \return Boolean status indicating success (can be safely ignored).
 */
void retQSS_volume_getPropertyVector(
		VolumeID v,
		PropertyTag tag,
		double *x, double *y, double *z);

/**
 * \brief Set value associated with a property tag.
 *
 * Associate a value with a property tag in the given volume.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this new value.
 *
 * \return Property value set.
 */
PropertyValue retQSS_volume_setProperty(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value);

/**
 * \brief Set 3D vector associated with a property tag.
 *
 * Associate a 3D vector with a property tag in the given volume.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this vector.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
Bool retQSS_volume_setPropertyVector(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3);

/**
 * \brief Count number of vertices in a volume.
 *
 * Get number of vertices of a volume.
 */
int retQSS_volume_countVertices(VolumeID v);

/**
 * \brief Count number of faces in a volume.
 *
 * Get number of faces of a volume.
 */
int retQSS_volume_countFaces(VolumeID v);

/**
 * \brief Retrieve the ID of a vertex in a volume.
 *
 * Retrieve the ID of the \f$i\f$-th vertex of volume \f$v\f$.
 *
 * \note Vertex order follows the same order given in the geometry file used
 * to initialize retQSS.
 */
VertexID retQSS_volume_vertexID(VolumeID v, Index i);

/**
 * \brief Retrieve the ID of a logical face in a volume.
 *
 * Retrieve the ID of the \f$i\f$-th logical face of volume \f$v\f$.
 *
 * \note Faces may not follow a particular order.
 */
LogicalFaceID retQSS_volume_faceID(VolumeID v, Index i);

/**
 * \brief Retrieve value associated with a vertex property tag.
 *
 * Retrieve the value associated with a property tag in the \f$i\f$-th vertex
 * of volume \f$v\f$.
 *
 * \pre Property must be defined in the object (see
 * retQSS_vertex_hasProperty()).
 */
PropertyValue retQSS_volume_getVertexProperty(
		VolumeID v,
		Index i,
		PropertyTag tag);

/**
 * \brief Set value associated with a vertex property tag.
 *
 * Associate a value with a property tag in the \f$i\f$-th vertex of volume
 * \f$v\f$.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this new value.
 *
 * \return Property value set.
 */
PropertyValue retQSS_volume_setVertexProperty(
		VolumeID v,
		Index i,
		PropertyTag tag,
		PropertyValue value);

/**
 * \brief Retrieve coordinates of a vertex in a volume.
 *
 * Retrieve the coordinates of the \f$i\f$-th vertex of volume \f$v\f$.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
void retQSS_volume_vertexCoordinates(
		VolumeID v,
		Index i,
		double *x, double *y, double *z);
/**
 * @}
 */

/** \addtogroup Vertex Vertex queries
 *  Vertex queries
 *  \ingroup API
 * @{
 */

/**
 * \brief Check if property is defined in a vertex.
 *
 * Check if a given property is defined in a vertex.
 *
 * \return Boolean value indicating whether the property is defined.
 */
Bool retQSS_vertex_hasProperty(VertexID u, PropertyTag tag);

/**
 * \brief Retrieve value associated with a property tag.
 *
 * Retrieve the value associated with a property tag in the
 * given vertex.
 *
 * \pre Property must be defined in the object (see
 * retQSS_vertex_hasProperty()).
 */
PropertyValue retQSS_vertex_getProperty(VertexID u, PropertyTag tag);

/**
 * \brief Retrieve 3D vector associated with a property tag.
 *
 * Retrieve the 3D vector associated with a property tag in the
 * given vertex.
 *
 * \note In case this property was set with a single value, the vector returned
 * will contain three copies of this value.
 *
 * \pre Property must be defined in the object (see
 * retQSS_particle_hasProperty()).
 *
* \return Boolean status indicating success (can be safely ignored).
 */
void retQSS_vertex_getPropertyVector(
		VertexID u,
		PropertyTag tag,
		double *x, double *y, double *z);

/**
 * \brief Set value associated with a property tag.
 *
 * Associate a value with a property tag in the given vertex.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this new value.
 *
 * \return Property value set.
 */
PropertyValue retQSS_vertex_setProperty(
		VertexID u,
		PropertyTag tag,
		PropertyValue value);

/**
 * \brief Set 3D vector associated with a property tag.
 *
 * Associate a 3D vector with a property tag in the given vertex.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this vector.
 *
 * \return Boolean status indicating success (can be safely ignored).
 */
Bool retQSS_vertex_setPropertyVector(
		VertexID u,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3);

/**
 * \brief Retrieve vertex coordinates.
 *
 * Retrieve 3D coordinates of the given vertex.
 */
void retQSS_vertex_coordinates(
		VertexID u,
		double *x, double *y, double *z);
/**
 * @}
 */

/** \addtogroup Neighborhood Neighborhood queries
 *  Neighborhood queries
 *  \ingroup API
 *
 *  Notation and assumptions:
 *   - Volume neighborhoods only consider polyhedral volumes (i.e., volume 0
 *     is never computed as neighbor).
 *   - \f$\PartNeigh{p}\f$ is the set of neighboring particles of particle
 * \f$p\f$.
 *   - \f$\PartNeigh{p} = \emptyset\f$ if \f$\Vol{p} = 0\f$.
 * @{
 */

/**
 * \brief Count particles in neighborhood.
 *
 * Get the number of particles in \f$\PartNeigh{p}\f$.
 *
 */
int retQSS_particleNeighborhood_countParticles(ParticleID p);

/**
 * \brief Compute average position of neighboring particles.
 *
 * Compute the average position of the particles in \f$\PartNeigh{p}\f$.
 *
 * \note Result is undefined when \f$\PartNeigh{p} = \emptyset\f$ (in these
 * situations, count will be zero).
 *
 * \return Number of neighboring particles evaluated.
 */
void retQSS_particleNeighborhood_averagePosition(
		ParticleID p,
		double *count, double *x, double *y, double *z);

/**
 * \brief Compute average velocity of neighboring particles.
 *
 * Compute the average velocity of the particles in \f$\PartNeigh{p}\f$.
 *
 * \note Result is undefined when \f$\PartNeigh{p} = \emptyset\f$ (in these
 * situations, count will be zero).
 *
 * \return Number of neighboring particles evaluated.
 */
void retQSS_particleNeighborhood_averageVelocity(
		ParticleID p,
		double *count, double *x, double *y, double *z);

/**
 * \brief Compute repulsive direction of neighboring particles.
 *
 * Compute the repulsive direction of the particles in \f$\PartNeigh{p}\f$.
 * The repulsive direction \f$\vec{d_p}\f$ of particle \f$p\f$ is
 * defined as
 * \f[\vec{d_p} = \sum_{q \, \in \, \PartNeigh{p}}
 * 					{\frac{\PartPos{p} - \PartPos{q}}
 * 						  {\VecNorm{ \PartPos{p} - \PartPos{q} }}
 * 				 	}
 * \f]
 *
 * \note Result is undefined when \f$\PartNeigh{p} = \emptyset\f$ (in these
 * situations, count will be zero).
 *
 * \return Number of neighboring particles evaluated.
 */
void retQSS_particleNeighborhood_repulsiveDirection(
		ParticleID p,
		double *count, double *x, double *y, double *z);

/**
 * \brief Apply a function to each particle in neighborhood.
 *
 * Given a particle \f$p\f$, for each particle \f$q \in \PartNeigh{p}\f$, this
 * function applies the C++ function given as argument to \f$q\f$ and sums the
 * resulting vector to an accumulator. The expected signature of the function is:
 *
 * @code
 * bool function(ParticleID p, ParticleID q, Vector_3 &result)
 * @endcode
 *
 * The result is ignored when \c function returns \c false.
 *
 * \note Result is undefined when \f$\PartNeigh{p} = \emptyset\f$ (in these
 * situations, count will be zero).
 *
 * \param function Name of the C++ function.
 *
 * \return Number of neighboring particles evaluated.
 */
void retQSS_particleNeighborhood_forEachParticle(
		ParticleID p,
		const char *function,
		double *count, double *x, double *y, double *z);

/**
 * \brief Apply a function to each particle in neighborhood.
 *
 * Given a particle \f$p\f$, for each particle \f$q \in \PartNeigh{p}\f$, this
 * function applies the C++ function given as argument to \f$q\f$ and sums the
 * resulting vector to an accumulator. The expected signature of the function is:
 *
 * @code
 * bool function(ParticleID p, ParticleID q, const vector<double> &args, Vector_3 &result)
 * @endcode
 *
 * Here, \c args is a C++ \c vector containing user-defined arguments for the function.
 *
 * The result is ignored when \c function returns \c false.
 *
 * \note Result is undefined when \f$\PartNeigh{p} = \emptyset\f$ (in these
 * situations, count will be zero).
 *
 * \param function Name of the C++ function.
 * \param arg1 Argument for \c function.
 *
 * \return Number of neighboring particles evaluated.
 *
 * \sa retQSS_particleNeighborhood_forEachParticle
 */
void retQSS_particleNeighborhood_forEachParticle_1(
		ParticleID p,
		const char *function,
		double arg1,
		double *count, double *x, double *y, double *z);

/**
 * \brief Apply a function to each particle in neighborhood.
 *
 * Given a particle \f$p\f$, for each particle \f$q \in \PartNeigh{p}\f$, this
 * function applies the C++ function given as argument to \f$q\f$ and sums the
 * resulting vector to an accumulator. The expected signature of the function is:
 *
 * @code
 * bool function(ParticleID p, ParticleID q, const vector<double> &args, Vector_3 &result)
 * @endcode
 *
 * Here, \c args is a C++ \c vector containing user-defined arguments for the function.
 *
 * The result is ignored when \c function returns \c false.
 *
 * \note Result is undefined when \f$\PartNeigh{p} = \emptyset\f$ (in these
 * situations, count will be zero).
 *
 * \param function Name of the C++ function.
 * \param arg1 First argument for \c function.
 * \param arg2 Second argument for \c function.
 *
 * \return Number of neighboring particles evaluated.
 *
 * \sa retQSS_particleNeighborhood_forEachParticle
 */
void retQSS_particleNeighborhood_forEachParticle_2(
		ParticleID p,
		const char *function,
		double arg1,
		double arg2,
		double *count, double *x, double *y, double *z);

/**
 * \brief Evaluate a function over a particle neighborhood.
 *
 * TBD
 *
 * \param function Name of the C++ function to apply.
 * \param arg1 Extra argument to pass to \c function.
 *
 * \return Number of neighboring particles evaluated.
 *
 * \sa retQSS_particleNeighborhood_forEachParticle
 */

/**
 * \brief Evaluate a function over a particle neighborhood.
 *
 * Given a particle \f$p\f$, this function applies the C++ function given as
 * argument to \f$\PartNeigh{p}\f$. The output is the 3D vector returned by
 * this funcion. Its expected signature is:
 * @code
 * Vector_3 function(retQSS::ParticleNeighborhood *neigh, double arg)
 * @endcode
 *
 * Here, \c arg is a user-defined argument for the function.
 *
 * \param function Name of the C++ function.
 * \param arg Argument for \c function.
 *
 * \return Number of neighboring particles evaluated.
 */
void retQSS_particleNeighborhood_evaluate_1(
		ParticleID p,
		const char *function,
		double arg1,
		double *x, double *y, double *z);

/**
 * \brief Set volume neighborhood computation algorithm to default.
 *
 * Set algorithm to compute volume neighbors to \em default (i.e., volume
 * \f$w\f$ is neighbor of \f$v\f$ iff they share a common face). These
 * neighborhoods are defined upon initializing geometry data structures, and
 * thus there are no extra performance penalties in computing them.
 *
 * \note Cached neighborhoods computed before switching algorithms are
 * discarded.
 * \note Volume 0 is not taken into account for neighbor computations.
 *
 * \return Boolean status indicating success (can be safely ignored).
 *
 * \sa retQSS_volumeNeighborhood_toFile,
 * retQSS_volumeNeighborhood_toRadial,
 * retQSS_volumeNeighborhood_toPeriodicRadial,
 * retQSS_volumeNeighborhood_toVertexSharing
 */
Bool retQSS_volumeNeighborhood_toDefault();

/**
 * \brief Set volume neighborhood computation algorithm to file-based.
 *
 * Set algorithm to compute volume neighbors to \em file-based. The text file
 * describing volume neighborhoods is expected to define one neighborhood per
 * line, starting with a polyehdral volume ID \f$v\f$ followed by every
 * neighbor ID of \f$v\f$ separated by an arbitrary number of spaces. In case
 * a volume is not included, its neighborhood will be empty.
 *
 * \note Cached neighborhoods computed before switching algorithms are
 * discarded.
 * \note Volume 0 is not taken into account for neighbor computations.
 *
 * \param filename Path to file specifying volume neighborhoods.
 *
 * \return Boolean status indicating success (can be safely ignored).
 *
 * \sa retQSS_volumeNeighborhood_toDefault,
 * retQSS_volumeNeighborhood_toRadial,
 * retQSS_volumeNeighborhood_toPeriodicRadial,
 * retQSS_volumeNeighborhood_toVertexSharing
 */
Bool retQSS_volumeNeighborhood_toFile(const char *filename);

/**
 * \brief Set volume neighborhood computation algorithm to radial.
 *
 * Set algorithm to compute volume neighbors to \em radial. Volume \f$w\f$ is
 * considered neighbor of volume \f$v\f$ iff the Euclidean distance from
 * \f$v\f$'s centroid to \f$w\f$ is not greater than \f$r\f$. Neighborhoods are
 * sorted increasingly by this distance (i.e., neighbor at index 1 is the
 * closest to \f$v\f$'s centroid).
 *
 * \note Can be computationally expensive, especially if \f$r\f$ spans several
 * volumes in the geometry.
 * \note Cached neighborhoods computed before switching algorithms are
 * discarded.
 * \note Volume 0 is not taken into account for neighbor computations.
 * \note \f$v\f$ is considered its own neighbor since its distance to its
 * centroid is 0.
 *
 * \param r Neighborhood radius.
 *
 * \return Boolean status indicating success (can be safely ignored).
 *
 * \sa retQSS_volumeNeighborhood_toDefault,
 * retQSS_volumeNeighborhood_toFile,
 * retQSS_volumeNeighborhood_toPeriodicRadial,
 * retQSS_volumeNeighborhood_toVertexSharing
 */
Bool retQSS_volumeNeighborhood_toRadial(double r);

/**
 * \brief Set volume neighborhood computation algorithm to periodic radial.
 *
 * Set algorithm to compute volume neighbors to \em periodic \em radial. A
 * \em periodic \em radial neighborhood works exactly like a standard radial
 * neighborhood when no periodic boundaries are faced during the computation.
 * When this happens, the periodic radial neighborhood translates \f$v\f$'s
 * centroid using the inward normal vector to the periodic boundary face and
 * resumes the computation in the opposite side of the geometry. Neighborhoods
 * are sorted increasingly by this distance (i.e., neighbor at index 1 is the
 * closest to \f$v\f$'s centroid).
 *
 * \note Can be computationally expensive, especially if \f$r\f$ spans several
 * volumes in the geometry.
 * \note Cached neighborhoods computed before switching algorithms are
 * discarded.
 * \note Volume 0 is not taken into account for neighbor computations.
 * \note \f$v\f$ is considered its own neighbor since its distance to its
 * centroid is 0.
 *
 * \param r Radius of the neighborhoods.
 *
 * \return Boolean status indicating success (can be safely ignored).
 *
 * \sa retQSS_volumeNeighborhood_toDefault,
 * retQSS_volumeNeighborhood_toRadial,
 * retQSS_volumeNeighborhood_toFile,
 * retQSS_volumeNeighborhood_toVertexSharing
 */
Bool retQSS_volumeNeighborhood_toPeriodicRadial(double r);

/**
 * \brief Set volume neighborhood computation algorithm to vertex-sharing.
 *
 * Set algorithm to compute volume neighbors to \em vertex-sharing. Volume
 * \f$w\f$ is considered neighbor of volume \f$v\f$ iff they both share at
 * least one common vertex.
 *
 * \note Cached neighborhoods computed before switching algorithms are
 * discarded.
 * \note Volume 0 is not taken into account for neighbor computations.
 *
 * \return Boolean status indicating success (can be safely ignored).
 *
 * \sa retQSS_volumeNeighborhood_toDefault,
 * retQSS_volumeNeighborhood_toFile,
 * retQSS_volumeNeighborhood_toPeriodicRadial,
 * retQSS_volumeNeighborhood_toRadial
 */
Bool retQSS_volumeNeighborhood_toVertexSharing();

/**
 * \brief Precompute all volume neighborhoods
 *
 * Compute and cache the neighborhoods of all polyhedral volumes in the
 * geometry.
 *
 * \note Can have significant performance penalties.
 */
Bool retQSS_volumeNeighborhood_precomputeAll();

/**
 * \brief Compute number of volumes in a volume neighborhood.
 *
 * Get the number of volumes in volume \f$v\f$'s neighborhood.
 */
int retQSS_volumeNeighborhood_countVolumes(VolumeID v);

/**
 * \brief Retrieve ID of a volume in a volume neighborhood.
 *
 * Retrieve the volume ID of the \f$i\f$-th neighbor of volume \f$v\f$.
 */
VolumeID retQSS_volumeNeighborhood_neighborID(VolumeID v, Index i);

/**
 * \brief Retrieve a property value in a volume neighbor.
 *
 * Retrieve the value associated with a property tag in the \f$i\f$-th
 * neighbor of volume \f$v\f$.
 *
 * \pre Property must be defined in the object (see
 * retQSS_volume_hasProperty()).
 */
PropertyValue retQSS_volumeNeighborhood_getNeighborProperty(
		VolumeID v,
		Index i,
		PropertyTag tag);

/**
 * \brief Set a property in a whole volume neighborhood.
 *
 * Associate a value with a property tag in every volume in the
 * neighborhood of volume \f$v\f$.
 *
 * \note Property is defined anew or otherwise its value is
 * overwritten using this new value.
 *
 * \return New property value set.
 */
PropertyValue retQSS_volumeNeighborhood_setProperty(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value);


/**
 * @}
 */

/** \addtogroup Misc Miscellaneous queries
 *  Miscellaneous queries
 *  \ingroup API
 * @{
 */

Bool retQSS_random_reseed(int);
double retQSS_random(double, double);
double retQSS_random_lognormal(double, double);
double retQSS_random_normal(double, double);
int retQSS_modulus(int, int);
int retQSS_div(int, int);
double retQSS_max(double, double);
double retQSS_min(double, double);
void retQSS_vectorWithNorm(
		double vx, double vy, double vz,
		double norm,
		double *ux, double *uy, double *uz);

void retQSS_randomVector(
		double *vx, double *vy, double *vz);

void retQSS_randomVectorWithNorm(
		double norm,
		double *vx, double *vy, double *vz);


/**
 * @}
 */

/** \addtogroup Engine Engine interface
 *  Engine interface
 *  \ingroup API
 * @{
 */

void retQSS_clear();
/**
 * @}
 */

#ifdef __cplusplus
}

#include <functional>

#define RETQSS_BIND(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS()))

#define RETQSS_BIND_1(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS(),\
		std::placeholders::_1))

#define RETQSS_BIND_2(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS(),\
		std::placeholders::_1,\
		std::placeholders::_2))

#define RETQSS_BIND_3(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS(),\
		std::placeholders::_1,\
		std::placeholders::_2,\
		std::placeholders::_3))

#define RETQSS_BIND_4(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS(),\
		std::placeholders::_1,\
		std::placeholders::_2,\
		std::placeholders::_3,\
		std::placeholders::_4))

#define RETQSS_BIND_5(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS(),\
		std::placeholders::_1,\
		std::placeholders::_2,\
		std::placeholders::_3,\
		std::placeholders::_4,\
		std::placeholders::_5))

#define RETQSS_BIND_6(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS(),\
		std::placeholders::_1,\
		std::placeholders::_2,\
		std::placeholders::_3,\
		std::placeholders::_4,\
		std::placeholders::_5,\
		std::placeholders::_6))

#define RETQSS_BIND_7(func) \
	(std::bind(\
		&retQSS::Interface::func,\
		RETQSS(),\
		std::placeholders::_1,\
		std::placeholders::_2,\
		std::placeholders::_3,\
		std::placeholders::_4,\
		std::placeholders::_5,\
		std::placeholders::_6,\
		std::placeholders::_7))

/**
 * \brief Execute retQSS query
 *
 * Execute the given retQSS query and catch eventual exceptions thrown by the
 * retQSS engine. When an exception is caught, its message is printed to the
 * standard output and the simulation is aborted.
 */
template<class ReturnType, class... Args>
ReturnType retQSS_execute(
		std::function<ReturnType(Args...)> query,
		Args... args);
#endif

#endif

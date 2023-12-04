/**
 * \file retqss_particle_tracker.hh
 */

#ifndef _RETQSS_PARTICLE_TRACKER_H_
#define _RETQSS_PARTICLE_TRACKER_H_

#include <vector>
#include <utility>

#include "retqss_cgal_main_types.hh"
#include "retqss_types.hh"
#include "retqss_geometry.hh"
#include "retqss_volume.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_simulator_interface.hh"
#include "retqss_particle.hh"
#include "retqss_trajectory.hh"
#include "retqss_utilities.hh"
#include "retqss_exceptions.hh"


namespace retQSS
{

class Interface;

/**
 * \brief Particle state
 *
 * Internal data structure to keep track of a particle as it travels across the
 * geometry.
 */
struct ParticleState
{
	bool alive; //!< Whether the particle is still alive

	retQSS::Volume *current_vol; //!< Current volume
	retQSS::PolyhedronFace *entry_face; //!< Face through which it entered the
	                                    //!< current volume
	double entry_time; //!< Time at which it entered the current volume

	retQSS::Volume *prev_vol; //!< Previous volume
	retQSS::PolyhedronFace *prev_exit_face; //!< Face through which it exited
	                                        //!< the previous volume
	Point_3 prev_exit_point; //!< Intersection point of the trajectory and the exit
	                         //!< face (meaningful when the exit face is set)

	double next_exit_time; //!< Time of next scheduled boundary crossing
	retQSS::PolyhedronFace *next_exit_face; //!< Exit face of next scheduled boundary crossing
	Point_3 next_exit_point; //!< Exit point of next scheduled boundary crossing

#ifdef RETQSS_DEBUG
	retQSS::TraversedVolumes traversed_vols;
	retQSS::PointLists points;
#endif
};

/**
 * \brief Tracker of particles
 *
 * The \em particle \em tracker is in charge of keeping track of each particle
 * and manage their geometrical state as they travel across the geometry. A
 * single instance of this class is instanciated by the retQSS interface
 * (retQSS::Interface) when the API method to initialize particles is invoked.
 */
class ParticleTracker
{
	friend class retQSS::Interface;

	typedef std::vector<
				std::pair<
					retQSS::Particle*,
					retQSS::ParticleState*
				>
			> ParticleArray;

private:
	retQSS::SimulatorInterface *sim_interface; //!< Simulator interface for
                                               //!< position and velocity
                                               //!< queries. Ownership taken.

	ParticleArray particles; //!< Vector of particles and their state, indexed
	                         //!< by particle ID

	Internal_ParticleID current_id; //!< Current ID to be used for next
	                                //!< particle

	retQSS::Geometry *geometry; //!< Geometrical queries are delegated to the
	                            //!< geometry object.

	std::mutex mutex; //!< Mutex to protect against concurrent access to
	                  //!< particle state (for parallel simulations)

	/**
	 * \brief Create new particle
	 *
	 * Create new particle with a given initial time, position and velocity.
	 */
	retQSS::Particle *new_particle(
			const retQSS::InitialCondition &initial_conditions);

	/**
	 * \brief Update particle geometrical state
	 *
	 * Taking into account the exit face and exit point of the given particle,
	 * this function retrieves the corresponding adjacent volume and updates
	 * the particle state using that information. The new entry face is the
	 * adjacent face of this volume which contains the exit point through
	 * which the particle escaped the old volume.
	 */
	void update_state(Internal_ParticleID p);

	/**
	 * \brief Set particle exit state
	 *
	 * Update particle exit state with the given information, typically
	 * obtained via the face crossing algorithm
	 * (retQSS::FaceCrossingAlgorithm).
	 */
	void set_exit_state(
			Internal_ParticleID p,
			retQSS::CrossedFaceData &crossed_face_data);

	/**
	 * \brief Relocate particle
	 *
	 * Relocate particle upating its tracked state from the given volume. Entry
	 * and exit faces are cleared.
	 */
	void relocate(
			Internal_ParticleID p,
			retQSS::Volume *volume);

	/**
	 * \brief Create new trajectory object
	 *
	 * Create new trajectory object for the given particle, calling the
	 * simulator interface to retrieve the pointers to the approximation
	 * polynomials for the positional state variables.
	 */
	void new_trajectory(retQSS::Particle *particle) const;

	/**
	 * \brief Create new particle neighborhood
	 *
	 * Create new particle neighborhood object for the given particle.
	 */
	void new_neighborhood(retQSS::Particle *particle) const;

	/**
	 * \brief Dump particle initial conditions to stream
	 */
	void dump_initial_conditions_to(
			retQSS::Particle *particle,
			std::ostream &out) const;

	/**
	 * \brief Retrieve particle geometrical state
	 */
	inline retQSS::ParticleState *get_state(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second;
	}

	/**
	 * \brief Kill particle
	 */
	inline void kill(Internal_ParticleID p)
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		this->particles[p].second->alive = false;
	}

#ifdef RETQSS_DEBUG
	/**
	 * \brief Add new point to particle trajectory
	 *
	 * Add new sampled point to the given particle trajectory.
	 */
	void add_trajectory_point(
			Internal_ParticleID p,
			const Point_3 &point,
			double time);

	/**
	 * \brief Dump particle trajectory to a VTK file
	 *
	 * Dump sampled particle trajectory to a VTK file containing a polyline
	 * constructed after the sampled points.
	 */
	void save_trajectory_to_vtk(
			Internal_ParticleID p,
			const retQSS::PointLists &point_lists);
#endif

public:
	/**
	 * \brief Constructor
	 *
	 * Create new particle tracker.
	 */
	ParticleTracker(
			retQSS::SimulatorInterface *simulator_interface,
			retQSS::Geometry *geometry);

	~ParticleTracker();

	/**
	 * \brief Create particles
	 *
	 * Initialize internal particle state using the initial conditions supplied
	 * as argument. For each particle in the model,
	 *  - A new particle object is created and assigned to the particle
	 *  tracker.
	 *  - Initial conditions are set into the simulation engine (in case they
	 *  were read from a file).
	 *  - The initial volume of the particle is found and particle state is
	 *  updated using this information.
	 */
	void create_particles(
			const retQSS::InitialConditionArray &initial_conditions,
			const std::string &model_name);

	/**
	 * \brief Dump particle initial position and velocity to file
	 */
	void dump_initial_conditions(
			Internal_ParticleID p,
			const std::string &filename) const;

	/**
	 * \brief Dump initial position and velocity of all particles to file
	 */
	void dump_initial_conditions(const std::string &filename) const;

	/**
	 * \brief TBD
	 */
	void dump_current_values(const std::string &filename) const;

	/**
	 * \brief Retrieve volume of next scheduled boundary crossing
	 *
	 * Retrieve volume of next scheduled boundary crossing of a particle.
	 *
	 * \param entry_face_ptr Optional. It will point to the face through which
	 * the particle will enter the volume if a non-null pointer is given.
	 */
	retQSS::Volume *next_volume(
			Internal_ParticleID p,
			retQSS::PolyhedronFace **entry_face_ptr =nullptr) const;

	/**
	 * \brief Retrieve total number of particles
	 */
	inline size_t num_particles() const
	{
		return this->particles.size();
	}

	/**
	 * \brief Retrieve particle object by ID
	 */
	inline retQSS::Particle *get_particle(Internal_ParticleID p) const
	{
		this->check_ID(p);
		return this->particles[p].first;
	}

	/**
	 * \brief Retrieve particle object by ID
	 *
	 * \pre ID is valid
	 */
	inline retQSS::Particle *get_particle_unsafe(Internal_ParticleID p) const
	{
		return this->particles[p].first;
	}

	/**
	 * \brief Retrieve current volume of a particle
	 */
	inline retQSS::Volume *current_volume(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second->current_vol;
	}

	/**
	 * \brief Retrieve current volume of a particle
	 *
	 * \pre Particle ID is valid
	 */
	inline retQSS::Volume *current_volume_unsafe(Internal_ParticleID p) const
	{
		return this->particles[p].second->current_vol;
	}

	/**
	 * \brief Retrieve previous volume of a particle
	 */
	inline retQSS::Volume *previous_volume(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second->prev_vol;
	}

	/**
	 * \brief Retrieve particle entry face to current volume
	 */
	inline retQSS::PolyhedronFace *entry_face(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second->entry_face;
	}

	/**
	 * \brief Retrieve particle exit face from previous volume
	 */
	inline retQSS::PolyhedronFace *exit_face(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second->prev_exit_face;
	}

	/**
	 * \brief Retrieve particle entry time to current volume
	 */
	inline double entry_time(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second->entry_time;
	}

	/**
	 * \brief Retrieve particle exit point from previous volume
	 */
	inline const Point_3 exit_point(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second->prev_exit_point;
	}

	/**
	 * \brief Retrieve time of next scheduled boundary crossing
	 */
	inline double next_exit_time(Internal_ParticleID p) const
	{
		this->check_ID(p);
		return this->particles[p].second->next_exit_time;
	}

	/**
	 * \brief Retrieve time of next scheduled boundary crossing
	 *
	 * \pre Particle ID is valid
	 */
	inline double next_exit_time_unsafe(Internal_ParticleID p) const
	{
		return this->particles[p].second->next_exit_time;
	}

	/**
	 * \brief Check whether particle is still alive
	 */
	inline bool is_alive(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->particles[p].second->alive;
	}

	/**
	 * \brief Compute current particle position
	 */
	inline Point_3 current_position(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->sim_interface->current_position(p);
	}

	/**
	 * \brief Compute current particle velocity
	 */
	inline Vector_3 current_velocity(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->sim_interface->current_velocity(p);
	}

	/**
	 * \brief Retrieve current particle time
	 */
	inline double current_time(Internal_ParticleID p)
	{
		this->check_ID(p);
		return this->sim_interface->get_simulator_time(p);
	}

	/**
	 * \brief Retrieve current particle time
	 *
	 * \pre Particle ID is valid
	 */
	inline double current_time_unsafe(Internal_ParticleID p)
	{
		return this->sim_interface->get_simulator_time(p);
	}

	/**
	 * \brief Retrieve initial particle time
	 */
	inline double initial_time(Internal_ParticleID p)
	{
		return this->get_particle(p)->initial_time();
	}

	/**
	 * \brief Retrieve boundary crossing tolerance
	 *
	 * \sa retQSS::SimulatorInterface::crossing_tolerance()
	 */
	inline double crossing_tolerance(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");

		return this->sim_interface->crossing_tolerance(p);
	}

	/**
	 * \brief Retrieve infinity constant defined by the simulation engine
	 *
	 * \sa retQSS::SimulatorInterface::infinity()
	 */
	inline double infinity() const
	{
		return this->sim_interface->infinity();
	}

	/**
	 * \brief Retrieve order of underlying numerical method
	 */
	inline int order() const
	{
		return this->sim_interface->get_method_order();
	}

	/**
	 * \brief TBD
	 */
	inline void check_ID(Internal_ParticleID p) const
	{
		if(p >= this->num_particles())
			throw retQSS::InvalidObjectIDException(p, "particle");
	}
};

}

#endif

#include "retqss_particle.hh"
#include "retqss_particle_neighborhood.hh"


retQSS::Particle::Particle(
		Internal_ParticleID p,
		double t0,
		const retQSS::InitialCondition &ic)
		: retQSS::Object(p, "Particle"),
		  trajectory_obj(nullptr),
		  neighborhood_obj(nullptr)
{
	this->t0 = t0;
	this->initial_pos = Point_3(ic.x, ic.y, ic.z);
	this->initial_vel = Vector_3(ic.vx, ic.vy, ic.vz);
}

retQSS::Particle::~Particle()
{
	delete this->trajectory_obj;
	delete this->neighborhood_obj;
}

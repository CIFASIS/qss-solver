#include "retqss_simulator_interface.hh"
#include "retqss_interface.hh"
#include "retqss_particle_tracker.hh"
#include "retqss_qss_solver_interface.hh"


retQSS::SimulatorInterface*
retQSS::SimulatorInterface::create_for(
		retQSS::Interface *interface,
		int n_particles)
{
	// Future interfaces should be added here.
	return new retQSS::QSSSolverInterface(n_particles);
}

Point_3 retQSS::SimulatorInterface::current_position(Internal_ParticleID p)
{
	double x, y, z;
	double time = this->get_simulator_time(p);
	this->get_position_at(p, time, &x, &y, &z);
	return Point_3(x,y,z);
}

Vector_3 retQSS::SimulatorInterface::current_velocity(Internal_ParticleID p)
{
	double vx, vy, vz;
	double time = this->get_simulator_time(p);
	this->get_velocity_at(p, time, &vx, &vy, &vz);
	return Vector_3(vx,vy,vz);
}

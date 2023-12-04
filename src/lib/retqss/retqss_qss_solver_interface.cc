#include <fstream>

#include "retqss_qss_solver_interface.hh"
#include "retqss_exceptions.hh"


retQSS::QSSSolverInterface::QSSSolverInterface(size_t num_particles) :
	retQSS::SimulatorInterface(num_particles),
	parallel(false)
{
	// _simulator is a global variable exposed by QSS Solver.
	this->simulators = _simulator;
}

void retQSS::QSSSolverInterface::particle_set_up(const std::string &model_name)
{
	QSS_simulator sim = this->get_root_simulator();

	if(sim->data->params->lps > 0)
	{
		if(sim->data->params->pm != SD_Manual)
			throw retQSS::UnsupportedPartitioningMethodException();

		this->lps.resize(this->num_particles());
		this->set_particle_partition(model_name);
		this->parallel = true;
	}
}

int retQSS::QSSSolverInterface::get_method_order()
{
	QSS_simulator sim = this->get_root_simulator();
	return sim->data->order;
}

double retQSS::QSSSolverInterface::crossing_tolerance(Internal_ParticleID p)
{
	QSS_simulator sim = this->get_root_simulator();
	size_t n = this->num_particles();

	return sim->data->dQMin[RETQSS_X_IDX_N(p,n)];
}

double retQSS::QSSSolverInterface::get_simulator_time(Internal_ParticleID p)
{
	QSS_simulator sim = this->get_particle_simulator(p);
	return sim->time ? sim->time->time : sim->data->it;
}

double retQSS::QSSSolverInterface::get_initial_time()
{
	QSS_simulator sim = this->get_root_simulator();
	return sim->data->it;
}

retQSS::Trajectory*
retQSS::QSSSolverInterface::get_particle_trajectory(Internal_ParticleID p)
{
	QSS_simulator sim = this->get_particle_simulator(p);

	QSS_data data = sim->data;
	QSS_time time = sim->time;

	int order = data->order, order1 = order + 1;
	size_t n = this->num_particles();
	double *x = data->x;

	int x_idx = RETQSS_X_IDX_N(p,n);
	int y_idx = RETQSS_Y_IDX_N(p,n);
	int z_idx = RETQSS_Z_IDX_N(p,n);

	double *px = x + x_idx*order1;
	double *py = x + y_idx*order1;
	double *pz = x + z_idx*order1;

	double *t0 = &time->time;
	double *tx = &time->tx[x_idx];
	double *ty = &time->tx[y_idx];
	double *tz = &time->tx[z_idx];

	return retQSS::Trajectory::for_order(
			order,
			px, py, pz,
			tx, ty, tz,
			t0);
}

void retQSS::QSSSolverInterface::get_position_at(
		Internal_ParticleID p,
		double time,
		double *x, double *y, double *z)
{
	QSS_simulator sim = this->get_particle_simulator(p);
	size_t n = this->num_particles();

	this->get_simulator_values(
		sim,
		time,
		RETQSS_X_IDX_N(p,n),
		RETQSS_Y_IDX_N(p,n),
		RETQSS_Z_IDX_N(p,n),
		x, y, z);
}

void retQSS::QSSSolverInterface::get_velocity_at(
		Internal_ParticleID p,
		double time,
		double *vx, double *vy, double *vz)
{
	QSS_simulator sim = this->get_particle_simulator(p);
	size_t n = this->num_particles();

	this->get_simulator_values(
		sim,
		time,
		RETQSS_VX_IDX_N(p,n),
		RETQSS_VY_IDX_N(p,n),
		RETQSS_VZ_IDX_N(p,n),
		vx, vy, vz);
}

void retQSS::QSSSolverInterface::set_position(
		Internal_ParticleID p,
		const Point_3 &position)
{
	QSS_simulator sim = this->get_root_simulator();
	double *x_poly = sim->data->x;
	const int x_order = sim->data->order, x_order1 = x_order + 1;
	size_t n = this->num_particles();

	x_poly[RETQSS_X_IDX_N(p,n)*x_order1]  = position.x();
	x_poly[RETQSS_Y_IDX_N(p,n)*x_order1]  = position.y();
	x_poly[RETQSS_Z_IDX_N(p,n)*x_order1]  = position.z();
}

void retQSS::QSSSolverInterface::set_velocity(
		Internal_ParticleID p,
		const Vector_3 &velocity)
{
	QSS_simulator sim = this->get_root_simulator();
	double *x_poly = sim->data->x;
	const int x_order = sim->data->order, x_order1 = x_order + 1;
	size_t n = this->num_particles();

	x_poly[RETQSS_VX_IDX_N(p,n)*x_order1]  = velocity.x();
	x_poly[RETQSS_VY_IDX_N(p,n)*x_order1]  = velocity.y();
	x_poly[RETQSS_VZ_IDX_N(p,n)*x_order1]  = velocity.z();
}

void retQSS::QSSSolverInterface::get_simulator_values(
		QSS_simulator sim,
		double time,
		int v1_idx, int v2_idx, int v3_idx,
		double *v1, double *v2, double *v3)
{
	const int x_order = sim->data->order, x_order1 = x_order + 1;
	double *x_poly = sim->data->x;
	double *tx = sim->time->tx;

	assert(tx[v1_idx] <= time);
	*v1 = evaluatePoly(v1_idx * x_order1, time - tx[v1_idx], x_poly, x_order);

	assert(tx[v2_idx] <= time);
	*v2 = evaluatePoly(v2_idx * x_order1, time - tx[v2_idx], x_poly, x_order);

	assert(tx[v3_idx] <= time);
	*v3 = evaluatePoly(v3_idx * x_order1, time - tx[v3_idx], x_poly, x_order);
}

void retQSS::QSSSolverInterface::set_particle_partition(
		const std::string &model_name)
{
	QSS_simulator sim = this->get_root_simulator();
	std::string filename = model_name + ".part";
	std::ofstream partition(filename);

	int lps = sim->data->params->lps;
	int states = sim->data->states;
	int events = sim->data->events;
	int var;

	for(Internal_ParticleID i = 0; i < this->num_particles(); i ++)
		this->lps[i] = i % lps;

	for(var = 0; var < 6; var++)
		for(Internal_ParticleID i = 0; i < this->num_particles(); i ++)
			partition << this->lps[i] << std::endl;

	if(states > (var=6*this->num_particles()))
	{
		for(; var < states; var++)
			partition << var%lps << std::endl;
	}

	for(var = 0; var < events; var++)
		partition << var%lps << std::endl;

	partition.close();
}

double retQSS::QSSSolverInterface::infinity()
{
	return INF;
}

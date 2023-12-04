#define private public
#define protected public

#include "qss_test_utils.h"

#include "stubs.h"

extern "C"
{
#include "qss/qss_commands.h"
}

#include "retqss/retqss_geometry.hh"
#include "retqss/retqss_utilities.hh"
#include "retqss/retqss_interface.hh"
#include "retqss/retqss_qss_solver_interface.hh"


// TODO: enhance this and allow the creation of realisitc models to use in
// test cases.
QSS_simulator retQSSTest::build_simulator(
		const std::string &geometry_path,
		retQSS::InitialConditionArray &ic_array,
		int n_particles)
{
	char *geom_path = const_cast<char*>(geometry_path.c_str());
	char *log_file = "temp_log";
	int states = 6*n_particles;

	SD_simulationSettings settings = SD_SimulationSettings();
	settings->parallel = 0;
	settings->hybrid = 0;
	settings->method = static_cast<SD_Solver>(5);

	SIM_simulator simulator = SIM_Simulator(settings);
	QSS_simulator sim = static_cast<QSS_simulator>(simulator->state->sim);
	sim->data = QSS_Data(states,0,0,0,0,geom_path);
	sim->time = QSS_Time(states,0,0,0,ST_Linear,NULL);

	sim->simulationLog = SD_SimulationLog(log_file);
	sim->quantizer = QA_Quantizer(sim->data, sim->time);
	sim->scheduler = SC_Scheduler(sim->data, sim->time);
	sim->frw = FRW_Framework(sim->data);
	sim->output = SD_Output(geom_path,0,0,states,nullptr,0,0,CI_Step,SD_Memory,nullptr);
	sim->model = QSS_Model(nullptr,nullptr,NULL,NULL,NULL);

	for(int i = 0; i < states; ++i)
	{
		sim->data->SD[i] = nullptr;
		sim->data->DS[i] = nullptr;
	}

	QSS_CMD_alloc(sim);

	return sim;
}

QSS_simulator retQSSTest::initialize(
		const std::string &geometry_path,
		retQSS::InitialConditionArray &ic_array,
		int n_particles)
{
	if(n_particles < 1)
		n_particles = ic_array.size();

	auto sim = retQSSTest::build_simulator(geometry_path, ic_array, n_particles);
	char *geom_path = const_cast<char*>(geometry_path.c_str());

	auto interface = RETQSS();

	interface->geometry_set_up(geom_path);
	interface->create_particle_dependent_objects(n_particles);
	interface->tracker()->create_particles(ic_array, geometry_path);

	return sim;
}

void retQSSTest::free_simulator(QSS_simulator sim)
{
	if(sim != nullptr)
		QSS_freeSimulator(sim);
}

void retQSSTest::run_model(bool is_parallel)
{
	SD_simulationSettings settings = SD_SimulationSettings();
	settings->debug = static_cast<SD_Debug>(0);
	settings->parallel = is_parallel;
	settings->hybrid = TRUE;
	settings->method = static_cast<SD_Solver>(3);

	SIM_simulator simulator = SIM_Simulator(settings);
	SIM_simulate(simulator);
	SIM_freeSimulator(simulator);
}

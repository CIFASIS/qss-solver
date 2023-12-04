#include <fstream>
#include <sstream>
#include <string>

#include "retqss_debug.h"
#include "retqss_cgal_main_types.hh"
#include "retqss_polyhedral_volume.hh"
#include "retqss_trajectory.hh"
#include "retqss_utilities.hh"

#ifdef RETQSS_DEBUG

#define POINTS_RATIO 100


void retQSS::trajectory_to_geomview(
		Internal_ParticleID particle,
		int index,
		retQSS::Volume *vol,
		retQSS::PointList *points,
		const char *path_str)
{
	std::string path = path_str;

	auto _vol = dynamic_cast<retQSS::PolyhedralVolume*>(vol);
	// Save current volume.
	if(_vol != nullptr)
		_vol->save_to_OFF(path);

	// Save trajectory.
	int n_points = points->size();
	std::stringstream trajectory_file, content;
	trajectory_file
		<< "trajectory-"
		<< particle << "-"
		<< index
		<< ".vect";

	std::ofstream trajectory_out(path + "/" + trajectory_file.str());

	content	<< "appearance { linewidth 7 }\n\n"
			<< "VECT\n1 " << n_points << " 1\n"
			<< n_points << " 1\n\n";

	for(auto time_point : *points)
		content
			<< time_point.second.x() << " "
			<< time_point.second.y() << " "
			<< time_point.second.z() << "\n";

	content << "\n0 0 1 0";

	trajectory_out << content.str();
	trajectory_out.close();

	// Save global file.
	std::stringstream global_file;
	global_file
		<< "global-"
		<< index
		<< ".list";
	std::ofstream global_out(path + "/" + global_file.str());

	global_out
			<< "appearance {\n"
			<< "+edge\n"
			<< "+transparent\n"
			<< "material { alpha 0.5 }\n"
			<< "}\n\n"
			<< "LIST\n"
			<< "< " << trajectory_file.str() << "\n";

	if(vol != nullptr)
			global_out
				<< "< " << vol->get_name() << ".off\n";

	global_out.close();
}

#endif

#include "retqss_stats.hh"
#include "retqss_exceptions.hh"

#include <fstream>

#ifdef RETQSS_STATS

retQSS::Stats::Stats()
{
	this->actual_crossings = 0;
	this->intersections_computed = 0;
	this->next_crossing_time_calls = 0;
	this->intersection_time = 0.;
}

retQSS::Stats::~Stats()
{
	this->dump();
}

void retQSS::Stats::dump() const
{
	std::ofstream log(RETQSS_STATS_LOG);
	if(!log.good())
		throw retQSS::FileAccessException(RETQSS_STATS_LOG);

	log.precision(12);

	double int_time_avg = this->intersection_time / this->intersections_computed;
	double int_time_avg_per_crossing = this->intersection_time / this->actual_crossings;

	log << "nextCrossingTime calls: " << this->next_crossing_time_calls << std::endl;
	log << "Crossings: " << this->actual_crossings << std::endl;
	log << "Intersections computed: " << this->intersections_computed << std::endl;

	log << "Intersection time: " << this->intersection_time << std::endl;
	log << "Intersection time avg.: " << int_time_avg << std::endl;
	log << "Intersection time avg. (per crossing): " << int_time_avg_per_crossing << std::endl;
}

#endif

#ifndef _RETQSS_STATS_H_
#define _RETQSS_STATS_H_

#ifdef RETQSS_STATS

#include <time.h>

#define RETQSS_STATS_LOG "retqss_stats.log"

#define RETQSS_GET_TIME(t0)   (clock_gettime(CLOCK_MONOTONIC, &t0))
#define RETQSS_TIME_SECS(s,t) ((t.tv_sec-s.tv_sec) + (t.tv_nsec-s.tv_nsec)/1e9)


namespace retQSS
{

struct Stats
{
	int actual_crossings;
	int intersections_computed;
	int next_crossing_time_calls;
	double intersection_time;

	Stats();
	~Stats();

	void dump() const;
};

}

#endif

#endif

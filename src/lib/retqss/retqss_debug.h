/**
 * \file retqss_debug.h
 * \brief Debugging functions
 *
 * Assorted debug functionality only compiled if flag RETQSS_DEBUG is defined
 * at compilation time.
 */

#ifndef _RETQSS_DEBUG_H_
#define _RETQSS_DEBUG_H_

#include "retqss_types.hh"
#include "retqss_volume.hh"

namespace retQSS
{

/**
 * \brief Save particle trajectories to Geomview files.
 *
 * Save particle trajectories for each volume traversed by particle \f$p\f$ in
 * a set of files compatible with Geomview.
 *
 * \param index Index of volume traversed.
 * \param volume Current volume.
 * \param points Sequence of points inside \c volume which compose \f$p\f$'s
 * trajectory inside \c volume.
 * \param path Path where files will be saved.
 */
void trajectory_to_geomview(
		retQSS::Internal_ParticleID p,
		int index,
		retQSS::Volume *volume,
		retQSS::PointList *points,
		const char *path =NULL);

}

#endif

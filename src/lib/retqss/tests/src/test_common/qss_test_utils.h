#ifndef _QSS_TEST_UTILS_H_
#define _QSS_TEST_UTILS_H_

#include <string>

#include "retqss/retqss_types.hh"

extern "C"
{
#include "common/simulator.h"
#include "qss/qss_frw.h"
#include "qss/qss_quantizer.h"
#include "qss/qss_scheduler.h"
#include "qss/qss_simulator.h"
}

#include "common/data.h"
#include "qss/qss_data.h"

namespace retQSSTest
{
QSS_simulator build_simulator(
		const std::string&,
		retQSS::InitialConditionArray&,
		int n =1);

QSS_simulator initialize(
		const std::string&,
		retQSS::InitialConditionArray&,
		int n =-1);

void free_simulator(QSS_simulator);

void run_model(bool =false);
}

#endif

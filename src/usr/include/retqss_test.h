#ifndef _RETQSS_TEST_H_
#define _RETQSS_TEST_H_

#include <retqss_types.hh>

#ifdef __cplusplus
extern "C"
{
#endif

Bool test_debug(
	double time,
	const char *format,
	int int1,
	int int2,
	double double1,
	double double2);

Bool test_arraySet(
	double *array,
	int indexFrom,
	int indexTo,
	double value);

#ifdef __cplusplus
}
#endif

#endif

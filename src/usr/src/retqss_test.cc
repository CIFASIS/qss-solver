#include "retqss_test.h"

#include <chrono>
#include <ctime>    

extern "C"
{

Bool test_debug(double time, const char *format, int int1, int int2, double double1, double double2)
{
    std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char * ct = std::ctime(&current_time);
    ct[strcspn(ct, "\n")] = '\0';
    printf("[%s] (t=%.2f) ", ct, time);
    printf(format, (int) int1, (int) int2, double1, double2);
    printf("\n");
	return true;
}


Bool test_arraySet(double *array, int indexFrom, int indexTo, double value)
{

	for (int index = indexFrom; index <= indexTo; index++) {
		array[index-1] = value;
	}
	return true;
}

}

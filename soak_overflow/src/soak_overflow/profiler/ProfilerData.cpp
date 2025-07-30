#include <soak_overflow/profiler/ProfilerData.h>

namespace soak_overflow::profiler
{
    // constructor *************************************************************
    ProfilerData::ProfilerData():
        // call
        call_count(0),
        // clock
        total_call_duration(std::chrono::high_resolution_clock::duration::zero())
    {}
}

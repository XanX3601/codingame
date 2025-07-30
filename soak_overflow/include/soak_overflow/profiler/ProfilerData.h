#ifndef SOAK_OVERFLOW_PROFILER_PROFILER_DATA_H
#define SOAK_OVERFLOW_PROFILER_PROFILER_DATA_H

#include <chrono>

namespace soak_overflow::profiler
{
    class ProfilerData
    {
    // call ********************************************************************
    public:
        long long call_count;

    // clock *******************************************************************
    public:
        std::chrono::high_resolution_clock::duration total_call_duration;

    // constructor *************************************************************
    public:
        ProfilerData();

        ProfilerData(const ProfilerData& cr_profiler_data) = default;

        ProfilerData(ProfilerData&& rv_profiler_data) noexcept = default;

        ~ProfilerData() noexcept = default;

    // operator ****************************************************************
    public:
        ProfilerData& operator=(const ProfilerData& cr_profiler_data) = default;

        ProfilerData& operator=(ProfilerData&& rv_profiler_data) noexcept = default;
    };
}

#endif

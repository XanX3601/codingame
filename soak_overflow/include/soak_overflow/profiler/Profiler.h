#ifndef SOAK_OVERFLOW_PROFILER_PROFIlER_H
#define SOAK_OVERFLOW_PROFILER_PROFIlER_H

#include <chrono>
#include <string>
#include <unordered_map>

#include <soak_overflow/profiler/ProfilerData.h>

namespace soak_overflow::profiler
{
    class Profiler
    {
    // constructor *************************************************************
    public:
        explicit Profiler(const std::string& cr_function_name);

        Profiler(const Profiler& cr_profiler) = delete;

        Profiler(Profiler&&) noexcept = delete;

        ~Profiler();

    // function ****************************************************************
    private:
        std::string __function_name;

        std::chrono::high_resolution_clock::time_point __function_start;

    // operator ****************************************************************
    public:
        Profiler& operator=(const Profiler& cr_profiler) = delete;

        Profiler& operator=(Profiler&& cr_profiler) noexcept = delete;

    // records *****************************************************************
    private:
        static std::unordered_map<std::string, ProfilerData> __records;

    // report ******************************************************************
    public:
        static void report_and_reset();
    };
}

#define PROFILE_SCOPE() soak_overflow::profiler::Profiler timer##__LINE__{__func__}

#endif

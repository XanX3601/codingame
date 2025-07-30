
#include <soak_overflow/profiler/Profiler.h>

#include <iostream>

namespace soak_overflow::profiler
{
    // constructor *************************************************************
    Profiler::Profiler(const std::string& cr_function_name):
        // function 
        __function_name(cr_function_name),
        __function_start(std::chrono::high_resolution_clock::now())
    {
        __records[__function_name].call_count++;
    }

    Profiler::~Profiler()
    {
        auto call_duration = std::chrono::high_resolution_clock::now() - __function_start;
        __records[__function_name].total_call_duration += call_duration;
    }

    // records *****************************************************************
    std::unordered_map<std::string, ProfilerData> Profiler::__records;

    // report ******************************************************************
    void Profiler::report_and_reset()
    {
        std::cerr << "=== Profiling Report ===" << std::endl;
        for (const auto& [function_name, data]: __records)
        {
            double total_call_duration_ms = (
                std::chrono::duration<double, std::milli>(data.total_call_duration).count()
            );
            std::cerr
                << function_name
                << " | " << data.call_count << " call"
                << " | " << total_call_duration_ms << "ms total"
                << " | " << total_call_duration_ms / data.call_count << "ms avg"
                << std::endl;
        }

        __records.clear();
    }
}


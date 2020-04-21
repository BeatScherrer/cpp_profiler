/**
 * @file InstrumentationTimer.cpp
 * @brief Definition of the InstrumentationTimer class.
 */

#include "cpp_profiling/InstrumentationTimer.h"
#include "cpp_profiling/Instrumentor.h"

#include <thread>

namespace cpp_profiling {

InstrumentationTimer::InstrumentationTimer(const char* name)
    : name_(name), stopped_(false) {
  start_timepoint_ = std::chrono::high_resolution_clock::now();
}

InstrumentationTimer::~InstrumentationTimer() {
  if (!stopped_) stop();
}

void InstrumentationTimer::stop() {
  auto end_timepoint = std::chrono::high_resolution_clock::now();

  long long start =
      std::chrono::time_point_cast<std::chrono::microseconds>(start_timepoint_)
          .time_since_epoch()
          .count();
  long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint)
                      .time_since_epoch()
                      .count();

  uint32_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
  Instrumentor::get().writeProfile({name_, start, end, thread_id});

  stopped_ = true;
}

} // namespace cpp_profiling
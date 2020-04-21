#pragma once

/**
 * @file InstrumentationTimer.h
 * @brief Declaration of the InstrumentationTimer class.
 */

#include "cpp_profiling/Instrumentor.h"

#include <chrono>

// TODO set to 0 by default
#define PROFILING 1

// check if profiling is enabled
#if PROFILING == 1
#define PROFILE_SCOPE(name) cpp_profiling::InstrumentationTimer timer##__LINE__(name)
// TODO is there a macro for function with signature for overrides?
#define PROFILE_FUNCTION PROFILE_SCOPE(__func__)
#else
// set macro to nothing
#define PROFILE_FUNCTION
#endif

namespace cpp_profiling {

/** Creates a timer on construction and stops it upon destruction. */
class InstrumentationTimer {
 public:
  /** Default constructor.
   *
   * @param name name of the timer.
   */
  InstrumentationTimer(const char* name);

  /** Default destructor. */
  ~InstrumentationTimer();

  /** Stops the timer. */
  void stop();

 private:
  /// Name of the timer.
  const char* name_;

  /// Start time of the timer.
  std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint_;

  /// Flag to show if the timer has been stopped.
  bool stopped_;
};

} // namespace cpp_profiling
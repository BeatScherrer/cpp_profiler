#pragma once

/**
 * @file Instrumentor.h
 * @brief Provides profiling utilities to be used with chrome tracer.
 *
 * Enable profiling with passing -DPROFILING=1 (may be conditional on other stuff, e.g.
 * cmake build type)
 *
 * Use as followed:
 * @code
 * void testFunction() {
 *   PROFILE_FUNCTION();
 *
 *   ...
 * }
 * @endcode
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>

#include <thread>

// TODO set to 0 by default
#define PROFILING 1

// check if profiling is enabled
#if PROFILING == 1
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION PROFILE_SCOPE(__func__)
#else
// set macro to nothing
#define PROFILE_FUNCTION
#endif

/** Holds the profiling results:
 * - name
 * - start-time, end-time
 * - thread id
 */
struct ProfileResult {
  std::string name;
  long long start, end;
  uint32_t thread_id;
};

/** A Instrumentation Session is defined by a name. */
struct InstrumentationSession {
  std::string name;
};


/** The Singleton Instrumentor creates sessions and manages the output of the InstrumentationTimer.
 *
 */
class Instrumentor {
 public:

  /** Begin a profiling session.
   *
   * @param name name of the session.
   * @param filepath Path to the file of the session.
   */
  void beginSession(const std::string& name,
                    const std::string& filepath = "results.json");

  /** terminates a profiling session. */
  void endSession();

  /** Writes a profile to a file. */
  void writeProfile(const ProfileResult& result);

  /** Writes the json header. */
  void writeHeader();

  /** Writes the json footer. */
  void writeFooter();

  /** Return the static instance of the Instrumentor. */
  static Instrumentor& get();

 private:

  /** Declare constructor private to get a singleton. */
  Instrumentor();

  /// Pointer to current session.
  std::unique_ptr<InstrumentationSession> current_session_;

  /// Current output stream.
  std::ofstream output_stream_;

  /// Profile counter.
  int profile_count_;

  /// Mutex to protect the file from concurrent writes.
  std::mutex file_mutex_;
};

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
#pragma once

/**
 * @file Instrumentor.h
 * @brief Provides profiling utilities to be used with chrome tracer.
 *
 * Enable profiling with passing -DPROFILING=1 (may be conditional on other stuff, e.g.
 * cmake build type)
 *
 * @example example.cpp
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <memory>


namespace cpp_profiling {

/** Datatype to hold the profiling results. */
struct ProfileResult {
  std::string name;
  long long start, end;
  uint32_t thread_id;
};

/** A Instrumentation Session is defined by a name. */
struct InstrumentationSession {
  std::string name;
};

/** The Singleton Instrumentor creates sessions and manages the output of the
 * InstrumentationTimer. Use as followed:
 * @code
 * void testFunction() {
 *   PROFILE_FUNCTION();
 *
 *   ...
 * }
 * @endcode
 *
 */
class Instrumentor {
 public:
  ~Instrumentor();

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

} // namespace cpp_profiling
#pragma once

/**
 * @file Instrumentor.h
 * @brief Provides profiling utilities to be used with chrome tracer.
 *
 * Enable profiling with passing -DPROFILING=1 (may be conditional on other stuff, e.g.
 * cmake build type)
 *
 * Use as follows:
 * void testFunction() {
 *   PROFILE_FUNCTION();
 *
 *   ...
 * }
 *
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>

#include <thread>

// check if profiling is enabled
#if PROFILING == 1
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION PROFILE_SCOPE(__FUNCSIG__)
#else
// set macro to nothing
#define PROFILE_SCOPE(name)
#endif

struct ProfileResult {
  std::string name;
  long long start, end;
  uint32_t thread_id;
};

struct InstrumentationSession {
  std::string name;
};

class Instrumentor {
 public:

  void beginSession(const std::string& name,
                    const std::string& filepath = "results.json");

  void endSession();

  void writeProfile(const ProfileResult& result);

  void writeHeader();
  void writeFooter();

  static Instrumentor& get();

 private:
  Instrumentor();

  std::unique_ptr<InstrumentationSession> current_session_;
  std::ofstream output_stream_;
  int profile_count_;

  std::mutex file_mutex_;
};

class InstrumentationTimer {
 public:
  InstrumentationTimer(const char* name);

  ~InstrumentationTimer();

  void stop();

 private:
  const char* name_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint_;
  bool stopped_;
};
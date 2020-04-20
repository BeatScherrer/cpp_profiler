/**
 * @file Instrumentor.cpp
 * @brief Definition of the Instrumentor class.
 *
 */

#include "cpp_profiling/Instrumentor.h"

// ------------------------------------------------------------------------------
// Instrumentor
// ------------------------------------------------------------------------------
Instrumentor::Instrumentor()
  : current_session_(nullptr), profile_count_(0) {}

void Instrumentor::beginSession(const std::string& name, const std::string& filepath) {
  output_stream_.open(filepath);
  writeHeader();
  current_session_ = std::make_unique<InstrumentationSession>();
  current_session_->name = name;
}

void Instrumentor::endSession() {
  writeFooter();
  output_stream_.close();
  current_session_ = nullptr;
  profile_count_ = 0;
}

void Instrumentor::writeProfile(const ProfileResult& result) {
  std::lock_guard<std::mutex> lock(file_mutex_);

  if (profile_count_++ > 0) {
    output_stream_ << ",";
  }

  std::string name = result.name;
  std::replace(name.begin(), name.end(), '"', '\'');

  output_stream_ << "{";
  output_stream_ << "\"cat\":\"function\",";
  output_stream_ << "\"dur\":" << (result.end - result.start) << ',';
  output_stream_ << "\"name\":\"" << name << "\",";
  output_stream_ << "\"ph\":\"x\",";
  output_stream_ << "\"pid\":0,";
  output_stream_ << "\"tid\":" << result.thread_id << ",";
  output_stream_ << "\"ts\":" << result.start;
  output_stream_ << "}";

  output_stream_.flush();
}

void Instrumentor::writeHeader() {
  std::lock_guard<std::mutex> lock(file_mutex_);

  output_stream_ << "{\"otherData\": {},\"traceEvents\":[";
  output_stream_.flush();
}

void Instrumentor::writeFooter() {
  std::lock_guard<std::mutex> lock(file_mutex_);

  output_stream_ << "]}";
  output_stream_.flush();
}

Instrumentor& Instrumentor::get() {
  static Instrumentor instance;
  return instance;
}

// ------------------------------------------------------------------------------
// Instrumentation Timer
// ------------------------------------------------------------------------------
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

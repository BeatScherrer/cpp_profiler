/**
 * @file Instrumentor.cpp
 * @brief Definition of the Instrumentor class.
 *
 */

#include "cpp_profiling/Instrumentor.h"
#include "glog/logging.h"

namespace cpp_profiling {

Instrumentor::Instrumentor() : current_session_(nullptr), profile_count_(0) {}

Instrumentor::~Instrumentor() { endSession(); }

void Instrumentor::beginSession(const std::string& name, const std::string& filepath) {
  LOG(INFO) << "beginning session '" << "name" << "'" << std::endl;

  output_stream_.open(filepath);

  // sanity check
  if (output_stream_.is_open()) {
    writeHeader();
    current_session_ = std::make_unique<InstrumentationSession>();
    current_session_->name = name;
  } else {
    LOG(ERROR) << "could not open file " << filepath << std::endl;
  }
}

void Instrumentor::endSession() {
  LOG(INFO) << "ending session '" << current_session_->name << "'" << std::endl;

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
  output_stream_ << "\"ph\":\"X\",";
  output_stream_ << "\"pid\":0,";
  output_stream_ << "\"tid\":" << result.thread_id << ",";
  output_stream_ << "\"ts\":" << result.start;
  output_stream_ << "}";

  output_stream_.flush();
}

void Instrumentor::writeHeader() {
  LOG(INFO) << "writing json header" << std::endl;

  std::lock_guard<std::mutex> lock(file_mutex_);

  output_stream_ << "{\"otherData\": {},\"traceEvents\":[";
  output_stream_.flush();
}

void Instrumentor::writeFooter() {
  LOG(INFO) << "writing json footer" << std::endl;

  std::lock_guard<std::mutex> lock(file_mutex_);

  output_stream_ << "]}";
  output_stream_.flush();
}

Instrumentor& Instrumentor::get() {
  static Instrumentor instance;
  return instance;
}

}  // namespace cpp_profiling

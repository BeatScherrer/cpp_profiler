/**
 * @file example.cpp
 *
 * @example
 */

#include <cpp_profiling/Instrumentor.h>

#include <chrono>
#include <thread>

void simple_function() {

  std::this_thread::sleep_for(std::chrono::seconds(10));
}

int main(int argc, char** argv) {
  cpp_profiling::Instrumentor::get().beginSession("example-session");

  simple_function();

  cpp_profiling::Instrumentor::get().endSession();

  return 0;
}
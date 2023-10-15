#include "procs.h"

#include <filesystem>
#include <memory>
#include <vector>

#include <sys/sysinfo.h>

namespace {

bool is_pid_directory(const std::string& entry) {
  for (auto& character: entry) {
    if (!std::isdigit(character)) {
      return false;
    }
  }
  return true;
}

} // namespace

namespace hightop {

int count_processes() {
  struct sysinfo info;
  if (sysinfo(&info) != 0) {
    throw std::runtime_error("sysinfo failed");
  }
  return info.procs;
}

std::vector<Process> get_running_processes() {
  std::vector<Process> processes;

  // iterate each file in the /proc directory looking for PID
  for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
    const auto filename = entry.path().filename().string();

    // if the directory entry is not a PID directory, skip to the next entry.
    if (!is_pid_directory(filename)) {
      continue;
    }

    // costruct a Process instance from the PID directory
    auto process = [&]() -> std::unique_ptr<Process> {
      try {
        return std::make_unique<Process>(std::stoi(filename));
      }
      catch (const std::exception& e) {
        // Unable to create Process. This typically happens when the process has ended
        // before all of its information could be read.
        return nullptr;
      }
    }();

    if (!process) {
      // if the process was not initialized, it means it has ended and should not be included
      continue;
    }

    if (process->is_kernel_process()) {
      // if the process is a kernel process (i.e. has no command), it should not be included
      continue;
    }

    processes.emplace_back(std::move(*process.release()));
  }

  return processes;
}

} // namespace hightop
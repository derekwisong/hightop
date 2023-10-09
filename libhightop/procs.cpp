#include "procs.h"

#include <filesystem>
#include <vector>

#include <sys/sysinfo.h>


namespace hightop {

int count_processes() {
  struct sysinfo info;
  if (sysinfo(&info) != 0) {
    throw std::runtime_error("sysinfo failed");
  }
  return info.procs;
}

bool is_pid_directory(const std::string& entry) {
  for (auto& character: entry) {
    if (!std::isdigit(character)) {
      return false;
    }
  }
  return true;
}

std::vector<Process> get_processes() {
  std::vector<Process> processes;

  for (const auto& path : std::filesystem::directory_iterator("/proc")) {
    const auto filename = path.path().filename().string();

    if (!is_pid_directory(filename)) {
      continue;
    }

    int pid = std::stoi(filename);
    Process process{pid};
    processes.emplace_back(process);
  }

  return processes;
}


std::ostream& operator<<(std::ostream& os, const Process& proc)
{
    os << "Process<" << proc.pid << ">";
    return os;
}

} // namespace hightop
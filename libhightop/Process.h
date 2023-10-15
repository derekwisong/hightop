#pragma once
#include <iostream>
#include <optional>

#include "ProcessStats.h"

namespace hightop {

class Process {
public:
  Process(int pid);
  Process(const Process &) = default;
  Process(Process &&) = default;
  Process &operator=(const Process &) = default;
  Process &operator=(Process &&) = default;

  friend std::ostream &operator<<(std::ostream &os, const Process &dt);

  int get_pid() const { return pid; }
  void set_pid(int pid) { this->pid = pid; }

  std::optional<std::string> get_command() const { return command; }
  void set_command(std::string command) { this->command = std::move(command); }

  const ProcessStats& get_stats() const { return stats; }
  bool is_kernel_process() const { return kernel_process; }

private:
  int pid;
  int uid;
  std::optional<std::string> command;
  bool kernel_process;
  ProcessStats stats;
};

} // namespace hightop
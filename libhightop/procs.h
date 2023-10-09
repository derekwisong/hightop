#pragma once

#include <iostream>
#include <vector>

namespace hightop {

class Process {
  public:
    Process(int pid) : pid(pid) {}
    Process(const Process&) = default;
    Process(Process&&) = default;
    Process& operator=(const Process&) = default;
    Process& operator=(Process&&) = default;

    int get_pid() const { return pid; }
    friend std::ostream& operator<<(std::ostream& os, const Process& dt);
  private:
    int pid;
};


int count_processes();
std::vector<Process> get_processes();

}  // namespace hightop
#pragma once

#include <iostream>
#include <optional>
#include <vector>

namespace hightop {

/* Content from /proc/$pid/stat */
struct ProcessStats {
  int pid;
  std::string comm;
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  unsigned flags;
  unsigned long minflt;
  unsigned long cminflt;
  unsigned long majflt;
  unsigned long cmajflt;
  unsigned long utime;
  unsigned long stime;
  long cutime;
  long cstime;
  long priority;
  long nice;
  long num_threads;
  long itrealvalue;
  unsigned long long starttime;
  unsigned long vsize;
  long rss;
  unsigned long rsslim;
  unsigned long startcode;
  unsigned long endcode;
  unsigned long startstack;
  unsigned long kstkesp;
  unsigned long kstkeip;
  unsigned long signal;
  unsigned long blocked;
  unsigned long sigignore;
  unsigned long sigcatch;
  unsigned long wchan;
  unsigned long nswap;
  unsigned long cnswap;
  int exit_signal;
  int processor;
  unsigned rt_priority;
  unsigned policy;
  unsigned long long delayacct_blkio_ticks;
  unsigned long guest_time;
  long cguest_time;
  unsigned long start_data;
  unsigned long end_data;
  unsigned long start_brk;
  unsigned long arg_start;
  unsigned long arg_end;
  unsigned long env_start;
  unsigned long env_end;
  int exit_code;
};

class Process {
  public:
    Process(int pid);
    Process(const Process&) = default;
    Process(Process&&) = default;
    Process& operator=(const Process&) = default;
    Process& operator=(Process&&) = default;

    friend std::ostream& operator<<(std::ostream& os, const Process& dt);

    int get_pid() const { return pid; }
    void set_pid(int pid) { this->pid = pid; }

    std::optional<std::string> get_command() const { return command; }
    void set_command(std::string command) { this->command = std::move(command); }

    std::optional<int> get_niceness() const { return niceness; }
    void set_niceness(int niceness) { this->niceness = niceness; }

    std::optional<int> get_priority() const { return priority; }
    void set_priority(int priority) { this->priority = priority; }

    std::optional<int> get_uid() const { return uid; }
    void set_uid(int uid) { this->uid = uid; }

    std::optional<long> get_virtual_memory() const { return virtual_memory; }
    void set_virtual_memory(long virtual_memory) { this->virtual_memory = virtual_memory; }

    std::optional<long> get_resident_memory() const { return resident_memory; }
    void set_resident_memory(long resident_memory) { this->resident_memory = resident_memory; }

    std::optional<long> get_shared_memory() const { return shared_memory; }
    void set_shared_memory(long shared_memory) { this->shared_memory = shared_memory; }

    std::optional<std::string> get_state() const { return state; }
    void set_state(std::string state) { this->state = std::move(state); }

  private:
    int pid;
    std::optional<std::string> command;
    std::optional<std::string> state;
    std::optional<int> niceness;
    std::optional<int> priority;
    std::optional<int> uid;
    std::optional<long> virtual_memory;
    std::optional<long> resident_memory;
    std::optional<long> shared_memory;
    std::optional<long> num_threads;
};

int count_processes();
std::vector<Process> get_running_processes();

}  // namespace hightop
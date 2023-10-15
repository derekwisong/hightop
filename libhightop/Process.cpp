#include "Process.h"
#include "fs.h"

#include <filesystem>
#include <fstream>

#include <sys/stat.h>


namespace {

ProcessStats read_process_stats(const std::filesystem::path& path) {
  // return a struct instance with tokens pared from the first line of path
  ProcessStats stats;
  std::ifstream stat_file{path};
  stat_file >> stats.pid;
  stat_file >> stats.comm;
  stat_file >> stats.state;
  stat_file >> stats.ppid;
  stat_file >> stats.pgrp;
  stat_file >> stats.session;
  stat_file >> stats.tty_nr;
  stat_file >> stats.tpgid;
  stat_file >> stats.flags;
  stat_file >> stats.minflt;
  stat_file >> stats.cminflt;
  stat_file >> stats.majflt;
  stat_file >> stats.cmajflt;
  stat_file >> stats.utime;
  stat_file >> stats.stime;
  stat_file >> stats.cutime;
  stat_file >> stats.cstime;
  stat_file >> stats.priority;
  stat_file >> stats.nice;
  stat_file >> stats.num_threads;
  stat_file >> stats.itrealvalue;
  stat_file >> stats.starttime;
  stat_file >> stats.vsize;
  stat_file >> stats.rss;
  stat_file >> stats.rsslim;
  stat_file >> stats.startcode;
  stat_file >> stats.endcode;
  stat_file >> stats.startstack;
  stat_file >> stats.kstkesp;
  stat_file >> stats.kstkeip;
  stat_file >> stats.signal;
  stat_file >> stats.blocked;
  stat_file >> stats.sigignore;
  stat_file >> stats.sigcatch;
  stat_file >> stats.wchan;
  stat_file >> stats.nswap;
  stat_file >> stats.cnswap;
  stat_file >> stats.exit_signal;
  stat_file >> stats.processor;
  stat_file >> stats.rt_priority;
  stat_file >> stats.policy;
  stat_file >> stats.delayacct_blkio_ticks;
  stat_file >> stats.guest_time;
  stat_file >> stats.cguest_time;
  stat_file >> stats.start_data;
  stat_file >> stats.end_data;
  stat_file >> stats.start_brk;
  stat_file >> stats.arg_start;
  stat_file >> stats.arg_end;
  stat_file >> stats.env_start;
  stat_file >> stats.env_end;
  stat_file >> stats.exit_code;
  return stats;
}

int read_process_uid(const std::filesystem::path& path) {
  struct stat statbuf;
  if (stat(path.c_str(), &statbuf) != 0) {
    throw std::runtime_error("stat failed");
  }
  return statbuf.st_uid;
}


} // namespace

namespace hightop {

Process::Process(int pid) : pid{pid} {
  constexpr int KERNEL_THREAD_ID = 2;
  auto procfs_path = std::filesystem::path{"/proc"} / std::to_string(pid);
  command = read_line_from_file(procfs_path / "cmdline");
  stats = read_process_stats(procfs_path / "stat");
  kernel_process = command->empty() || stats.ppid == KERNEL_THREAD_ID;
  uid = read_process_uid(procfs_path);
}

std::ostream& operator<<(std::ostream& os, const Process& proc)
{
    os << "Process<" << proc.pid << ">: " << proc.stats.comm;
    return os;
}

} // namespace hightop
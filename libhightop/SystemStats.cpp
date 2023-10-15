#include "SystemStats.h"
#include "fs.h"
#include <fstream>

namespace {

} // namespace

namespace hightop {

SystemStats::SystemStats() {
  {
    std::ifstream uptime_file{"/proc/uptime"};
    uptime_file >> uptime;
    uptime_file >> idle_time;
  }

  {
    std::ifstream loadavg_file{"/proc/loadavg"};
    loadavg_file >> load_1_min;
    loadavg_file >> load_5_min;
    loadavg_file >> load_15_min;
    loadavg_file >> num_runnable_entities;
    loadavg_file >> num_entities;
    loadavg_file >> recent_pid;
  }
}

} // namespace hightop
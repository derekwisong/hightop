#pragma once

namespace hightop {
  class SystemStats {
    public:
    SystemStats();
    SystemStats(const SystemStats&) = default;
    SystemStats(SystemStats&&) = default;
    SystemStats& operator=(const SystemStats&) = default;
    SystemStats& operator=(SystemStats&&) = default;

    int get_load_1_min() const { return load_1_min; }
    int get_load_5_min() const { return load_5_min; }
    int get_load_15_min() const { return load_15_min; }
    int get_num_runnable_entities() const { return num_runnable_entities; }
    int get_num_entities() const { return num_entities; }
    int get_recent_pid() const { return recent_pid; }

    int get_uptime() const { return uptime; }
    int get_idle_time() const { return idle_time; }

    private:
    // fields from /proc/loadavg
    int load_1_min; // load average over the last minute
    int load_5_min; // load average over the last 5 minutes
    int load_15_min; // load average over the last 15 minutes
    int num_runnable_entities; // number of runnable entities (processes, threads)
    int num_entities; // number of entities (processes, threads)
    int recent_pid; // PID of the most recently created process

    // fields from /proc/uptime
    int uptime; // number of seconds since boot
    int idle_time; // number of seconds spent in idle process
  };
}
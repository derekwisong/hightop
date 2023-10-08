
#include "procs.h"

#include <sys/sysinfo.h>

#include <stdexcept>

namespace hightop {

int count_processes() {
  struct sysinfo info;
  if (sysinfo(&info) != 0) {
    throw std::runtime_error("sysinfo failed");
  }
  return info.procs;
}

} // namespace hightop
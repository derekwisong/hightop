#pragma once

#include <iostream>
#include <optional>
#include <vector>

#include "Process.h"

namespace hightop {

int count_processes();
std::vector<Process> get_running_processes();

}  // namespace hightop
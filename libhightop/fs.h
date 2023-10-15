#pragma once

#include <filesystem>
#include <string>

namespace hightop {

std::string read_line_from_file(const std::filesystem::path& path);

} // namespace hightop
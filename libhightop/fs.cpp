#include <fstream>

#include "fs.h"

namespace hightop {

std::string read_line_from_file(const std::filesystem::path& path) {
  std::ifstream file{path};
  std::string contents;
  std::getline(file, contents);
  return contents;
}

} // namespace hightop
#pragma once
#include <ftxui/component/component_base.hpp>

#include <vector>
#include <memory>

namespace hightop::ui {

ftxui::Component InteractiveTable(const std::vector<ftxui::Components>& rows, std::shared_ptr<int> selected_row);
ftxui::Component InteractiveTable(const std::vector<ftxui::Components>& rows, std::vector<std::string> columns, std::shared_ptr<int> selected_row);

}
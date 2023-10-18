#include "interactive-table.h"

#include <iostream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

namespace hightop::ui {

namespace {
  class Impl : public ftxui::ComponentBase {

    // A line is a focusable container of cells
    class Line : public ftxui::ComponentBase {
      public:
      Line(ftxui::Components cells) {
        for (const auto& cell : cells) {
          Add(cell);
        }
      }

      bool Focusable() const final { return true; }

      private:
      ftxui::Element Render() override {
        using namespace ftxui;

        std::vector<Element> elements;
        for (size_t i = 0; i < ChildCount(); i++) {
          Element elem = ChildAt(i)->Render();
          if (Focused()) {
            elem = std::move(elem) | inverted | focus;
          }
          elements.push_back(elem);
        }

        return hbox(std::move(elements));
      }
    };

    public:

    Impl(const std::vector<ftxui::Components>& rows, std::vector<std::string> columns, std::shared_ptr<int> selected_row) : selected_row(selected_row), columns(std::move(columns)) {
      ftxui::Components lines;
      for (const auto& row : rows) {
        lines.push_back(ftxui::Make<Line>(row));
      }
      Add(ftxui::Container::Vertical(lines, selected_row.get()));
    }

    Impl(const std::vector<ftxui::Components>& rows, std::shared_ptr<int> selected_row) : Impl(rows, {}, std::move(selected_row)) {}
    private:

    ftxui::Element Render() override {
      using namespace ftxui;
      auto vertical = Container::Vertical({});
      
      if (!columns.empty()) {
        Components header_cells;
        for (const auto& column : columns) {
          header_cells.push_back(Renderer([&]{
            return text(column);
          }));
        }
        auto header_row = Container::Horizontal(header_cells);
        return vbox({
          header_row->Render(),
          ComponentBase::Render(),
        });
      }

      return ComponentBase::Render();
    }

    std::shared_ptr<int> selected_row;
    std::vector<std::string> columns;
  };
} // namespace 

ftxui::Component InteractiveTable(const std::vector<ftxui::Components>& rows, std::shared_ptr<int> selected_row) {
  return ftxui::Make<Impl>(rows, std::move(selected_row));
}

ftxui::Component InteractiveTable(const std::vector<ftxui::Components>& rows, std::vector<std::string> columns, std::shared_ptr<int> selected_row) {
  return ftxui::Make<Impl>(rows, std::move(columns), std::move(selected_row));
}


} // namespace hightop::ui
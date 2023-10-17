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
    Impl(const std::vector<ftxui::Components>& rows, std::shared_ptr<int> selected_row) : rows(rows), selected_row(selected_row) {
      ftxui::Components lines;
      for (const auto& row : rows) {
        lines.push_back(ftxui::Make<Line>(row));
      }
      Add(ftxui::Container::Vertical(lines, selected_row.get()));
    }

    private:

    ftxui::Element Render() override {
      return ComponentBase::Render();
    }
    const std::vector<ftxui::Components>& rows;
    std::shared_ptr<int> selected_row;
  };
} // namespace 

ftxui::Component InteractiveTable(const std::vector<ftxui::Components>& rows, std::shared_ptr<int> selected_row) {
  return ftxui::Make<Impl>(rows, std::move(selected_row));
}

} // namespace hightop::ui
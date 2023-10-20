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

    Impl(const std::vector<ftxui::Components>& rows, std::vector<std::string> columns, std::shared_ptr<int> selected_row) : rows(rows),  columns(std::move(columns)), selected_row(selected_row) {
      lines_container = ftxui::Container::Vertical({}, selected_row.get());
      Add(lines_container);
      update_children();
    }
    Impl(const std::vector<ftxui::Components>& rows, std::shared_ptr<int> selected_row) : Impl(rows, {}, std::move(selected_row)) {
      
    }

    virtual bool OnEvent(ftxui::Event event) override {
      if (event == ftxui::Event::Custom) {
        // update the children of the component, but don't mark the event as handled
        update_children();
      }
      return false;
    }

    private:
    const std::vector<ftxui::Components>& rows;
    std::vector<std::string> columns;
    std::shared_ptr<int> selected_row;

    ftxui::Component lines_container;
  
    virtual ftxui::Element Render() override {
      using namespace ftxui;
      constexpr int CELL_PAD = 2;
      //constexpr int ROW_PAD = 1;

      // the row height of each row
      std::vector<int> row_heights = std::vector(lines_container->ChildCount(), 0);
      // the width of each column
      std::vector<int> column_widths = std::vector(columns.size(), 0); // note bug if columns is empty

      // determine the necessary column widths and row heights
      for (size_t line_i = 0; line_i < lines_container->ChildCount(); ++line_i) {
        auto line = lines_container->ChildAt(line_i);
        for (size_t cell_i = 0; cell_i < line->ChildCount(); ++cell_i) {
          auto cell = line->ChildAt(cell_i);
          auto elem = cell->Render();  // note save this rendered result
          elem->ComputeRequirement();
          column_widths[cell_i] = std::max(column_widths[cell_i], elem->requirement().min_x);
          row_heights[line_i] = std::max(row_heights[line_i], elem->requirement().min_y);
        }
      }

      Elements column_elements;
      for (size_t column_i = 0; column_i < columns.size(); ++column_i) {
        auto elem = text(columns.at(column_i));
        elem->ComputeRequirement();
        column_widths[column_i] = CELL_PAD + std::max(column_widths[column_i], elem->requirement().min_x);
        // add the size to the column header
        auto column_size = column_widths.at(column_i);
        if (column_i == columns.size() - 1) {
          elem |= flex;
        }
        else {
          elem |= size(WIDTH, EQUAL, column_size);
        }
        column_elements.push_back(std::move(elem));
      }

      // auto rows_renderer = Renderer(lines_container, [&] {
      //   Elements row_elements;
      //   for (size_t line_i = 0; line_i < lines_container->ChildCount(); ++line_i) {
      //     auto line = lines_container->ChildAt(line_i);
      //     Elements line_elements;
      //     for (size_t cell_i = 0; cell_i < line->ChildCount(); ++cell_i) {
      //       auto cell = line->ChildAt(cell_i);
      //       auto elem = cell->Render();  // note save this rendered result
      //       if (cell_i == columns.size() - 1) {
      //       elem |= flex;
      //       }
      //       else {
      //         auto column_size = column_widths.at(cell_i);
      //         elem |= size(WIDTH, EQUAL, column_size);
      //       }
      //       line_elements.push_back(std::move(elem));
      //     }
      //     row_elements.push_back(hbox(std::move(line_elements)));
      //   }
      //   return vbox({row_elements});
      // });


      return vbox({
        bgcolor(Color::Blue, hbox(column_elements)),
        //rows_renderer->Render() | vscroll_indicator | yframe,
        ComponentBase::Render() | vscroll_indicator | yframe
      });
    }

    void update_children() {
      lines_container->DetachAllChildren();
      for (const auto& row : rows) {
        auto line = Make<Line>(row);
        lines_container->Add(std::move(line));
      }
    }
  };
} // namespace 

ftxui::Component InteractiveTable(const std::vector<ftxui::Components>& rows, std::shared_ptr<int> selected_row) {
  return ftxui::Make<Impl>(rows, std::move(selected_row));
}

ftxui::Component InteractiveTable(const std::vector<ftxui::Components>& rows, std::vector<std::string> columns, std::shared_ptr<int> selected_row) {
  return ftxui::Make<Impl>(rows, std::move(columns), std::move(selected_row));
}


} // namespace hightop::ui
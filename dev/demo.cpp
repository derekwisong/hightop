#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
struct Record {
  int id;
  std::string name;
};

class RecordRowImpl: public ftxui::ComponentBase {
  public:
  RecordRowImpl(Record rec): record(std::move(rec)) {};

  virtual bool Focusable() const override {
    return true;
  }

  virtual ftxui::Element Render() override {
    using namespace ftxui;

    auto elem = hbox({
      text(std::to_string(record.id)),
      text(record.name),
    });

    if (Focused()) {
      elem |= inverted;
    }
  
    return elem;
  }

  private:
  Record record;
};


ftxui::Component RecordRow(Record rec) {
  return ftxui::Make<RecordRowImpl>(rec);
}

class RecordTableImpl : public ftxui::ComponentBase {
  public:
  RecordTableImpl(std::shared_ptr<std::vector<Record>> records, std::vector<std::string> columns, int* selected_row) : records(std::move(records)), columns(std::move(columns)) {
    container = ftxui::Container::Vertical({}, selected_row);
    Add(container);
  }

  virtual ftxui::Element Render() override {
    container->DetachAllChildren();
    for (const auto& rec : *records) {
      container->Add(RecordRow(rec));
    }

    return container->Render();

    // iterate over each child and make a vector of elements of the rendered cells for each row
    std::vector<ftxui::Elements> rows;
    for (size_t i = 0; i < container->ChildCount(); ++i) {
      ftxui::Elements cells;
      auto row = container->ChildAt(i);
      for (size_t j = 0; j < columns.size(); ++j) {
        // cells.push_back(row->ChildAt(j)->Render());
      }
      rows.push_back(std::move(cells));
    }

    return ftxui::gridbox(rows);
  }

  private:
  ftxui::Component container;
  std::shared_ptr<std::vector<Record>> records;
  std::vector<std::string> columns;
};


ftxui::Component RecordTable(std::shared_ptr<std::vector<Record>> records, std::vector<std::string> columns, int* selected_row) {
  return ftxui::Make<RecordTableImpl>(std::move(records), std::move(columns), selected_row);
}

}

int main() {
  int selected_row = 0;  // using int instead of size_t because FTXUI expects it
  auto records = std::make_shared<std::vector<Record>>();
  auto columns = std::vector<std::string>({"ID", "Name"});

  records->push_back(Record{.id=0, .name="Thing 1"});
  records->push_back(Record{.id=1, .name="Thing 2"});
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  auto table = RecordTable(records, columns, &selected_row);

  auto table_renderer = ftxui::Renderer(table, [&] { 
    return ftxui::vbox({
      ftxui::text("Selected row: " + std::to_string(selected_row)),
      table->Render()
    });
  });

  screen.Loop(table_renderer);
  return EXIT_SUCCESS;
}
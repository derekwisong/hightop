#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <iostream>
#include <vector>

#include "hightop/procs.h"
 
int main(void) {
  using namespace ftxui;
  auto status_label = std::string{"Processes: "} + std::to_string(hightop::count_processes());
  auto get_proc_table = []{
    auto procs = hightop::get_processes();
    std::vector<std::vector<std::string>> rows;
    std::transform(procs.begin(), procs.end(), std::back_inserter(rows), [](auto& proc){
      return std::vector<std::string>{"Process " + std::to_string(proc.get_pid())};});
    return Table(rows);
  };

  auto table = get_proc_table();
  table.SelectAll().Border();

  // Define the document
  Element document =
    vbox({
      hbox({
        text(status_label)   | border,
        text("middle") | border | flex,
        text("right")  | border,
      }),
      filler(),
      vbox({
        table.Render() | flex,
        filler(),
      }),
      filler(),
      hbox({text("footer")})
    });
 
  auto screen = Screen::Create(
    Dimension::Full(),       // Width
    Dimension::Fit(document) // Height
  );
  Render(screen, document);
  screen.Print();
 
  return EXIT_SUCCESS;
}
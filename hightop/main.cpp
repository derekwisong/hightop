#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <iostream>

#include "hightop/procs.h"
 
int main(void) {
  using namespace ftxui;
 
  // Define the document
  Element document =
    hbox({
      text("Processes: " + std::to_string(hightop::count_processes()))   | border,
      text("middle") | border | flex,
      text("right")  | border,
    });
 
  auto screen = Screen::Create(
    Dimension::Full(),       // Width
    Dimension::Fit(document) // Height
  );
  Render(screen, document);
  screen.Print();
 
  return EXIT_SUCCESS;
}
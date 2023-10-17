#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui-grid-container/grid-container.hpp"

#include <string>
using namespace ftxui;

namespace {
template<typename Type> ftxui::Component Label(const Type& value, bool flexible = false);

template<typename Type>
ftxui::Component Label(const Type& value, bool flexible) {
  return Label(std::to_string(value), flexible);
}

template<> ftxui::Component Label<std::string>(const std::string& value, bool flexible) {
  return ftxui::Renderer([flexible, value] {
    return ftxui::text(value) | (flexible ? ftxui::flex : ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10));
  });
}
}

int main() {
  auto button0 = Label(std::string("0"));
  auto button1 = Label(std::string("1"));
  auto button2 = Label(std::string("2"));
  auto button3 = Label(std::string("3"));

  auto grid = GridContainer({
    { button0, button1 },
    { button2, button3 },
  });

  auto screen = ScreenInteractive::Fullscreen();
  auto renderer = Renderer(grid, [&] {
    return grid->Render() | border;
  });

  screen.Loop(renderer);
  return 0;
}

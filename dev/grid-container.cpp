#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui-grid-container/grid-container.hpp>
#include <string>

#include "hightop/procs.h"

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
} // namespace

int main() {
  auto procs = hightop::get_running_processes();
  int selected_row = 0, selected_col = 0;
  std::vector<ftxui::Components> grid_items;
  for (const auto& proc : procs) {
    const auto& stats = proc.get_stats();
    if (proc.is_kernel_process() || stats.ppid == 2) continue;
    ftxui::Components row;
    row.push_back(Label(proc.get_pid()));
    row.push_back(Label(stats.nice));
    row.push_back(Label(stats.priority));
    row.push_back(Label(stats.vsize));
    row.push_back(Label(stats.rss));
    row.push_back(Label(std::string(1, stats.state)));
    row.push_back(Label(stats.stime));
    row.push_back(Label(stats.comm, true));
    grid_items.push_back(row);
  }
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  auto grid = ftxui::GridContainer(grid_items, &selected_row, &selected_col);

  std::atomic<int> frame_counter = 0;

  auto renderer = ftxui::Renderer(grid, [&] {
    using namespace ftxui;
    screen.Post([&] { frame_counter++; });

    Elements grid_elements;
    // for each child in grid
    auto child = grid->ChildAt(0);
    for (size_t i = 0; i < child->ChildCount(); ++i) {
      auto row = child->ChildAt(i);
      if (static_cast<int>(i) == selected_row) {
        grid_elements.push_back(row->Render() | inverted);
      }
      else {
        grid_elements.push_back(row->Render());
      }
    }

    return 
      vbox({
        hbox({
          text("x = " + std::to_string(selected_row)),
          filler() | size(WIDTH, EQUAL, 2),
          text("y = " + std::to_string(selected_col)),
          filler() | size(WIDTH, EQUAL, 2),
          text("frame = " + std::to_string(frame_counter)),
        }),
        vbox(grid_elements) | yframe | vscroll_indicator | flex,
      }) | border;
  });

  renderer |= ftxui::CatchEvent([&](ftxui::Event event) {
    if (event == ftxui::Event::ArrowLeft) {
      if (selected_col > 0) {
        selected_col--;
        return true;
      }
    }
    else if (event == ftxui::Event::ArrowRight) {
      selected_col++;
      return true;
    }
    else if (event == ftxui::Event::ArrowUp) {
      if (selected_row > 0) {
        selected_row--;
        return true;
      }
    }
    else if (event == ftxui::Event::ArrowDown) {
      selected_row++;
      return true;
    }
    else if (event == ftxui::Event::Character('q')) {
      screen.ExitLoopClosure()();
      return true;
    }

    return false;
  });

  screen.Loop(renderer);
  return EXIT_SUCCESS;
}
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <ftxui-grid-container/grid-container.hpp>

#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#include "hightop/procs.h"
#include "hightop/SystemStats.h"
#include "libhightop/SystemStats.h"

namespace {

std::vector<std::vector<std::string>> get_processes() {
  auto procs = hightop::get_running_processes();
  std::vector<std::vector<std::string>> rows;
  rows.push_back({"PID",  "NICE", "PRIO", "VIRT", "RES", "STATE", "STIME", "COMMAND"});
  std::transform(
    procs.begin(),
    procs.end(),
    std::back_inserter(rows),
    [](const auto& proc){
      const auto& stats = proc.get_stats();
      return std::vector<std::string>({
        std::to_string(proc.get_pid()),
        std::to_string(stats.nice),
        std::to_string(stats.priority),
        std::to_string(stats.vsize),
        std::to_string(stats.rss),
        std::string(1, stats.state),
        std::to_string(stats.stime),
        stats.comm
      });
    });
  return rows;
}
}

int main(void) {
  using namespace ftxui;
  hightop::SystemStats system_stats;
  auto processes = get_processes();
  auto screen = ScreenInteractive::Fullscreen();

  auto renderer = Renderer([&] {
    std::vector<ftxui::Components> process_rows;
    for (const auto& process : processes) {
      ftxui::Components row;
      for (auto iter = process.begin(); iter != process.end(); iter++) {
        auto val = *iter;
        auto decorator = (iter == process.end() - 1) ? ftxui::flex : ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10);
        row.push_back(ftxui::Renderer([val, decorator]{return text(val) | decorator;}));
      }
      process_rows.emplace_back(std::move(row));
    }
    auto grid = ftxui::GridContainer(process_rows);
    Table table(processes);
    table.SelectRow(0).BorderBottom(BorderStyle::DOUBLE);

    return vbox({
      hbox({
        text("Processes: " + std::to_string(processes.size())),
        separator(),
        text(
          "Load avg.: " 
          + std::to_string(system_stats.get_load_1_min())
          + " "
          + std::to_string(system_stats.get_load_5_min())
          + " "
          + std::to_string(system_stats.get_load_15_min())
        ),
        separator(),
        text(std::to_string(system_stats.get_uptime())),
        separator(),
        text(std::to_string(system_stats.get_idle_time())),
        separator(),
        text(std::to_string(system_stats.get_num_entities())),
        separator(),
        text(std::to_string(system_stats.get_num_runnable_entities())),
      }) | border,
      vbox({
        grid->Render() | vscroll_indicator | frame | flex | border,
      }) | flex,
      separator(),
      hbox({text("footer")})
    });
  });
 
  
  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(2.0s);
      screen.Post([&]{
        processes = get_processes();
        system_stats = hightop::SystemStats();
      });
      // After updating the state, request a new frame to be drawn. This is done
      // by simulating a new "custom" event to be handled.
      screen.Post(Event::Custom);
    }
  });

  screen.Loop(renderer);
  refresh_ui_continue = false;
  refresh_ui.join();

  return EXIT_SUCCESS;
}
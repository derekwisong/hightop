#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <ftxui-grid-container/grid-container.hpp>

#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#include "hightop/procs.h"
#include "hightop/SystemStats.h"

#include "libhightop/procs.h"
#include "ui/ftxui-interactive-table/interactive-table.h"

namespace {

class Column {
  public:
  Column(std::string name) : name(std::move(name)) {}
  Column(std::string name, std::function<ftxui::Component(const hightop::Process&)> get_element) : name(std::move(name)), get_element_callback(std::move(get_element)) {}
  ftxui::Component get_element(const hightop::Process& proc) const {
    return get_element_callback(proc);
  }
  std::string get_name() const {
    return name;
  }
  private:
  std::string name;
  std::function<ftxui::Component(const hightop::Process&)> get_element_callback;
};

std::vector<std::vector<std::string>> get_processes() {
  auto procs = hightop::get_running_processes();
  std::vector<std::vector<std::string>> rows;
  //rows.push_back({"PID",  "NICE", "PRIO", "VIRT", "RES", "STATE", "STIME", "COMMAND"});
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
  auto processes = hightop::get_running_processes();
  auto screen = ScreenInteractive::Fullscreen();
  auto selected_row = std::make_shared<int>(0);

  std::vector<Column> columns({
    Column("PID", [](const hightop::Process& proc){return Renderer([&]{return text(std::to_string(proc.get_pid()));});}),
    Column("NICE", [](const hightop::Process& proc){return Renderer([&]{return text(std::to_string(proc.get_stats().nice));});}),
    Column("PRIO", [](const hightop::Process& proc){return Renderer([&]{return text(std::to_string(proc.get_stats().priority));});}),
    Column("VIRT", [](const hightop::Process& proc){return Renderer([&]{return text(std::to_string(proc.get_stats().vsize));});}),
    Column("RES", [](const hightop::Process& proc){return Renderer([&]{return text(std::to_string(proc.get_stats().rss));});}),
    Column("STATE", [](const hightop::Process& proc){return Renderer([&]{return text(std::string(1, proc.get_stats().state));});}),
    Column("STIME", [](const hightop::Process& proc){return Renderer([&]{return text(std::to_string(proc.get_stats().stime));});}),
    Column("COMMAND", [](const hightop::Process& proc){return Renderer([&]{return text(proc.get_stats().comm);});}),
    Column("CMDLINE", [](const hightop::Process& proc){return Renderer([&]{return text(proc.get_command().value_or(""));});}),
  });

  std::vector<ftxui::Components> process_rows;

  for (const auto& process : processes) {
    ftxui::Components row;
    for (const auto& column : columns) {
      row.push_back(column.get_element(process));
    }
    process_rows.emplace_back(std::move(row));
  }

  std::vector<std::string> column_names;
  std::transform(columns.begin(), columns.end(), std::back_inserter(column_names), [](const auto& column){return column.get_name();});

  auto grid = hightop::ui::InteractiveTable(process_rows, column_names, selected_row);

  auto renderer = Renderer([&] {
    return vbox({
      hbox({
        text("TASKS: " + std::to_string(processes.size())),
        filler(),
        text(
          "LOAD: " 
          + std::to_string(system_stats.get_load_1_min())
          + "/"
          + std::to_string(system_stats.get_load_5_min())
          + "/"
          + std::to_string(system_stats.get_load_15_min())
        ),
        filler(),
        text(std::to_string(system_stats.get_uptime())),
        filler(),
        text(std::to_string(system_stats.get_idle_time())),
        filler(),
        text(std::to_string(system_stats.get_num_entities())),
        filler(),
        text(std::to_string(system_stats.get_num_runnable_entities())),
      }) | bold,
      separator(),
      grid->Render() | flex,
      separator(),
      hbox({
        text("Selected row: " + std::to_string(*selected_row)),
      })
    });
  });
 

  renderer = ftxui::CatchEvent(renderer, [&](ftxui::Event event){
    using namespace ftxui;
    if (event == Event::ArrowDown) {
      if (*selected_row < processes.size() - 1) (*selected_row)++;
      return true;
    }
    else if (event == Event::ArrowUp) {
     if (*selected_row > 0) (*selected_row)--;
      return true;
    }

    return false;
  });

  
  // std::atomic<bool> refresh_ui_continue = true;
  // std::thread refresh_ui([&] {
  //   while (refresh_ui_continue) {
  //     using namespace std::chrono_literals;
  //     std::this_thread::sleep_for(2.0s);
  //     screen.Post([&]{
  //       processes = get_processes();
  //       system_stats = hightop::SystemStats();
  //     });
  //     // After updating the state, request a new frame to be drawn. This is done
  //     // by simulating a new "custom" event to be handled.
  //     screen.Post(Event::Custom);
  //   }
  // });

  screen.Loop(renderer);
  // refresh_ui_continue = false;
  // refresh_ui.join();

  return EXIT_SUCCESS;
}
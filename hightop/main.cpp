#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <vector>

#include "hightop/procs.h"
 

std::vector<std::vector<std::string>> get_processes() {
  auto procs = hightop::get_running_processes();
  std::vector<std::vector<std::string>> rows;
  rows.push_back({"PID",  "NICE", "PRIO", "VIRT", "RES", "STATE", "COMMAND"});
  std::transform(
    procs.begin(),
    procs.end(),
    std::back_inserter(rows),
    [](const auto& proc){
      return std::vector<std::string>({
        std::to_string(proc.get_pid()),
        proc.get_niceness().has_value() ? std::to_string(proc.get_niceness().value()) : "",
        proc.get_priority().has_value() ? std::to_string(proc.get_priority().value()) : "",
        proc.get_virtual_memory().has_value() ? std::to_string(proc.get_virtual_memory().value()) : "",
        proc.get_resident_memory().has_value() ? std::to_string(proc.get_resident_memory().value()) : "",
        proc.get_state().has_value() ? proc.get_state().value() : "",
        proc.get_command().value_or(""),
      });
    });
  return rows;
}

int main(void) {
  using namespace ftxui;
  auto processes = get_processes();
  auto status_label = std::string{"Processes: "} + std::to_string(processes.size());

  auto renderer = Renderer([&] {
    Table table(processes);
    table.SelectRow(0).BorderBottom(BorderStyle::DOUBLE);

    return vbox({
      hbox({
        text(status_label),
      }) | border,
      vbox({
        table.Render() | vscroll_indicator | frame | flex
      }) | flex,
      separator(),
      hbox({text("footer")})
    });
  });
 
  auto screen = ScreenInteractive::Fullscreen();
  
  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.2s);
      screen.Post([&]{processes = get_processes(); });
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
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <vector>

#include "hightop/procs.h"
 

std::vector<std::vector<ftxui::Element>> get_processes() {
  auto procs = hightop::get_processes();
  std::vector<std::vector<ftxui::Element>> rows;
  std::transform(
    procs.begin(),
    procs.end(),
    std::back_inserter(rows),
    [](const auto& proc){
      auto name = ftxui::text("PID " + std::to_string(proc.get_pid()));
      return std::vector<ftxui::Element>({name});
    });
  return rows;
}

int main(void) {
  using namespace ftxui;
  auto status_label = std::string{"Processes: "} + std::to_string(hightop::count_processes());
  auto processes = get_processes();

  auto renderer = Renderer([&] {
    Table table(processes);

    return vbox({
      hbox({
        text(status_label),
      }),
      separator(),
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
      std::this_thread::sleep_for(0.05s);
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
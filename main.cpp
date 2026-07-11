#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN


#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <raylib.h>
#include <thread>

#include "process/process.h"

using std::cout;
using std::endl;

namespace ui {
  int window_width = 1920;
  int window_height = 1080;
  Font font;
  int scroll;
}

void update_input();

//  cout << std::chrono::high_resolution_clock::now().time_since_epoch().count() << endl;

int main() {
  // std::jthread thread_test = std::thread(&function_test);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(ui::window_width, ui::window_height, "test");

  ui::font = LoadFont("Archivo-SemiBold.ttf");

  process::update_list();

  auto last_clock_processes_cpu = std::chrono::steady_clock::now();
  auto last_clock_processes_update = std::chrono::steady_clock::now();

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    auto now = std::chrono::steady_clock::now();

    if (now - last_clock_processes_update >= std::chrono::milliseconds(2000)) {
      process::update_list();
      // process::group_children_processes();
      last_clock_processes_update = now;
    }
    if (now - last_clock_processes_cpu >= std::chrono::milliseconds(500)) {
      std::jthread t(process::update_processes_cpu_usage);
      t.detach();
      last_clock_processes_cpu = now;
    }

    update_input();

    ClearBackground({20, 20, 20, 255});
    SetWindowOpacity(1);
    BeginDrawing();
    DrawFPS(1700, 10);

    float i = 50;
    float n = 0;

    DrawLine(0, i - 6, ui::window_width, i - 6, GRAY);
    DrawLine(0, i - 7, ui::window_width, i - 7, GRAY);

    for (const auto& proc: process::processes) {
      if (n < ui::scroll) {
        n++;
        continue;
      }
      if (i > 1000) {break;}
      DrawTextEx(ui::font, proc.second.name.c_str(), {50, i}, 18, 0.5, WHITE);
      DrawTextEx(ui::font, std::to_string(proc.second.cpu_usage.load()).c_str(), {450, i}, 18, 0.5, WHITE);
      DrawTextEx(ui::font, std::to_string(proc.first).c_str(), {600, i}, 18, 0.5, WHITE);
      DrawTextEx(ui::font, std::to_string(proc.second.parent_id).c_str(), {750, i}, 18, 0.5, WHITE);
      if (proc.second.sub_proc_number != 0) {
        DrawTextEx(ui::font, std::to_string(proc.second.sub_proc_number).c_str(), {900, i}, 18, 0.5, WHITE);
      }

      DrawLine(0, i + 22, ui::window_width, i + 22, GRAY);
      DrawLine(0, i + 21, ui::window_width, i + 21, GRAY);
      i += 28;

    }
    EndDrawing();
  }

}


void update_input() {

  ui::scroll -= GetMouseWheelMove();
  if (ui::scroll < 0) {ui::scroll = 0;}

  if (IsWindowResized()) {
    ui::window_width = GetScreenWidth();
    ui::window_height = GetScreenHeight();
  }


}
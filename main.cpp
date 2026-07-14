#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <raylib.h>
#include <thread>

#include "processes/processes_windows.h"
#include "ui/ui.h"
#include "assets/font.h"

using std::cout;
using std::endl;

void update_input();

//  cout << std::chrono::high_resolution_clock::now().time_since_epoch().count() << endl;

int main() {
  // std::jthread thread_test = std::thread(&function_test);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(ui::window_width, ui::window_height, "SystemMonitor");

  ui::font = LoadFontFromMemory(".ttf", assets_font_ttf, assets_font_ttf_len, 48, nullptr, 250);


  process::update_list();

  auto last_clock_processes_cpu = std::chrono::steady_clock::now();
  auto last_clock_processes_update = std::chrono::steady_clock::now();

  //SetTargetFPS(60);

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

    ClearBackground({30, 30, 30, 255});
    SetWindowOpacity(1);
    BeginDrawing();

    ui::render_background();
    ui::render_processes();

    DrawFPS(5, 5);
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


  //pulsanti

  if (CheckCollisionPointRec(GetMousePosition(), ui::header_name_rect)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      ui::processes_sort = SORT_ALPHABETICAL;
    }
  }

  if (CheckCollisionPointRec(GetMousePosition(), ui::header_pid_rect)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      ui::processes_sort = SORT_PID;
    }
  }

  if (CheckCollisionPointRec(GetMousePosition(), ui::header_parent_pid_rect)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      ui::processes_sort = SORT_PARENT_PID;
    }
  }

  if (CheckCollisionPointRec(GetMousePosition(), ui::header_cpu_usage_rect)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      ui::processes_sort = SORT_CPU_USAGE;
    }
  }

}
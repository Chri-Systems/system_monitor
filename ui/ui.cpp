#include "ui.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


#include "../processes/processes.h"


namespace ui {
  void render_menu() {
    int n = 40;
    const char* text[3] = {"usage", "processes", "settings"};
    for (auto & loop_text : text) {
      Vector2 text_size = MeasureTextEx(font, loop_text, 24, 0);
      Rectangle rect(40, n, 150, 60);
      
      DrawRectangleRounded(rect, 0.4, 2, lines_color_2);
      DrawTextEx(font, loop_text, {((150 - text_size.x) / 2) + 40, ((60 - text_size.y) / 2) + n}, 24, 0, {GRAY});
      n += 80;
    }
  }

  void render_processes() {
    float i = 100;
    float n = 0;

    DrawTextEx(font, "Executable", {column_name + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Pid", {column_proc_id + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Parent pid", {column_parent_id + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Cpu usage", {column_cpu + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Path", {column_path + column_offset, header_text_height}, header_text_size, 0, header_text_color);

    //vettore puntatori map
    std::vector<const std::pair<const long unsigned int, processes::Process>*> sorted_procs;
    for (const auto& proc : processes::processes) {
      sorted_procs.push_back(&proc);
    }

    switch (processes_sort) {

      case SORT_ALPHABETICAL: {
        std::sort(sorted_procs.begin(), sorted_procs.end(), [](const auto* a, const auto* b) {
          return a->second.name < b->second.name;
        });

        // DrawTextEx(font, "°", {column_proc_id + column_offset - 30 ,header_text_height}, 32, 0, WHITE);

        break;
      }

      case SORT_PID: {
        std::sort(sorted_procs.begin(), sorted_procs.end(), [](const auto* a, const auto* b) {
          return a->first < b->first;
        });
        break;
      }

      case SORT_PARENT_PID: {
        std::sort(sorted_procs.begin(), sorted_procs.end(), [](const auto* a, const auto* b) {
          return a->second.parent_pid < b->first;
        });
        break;
      }

      case SORT_CPU_USAGE: {
        std::sort(sorted_procs.begin(), sorted_procs.end(), [](const auto* a, const auto* b) {
          return a->second.cpu_usage.load() > b->second.cpu_usage.load();
        });
        break;
      }
      default: break;
    }

    for (const auto& proc: sorted_procs) {
      if (n < float(scroll)) {
        n++;
        continue;
      }
      if (i > window_height - 70) {break;}

      DrawTexture(proc->second.icon, column_ico + column_offset, i + 4, WHITE);
      DrawTextEx(font, proc->second.name.c_str(), {column_name + column_offset, i}, data_text_size, 0, data_text_color);
      DrawTextEx(font, std::to_string(proc->first).c_str(), {column_proc_id + column_offset, i}, data_text_size, 0, data_text_color);
      DrawTextEx(font, std::to_string(proc->second.parent_pid).c_str(), {column_parent_id + column_offset, i}, data_text_size, 0, data_text_color);
      DrawTextEx(font, std::to_string(proc->second.cpu_usage.load()).substr(0, std::to_string(proc->second.cpu_usage.load()).find('.') + 3).c_str(), {column_cpu + column_offset, i}, data_text_size, 0, data_text_color);


      DrawTextEx(font, proc->second.path_to_show.c_str(), {column_path + column_offset, i}, data_text_size, 0, data_text_color);


      i += line_height;
      n++;
    }
  }



  void render_background() {
    Rectangle rect(220, 20, window_width - 260, window_height - 40);
    DrawRectangleRounded(rect, 0.04, 2, lines_color_1);

    Rectangle rect_menu(20, 20, 190, window_height - 40);
    DrawRectangleRounded(rect_menu, 0.2, 2, lines_color_1);

    for (int i = 98; i < window_height - 70; i += line_height * 2) {
      DrawRectangle(220, i - 2, window_width - 260, line_height, lines_color_2);
    }

  }

  void draw_path_string_fix() {
    float max_width = window_width - 1094.0f;
    if (max_width < 50.0f) max_width = 50.0f;

    for (auto& proc: processes::processes) {
      proc.second.path_to_show = proc.second.path;
      float x_size = MeasureTextEx(font, proc.second.path_to_show.c_str(), data_text_size, 0).x;
      if (x_size > max_width) {
        while (x_size + 14 > max_width) {
          proc.second.path_to_show.pop_back();
          x_size = MeasureTextEx(font, proc.second.path_to_show.c_str(), data_text_size, 0).x;
        }
        proc.second.path_to_show += "...";
      }
    }
  }

}
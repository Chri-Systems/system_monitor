#include "ui.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


#include "../processes/processes.h"


namespace ui {
  float column_ico = 190;
  float column_name = 220;
  float column_proc_id = 570;
  float column_parent_id = 720;
  float column_cpu = 870;
  float column_path = 1000;
  float column_offset = 45;

  Color header_text_color = {200, 200, 200, 255};
  float header_text_size = 24;
  float header_text_height = 52;

  Color data_text_color = {200, 200, 200, 255};
  float data_text_size = 20;

  float line_height = 30;
  Color lines_color_1 = {40, 40, 40, 255};
  Color lines_color_2 = {60, 60, 60, 255};

  void render_processes() {
    float i = 100;
    float n = 0;

    DrawTextEx(font, "Executable", {column_name + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Pid", {column_proc_id + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Parent pid", {column_parent_id + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Cpu usage", {column_cpu + column_offset, header_text_height}, header_text_size, 0, header_text_color);
    DrawTextEx(font, "Path", {column_path + column_offset, header_text_height}, header_text_size, 0, header_text_color);

    //vettore puntatori map
    std::vector<const std::pair<const long unsigned int, process::Process>*> sorted_procs;
    for (const auto& proc : process::processes) {
      sorted_procs.push_back(&proc);
    }

    switch (processes_sort) {

      case 0: {
        //sort alfabetico
        std::sort(sorted_procs.begin(), sorted_procs.end(), [](const auto* a, const auto* b) {
          return a->second.name < b->second.name;
        });
        break;
      }

      case 1: {
        //sort cpu
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
      DrawTextEx(font, std::to_string(proc->second.parent_id).c_str(), {column_parent_id + column_offset, i}, data_text_size, 0, data_text_color);
      DrawTextEx(font, std::to_string(proc->second.cpu_usage.load()).substr(0, std::to_string(proc->second.cpu_usage.load()).find('.') + 3).c_str(), {column_cpu + column_offset, i}, data_text_size, 0, data_text_color);

      float max_width = window_width - 1094.0f;
      if (max_width < 50.0f) max_width = 50.0f;

      std::string path = proc->second.path;
      float x_size = MeasureTextEx(font, path.c_str(), data_text_size, 0).x;
      if (x_size > max_width) {
        while (x_size + 14 > max_width) {
          path.pop_back();
          x_size = MeasureTextEx(font, path.c_str(), data_text_size, 0).x;
        }
        path += "...";
      }
      DrawTextEx(font, path.c_str(), {column_path + column_offset, i}, data_text_size, 0, data_text_color);

      /*
      if (proc.second.sub_proc_number != 0) {
        DrawTextEx(font, std::to_string(proc.second.sub_proc_number).c_str(), {column_sub_proc_number, i}, 18, 0, WHITE);
      }
      */

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
}
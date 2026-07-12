#pragma once
#include <raylib.h>

enum sort_types {
  SORT_ALPHABETICAL,
  SORT_PID,
  SORT_PARENT_PID,
  SORT_CPU_USAGE,
};

namespace ui {
  inline int window_width = 1920;
  inline int window_height = 1080;
  inline Font font;
  inline int scroll;
  inline int processes_sort = 0;


  inline float column_ico = 190;
  inline float column_name = 220;
  inline float column_proc_id = 570;
  inline float column_parent_id = 720;
  inline float column_cpu = 870;
  inline float column_path = 1000;
  inline float column_offset = 45;

  inline Color header_text_color = {200, 200, 200, 255};
  inline float header_text_size = 24;
  inline float header_text_height = 52;

  inline Color data_text_color = {200, 200, 200, 255};
  inline float data_text_size = 20;

  inline float line_height = 30;
  inline Color lines_color_1 = {40, 40, 40, 255};
  inline Color lines_color_2 = {60, 60, 60, 255};


  inline Rectangle header_name_rect(column_name + column_offset, 30, column_proc_id - column_name - 20, 60);
  inline Rectangle header_pid_rect(column_proc_id + column_offset, 30, column_parent_id - column_proc_id - 20, 60);
  inline Rectangle header_parent_pid_rect(column_parent_id + column_offset, 30, column_cpu - column_parent_id - 20, 60);
  inline Rectangle header_cpu_usage_rect(column_cpu + column_offset, 30, column_path - column_cpu - 20, 60);

  void render_processes();
  void render_background();
}

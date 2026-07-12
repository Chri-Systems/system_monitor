#pragma once
#include <raylib.h>


namespace ui {
  inline int window_width = 1920;
  inline int window_height = 1080;
  inline Font font;
  inline int scroll;
  inline int processes_sort = 0;

  inline Rectangle header_cpu_usage_rect(220, 30, 330, 60);
  inline Rectangle header_name_rect(870, 30, 120, 60);

  void render_processes();
  void render_background();
}

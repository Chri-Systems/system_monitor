#pragma once
#define WIN32_LEAN_AND_MEAN

#include <atomic>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <tchar.h>

#define Rectangle RectangleWindows
#define CloseWindow CloseWindowWindows
#define ShowCursor ShowCursorWindows
#define DrawTextEx DrawTextExWindows
#include <windows.h>
#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef DrawTextEx


namespace process {
  struct proc_times {
    unsigned long long kernel;
    unsigned long long user;
  };
  struct Process {
    DWORD parent_pid;
    std::string name;
    std::string path;
    std::atomic<double> cpu_usage;

    HANDLE handle;
    int sub_proc_number;
    proc_times last_time;
    Texture icon;

    bool is_system = false;
  };

  inline std::unordered_map<DWORD, Process> processes;


  proc_times get_process_time(HANDLE process);
  double get_process_percentage(HANDLE process, int sleep);
  void update_list();
  void update_processes_cpu_usage();
  std::string get_process_path(HANDLE handle);
  // void group_children_processes();
  Texture2D get_ico_texture(const std::string& path);
}

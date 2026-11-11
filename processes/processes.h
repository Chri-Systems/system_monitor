#pragma once
#define WIN32_LEAN_AND_MEAN

#include <atomic>
#include <string>
#include <unordered_map>

#ifdef _WIN32
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
#endif


namespace processes {
  struct proc_times {
    unsigned long long kernel;
    unsigned long long user;
  };
  struct Process {
    unsigned long parent_pid;
    std::string name;
    std::string path;
    std::string path_to_show;
    std::atomic<double> cpu_usage;

    void *handle;
    int sub_proc_number;
    proc_times last_time;

    bool is_system = false;
  };

  inline std::unordered_map<unsigned long, Process> processes;


  proc_times get_process_time(const std::string &pid);

  double get_process_percentage(void *process, int sleep);
  void update_process_list();
  void update_process_ui();
  void update_processes_cpu_usage();
  std::string get_process_path(void *handle);
  // void group_children_processes();
} // namespace processes

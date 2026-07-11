#pragma once
#include <atomic>
#include <unordered_map>


namespace process {
  struct proc_times {
    unsigned long long kernel;
    unsigned long long user;
  };
  struct Process {
    DWORD parent_id;
    std::string name;
    std::string path;

    std::atomic<double> cpu_usage;

    HANDLE handle;

    int sub_proc_number;

    proc_times last_time;
  };

  inline std::unordered_map<DWORD, Process> processes;


  proc_times get_process_time(HANDLE process);
  double get_process_percentage(HANDLE process, int sleep);
  void update_list();
  void update_processes_cpu_usage();
  std::string get_process_path(HANDLE handle);
  // void group_children_processes();
}

#pragma once


namespace process {
  struct proc_times {
    unsigned long long kernel;
    unsigned long long user;
  };


  proc_times get_process_time(HANDLE process);
  double get_process_percentage(HANDLE process, int sleep);
}

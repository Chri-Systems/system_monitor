#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#include "process.h"

namespace process {
  proc_times get_process_time(HANDLE process) {
    FILETIME c_time, e_time, k_time, u_time;
    if (!GetProcessTimes(process, &c_time, &e_time, &k_time, &u_time)) {
      return {0, 0};
    }

    ULARGE_INTEGER kernel_time;
    kernel_time.LowPart = k_time.dwLowDateTime;
    kernel_time.HighPart = k_time.dwHighDateTime;

    ULARGE_INTEGER user_time;
    user_time.LowPart = u_time.dwLowDateTime;
    user_time.HighPart = u_time.dwHighDateTime;

    return {kernel_time.QuadPart, user_time.QuadPart};

  }

  double get_process_percentage(HANDLE process, int sleep) {
    proc_times proc_times1 = get_process_time(process);
    Sleep(sleep);
    proc_times proc_times2 = get_process_time(process);

    unsigned long long used = (proc_times2.user - proc_times1.user) + (proc_times2.kernel - proc_times1.kernel);
    unsigned long long time_passed = sleep * 10000;

    return ((static_cast<double>(used) * 100) / static_cast<double>(time_passed));
  }
}
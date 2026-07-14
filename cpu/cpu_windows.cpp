#ifdef _WIN32

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#include "cpu_windows.h"


namespace cpu {
  cpu_times get_cpu_time() {
    FILETIME i_time, k_time, u_time;
    GetSystemTimes(&i_time, &k_time, &u_time);

    ULARGE_INTEGER idle_time;
    idle_time.LowPart = i_time.dwLowDateTime;
    idle_time.HighPart = i_time.dwHighDateTime;

    ULARGE_INTEGER kernel_time;
    kernel_time.LowPart = k_time.dwLowDateTime;
    kernel_time.HighPart = k_time.dwHighDateTime;

    ULARGE_INTEGER user_time;
    user_time.LowPart = u_time.dwLowDateTime;
    user_time.HighPart = u_time.dwHighDateTime;

    return {idle_time.QuadPart, kernel_time.QuadPart, user_time.QuadPart};

  }


  double get_cpu_percentage(int sleep) {
    cpu_times cpu_times1 = get_cpu_time();
    Sleep(sleep);
    cpu_times cpu_times2 = get_cpu_time();

    unsigned long long total = (cpu_times2.user - cpu_times1.user) + (cpu_times2.kernel - cpu_times1.kernel);
    unsigned long long idle = (cpu_times2.idle - cpu_times1.idle);
    unsigned long long used = total - idle;

    return ((static_cast<double>(used) * 100) / static_cast<double>(total));
  }
}

#endif
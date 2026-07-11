#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#include "process.h"

#include <thread>

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

  double get_process_percentage(proc_times proc_times1, proc_times proc_times2, int ms_passed) {

    unsigned long long used = (proc_times2.user - proc_times1.user) + (proc_times2.kernel - proc_times1.kernel);
    unsigned long long time_passed = ms_passed * 10000;

    return ((static_cast<double>(used) * 100) / static_cast<double>(time_passed));
  }

  void update_list() {
      
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 proc_entry;
  
    if (snapshot == INVALID_HANDLE_VALUE) {
      std::cout << "error snapshot: INVALID_HANDLE_VALUE" << std::endl;
    }

    proc_entry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &proc_entry)) {
      CloseHandle(snapshot);
      std::cout << "error proc" << std::endl;

    }


    while (Process32Next(snapshot, &proc_entry)) {
      if (!processes.contains(proc_entry.th32ProcessID)) {
        if (static_cast<std::string>(proc_entry.szExeFile).empty()) {continue;}
        // if (static_cast<std::string>(proc_entry.szExeFile)[0] != 'o') {continue;}
        Process& proc = processes[proc_entry.th32ProcessID];
        proc.name = std::string(proc_entry.szExeFile);
        proc.handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, proc_entry.th32ProcessID);
        proc.parent_id = proc_entry.th32ParentProcessID;
        proc.path = get_process_path(proc.handle);
        if (proc.path.starts_with(R"(C:\Windows\System32\)")) {
          CloseHandle(proc.handle);
          processes.erase(proc_entry.th32ProcessID);
        }
      }
    }
    CloseHandle(snapshot);



    /*
    for (const auto& proc: processes) {
      if (proc.second.handle == nullptr) {
        CloseHandle(proc.second.handle);
        processes.erase(proc.first);
      }
    }
    */


  }

  void update_processes_cpu_usage() {


    for (auto& proc: processes) {
      proc.second.last_time = get_process_time(proc.second.handle);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    for (auto& proc: processes) {
      proc_times new_time = get_process_time(proc.second.handle);
      proc.second.cpu_usage.store(get_process_percentage(proc.second.last_time, new_time, 300));
    }
  }


  std::string get_process_path(HANDLE handle) {
    char buffer[MAX_PATH];
    DWORD size = MAX_PATH;
    QueryFullProcessImageNameA(handle, 0, buffer, &size);

    return {buffer, size};
  }

  /*
  void group_children_processes() {
    for (auto& proc1: processes) {
      for (auto& proc2: processes) {
        if (proc1.first == proc2.first) {continue;}
        if (proc1.second.parent_id != proc2.second.parent_id) {continue;}
        if (!processes.contains(proc1.second.parent_id)) {continue;}

        processes.find(proc1.second.parent_id)->second.sub_proc_number += 2;

        processes.erase(proc1.first);
        processes.erase(proc2.first);
      }
    }
  }
  */

}

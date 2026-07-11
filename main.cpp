#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

using std::cout;
using std::endl;

struct cpu_times {
  unsigned long long idle;
  unsigned long long kernel;
  unsigned long long user;
};

struct proc_times {
  unsigned long long kernel;
  unsigned long long user;
};

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

proc_times get_process_time(HANDLE process) {
  FILETIME c_time, e_time, k_time, u_time;
  if (!GetProcessTimes(process, &c_time, &e_time, &k_time, &u_time)) {
    cout << "error" << endl;
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

  cout << proc_times1.kernel + proc_times1.user << endl;
  cout << proc_times2.kernel + proc_times2.user << endl;


  unsigned long long used = (proc_times2.user - proc_times1.user) + (proc_times2.kernel - proc_times1.kernel);
  unsigned long long time_passed = sleep * 10000;

  return ((static_cast<double>(used) * 100) / static_cast<double>(time_passed));
}

int main() {
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 proc_entry;


  if (snapshot == INVALID_HANDLE_VALUE) {
    return 2;
  }

  proc_entry.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(snapshot, &proc_entry)) {
    CloseHandle(snapshot);
    return 3;
  }


  cout << proc_entry.th32ProcessID << endl;
  _tprintf(proc_entry.szExeFile);
  cout << "\n\n";

  while (Process32Next(snapshot, &proc_entry)) {
    
    if (static_cast<std::string>(proc_entry.szExeFile) != "opera.exe") {
      continue;
    }
    cout << "proc id: " << proc_entry.th32ProcessID << endl;
    cout << "proc name: " << proc_entry.szExeFile << endl;
    cout << "parent id: " << proc_entry.th32ParentProcessID  << endl;

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, proc_entry.th32ProcessID);
    cout << get_process_percentage(process, 2000) << endl;


    cout << "\n\n";


  }
  CloseHandle(snapshot);


}

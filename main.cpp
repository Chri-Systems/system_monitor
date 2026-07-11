#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN


#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <raylib.h>
#include <thread>

#include "process/process.h"

using std::cout;
using std::endl;


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


  cout << "proc id: " << proc_entry.th32ProcessID << endl;
  cout << "proc name: " << proc_entry.szExeFile << endl;
  cout << "parent id: " << proc_entry.th32ParentProcessID  << endl;

  while (Process32Next(snapshot, &proc_entry)) {
    
    if (static_cast<std::string>(proc_entry.szExeFile) != "opera.exe") {
      continue;
    }
    cout << "proc id: " << proc_entry.th32ProcessID << endl;
    cout << "proc name: " << proc_entry.szExeFile << endl;
    cout << "parent id: " << proc_entry.th32ParentProcessID  << endl;

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, proc_entry.th32ProcessID);
    cout << process::get_process_percentage(process, 2000) << endl;


    cout << "\n\n";


  }
  CloseHandle(snapshot);


}

#ifdef __linux__

#include "processes.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h>

namespace processes {

  proc_times get_process_time(const std::string& pid) {
    std::filesystem::path pid_path(pid);

    std::ifstream stat_path("/proc" / pid_path / "stat");
    if (!stat_path.is_open()) return {0, 0};

    std::string stat;
    std::getline(stat_path, stat);

    size_t last_paren = stat.find_last_of(')');
    if (last_paren == std::string::npos || last_paren + 2 >= stat.size()) return {0, 0};

    std::string cpu = stat.substr(last_paren + 2);
    std::istringstream cpu_stream(cpu);

    std::string void_string;
    for (int i = 0; i < 11; i++) {
      cpu_stream >> void_string;
    }

    ulong u_time;
    ulong k_time;

    cpu_stream >> u_time >> k_time;

    return {k_time, u_time};

  }

  double get_process_percentage(proc_times proc_times1, proc_times proc_times2, int ms_passed) {
    unsigned long long used_ticks = (proc_times2.user - proc_times1.user) + (proc_times2.kernel - proc_times1.kernel);

    long tps = sysconf(_SC_CLK_TCK);

    double used_ms = (static_cast<double>(used_ticks) * 1000.0) / tps;

    return (used_ms * 100.0) / static_cast<double>(ms_passed);
  }

  void update_list() {
    for (const auto& proc_path : std::filesystem::directory_iterator("/proc")) {
      std::string pid = proc_path.path().filename().string();

      if (proc_path.is_directory()) {
        bool is_pid = true;
        for (const char c : pid) {
          if (!std::isdigit(c)) {
            is_pid = false;
            break;
          }
        }
        if (is_pid) {
          Process& proc = processes[stoul(pid)];

          std::ifstream name_path(proc_path.path() / "comm");
          std::string name;
          std::getline(name_path, name);
          proc.name = name;

        }
      }
    }
  }

  void update_processes_cpu_usage() {
    for (auto& proc: processes) {
      proc.second.last_time = get_process_time(std::to_string(proc.first));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    for (auto& proc: processes) {
      proc_times new_time = get_process_time(std::to_string(proc.first));
      proc.second.cpu_usage.store(get_process_percentage(proc.second.last_time, new_time, 300));
    }
  }

}

#endif

#pragma once

namespace cpu {
  struct cpu_times {
    unsigned long long idle;
    unsigned long long kernel;
    unsigned long long user;
  };


  cpu_times get_cpu_time();
  double get_cpu_percentage(int sleep);
}


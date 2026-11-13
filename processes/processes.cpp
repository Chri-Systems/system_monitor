#include "processes.h"

#include <filesystem>
#include <fstream>

#include "ui_main_window.h"
#include "ui_process_list_item.h"
#include "../global.h"

namespace processes {

  void update_process_list() {
    using global::ui;

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
          std::ifstream name_path(proc_path.path() / "comm");
          std::string name;
          std::getline(name_path, name);

          Process& proc = processes[stoi(pid)];
          proc.name = name;

        }
      }
    }
  }
  void update_process_ui() {
    using global::ui;
    for (auto &proc: processes) {

      auto *process_item = new QWidget();
      Ui::process_list_item process_list_item;
      process_list_item.setupUi(process_item);

      process_list_item.label_name->setText(proc.second.name.data());

      auto *item = new QListWidgetItem(ui.process_list);
      ui.process_list->addItem(item);
      ui.process_list->setItemWidget(item, process_item);

    }
  }
}
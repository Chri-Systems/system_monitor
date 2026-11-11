#include <algorithm>
#include <iostream>
#include <thread>
#include <qscreen.h>

#include "global.h"
#include "ui_main_window.h"
#include "ui_process_list_item.h"
#include "processes/processes.h"

using std::cout;
using std::endl;

//  cout << std::chrono::high_resolution_clock::now().time_since_epoch().count() << endl;

int main(int argc, char *argv[]) {
  using global::ui;
  QApplication app(argc, argv);
  QMainWindow window;
  ui.setupUi(&window);
  window.resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.6);

  processes::update_process_list();
  processes::update_process_ui();

  window.show();
  return QApplication::exec();
}

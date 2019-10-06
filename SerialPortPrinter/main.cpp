#include <windows.h>

#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  HWND hWnd = FindWindowA(nullptr, "SerialPortPrinter");
  if (hWnd != nullptr) {
    BringWindowToTop(hWnd);
    if (QApplication::arguments().size() > 1) {
      std::string s = QApplication::arguments().last().toStdString();
      COPYDATASTRUCT cds;
      cds.dwData = 1234;  // can be anything
      cds.cbData = sizeof(char) * static_cast<unsigned long>(s.size() + 1);
      cds.lpData = s.data();
      SendMessage(hWnd, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&cds));
    }
  } else {
    MainWindow w;
    w.show();
    return a.exec();
  }
  return 0;
}

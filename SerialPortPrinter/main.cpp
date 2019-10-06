#include <windows.h>

#include <QApplication>

#include "MainWindow.h"

void sendCopydataMessage(HWND hWnd) {
  std::string s = QApplication::arguments().last().toStdString();
  COPYDATASTRUCT cds;
  cds.dwData = MainWindow::copydataIdentifier;
  cds.cbData = sizeof(char) * static_cast<unsigned long>(s.size() + 1);
  cds.lpData = s.data();
  SendMessage(hWnd, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&cds));
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  HWND hWnd = FindWindowA(nullptr, "SerialPortPrinter");
  if (hWnd != nullptr) {
    if (QApplication::arguments().size() > 1) {
      sendCopydataMessage(hWnd);
    }
    return 0;
  } else {
    MainWindow w;
    w.show();
    return a.exec();
  }
}

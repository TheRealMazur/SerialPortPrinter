#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include <string>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SettingsDialog;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:

 private slots:
  void openSerialPort();
  void closeSerialPort();
  void about();
  void writeData(const QByteArray &data);
  void readData();

  void handleError(QSerialPort::SerialPortError error);
  void on_portOpenButton_released();

  void on_fileOpenButton_released();

  void on_sendButton_released();

 private:
  void initActionsConnections();
  void showStatusMessage(const QString &message);
  void logPortInfo(const QSerialPortInfo &info);
  QMessageLogger mLogger;
  QString mFileContent;
  SettingsDialog *mSettings = nullptr;
  QSerialPort *mSerial = nullptr;
  Ui::MainWindow *ui;
  void getPortsInfo();
  void openFileAndReadContent(const QString &fileName);
};
#endif  // MAINWINDOW_H

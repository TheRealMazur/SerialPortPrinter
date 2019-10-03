#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include "settingsdialog.h"
#include "SerialPortManager.h"

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
  void showAboutDialog();
  void showAboutQtDialog();

  void handleError(QString error);
  void on_portOpenButton_released();

  void on_fileOpenButton_released();

  void on_sendButton_released();

 private:
  void initActionsConnections();
  void showStatusMessage(const QString &message);
  void openFileAndReadContent(const QString &fileName);
  char getCheckSum(QByteArray data);
  QMessageLogger* mLogger = nullptr;
  QByteArray mFileContent = {};
  QList<QByteArray> mCommandList;
  SettingsDialog *mSettings = nullptr;
  SerialPortManager mSerialPortManager;
  Ui::MainWindow *ui = nullptr;
};
#endif  // MAINWINDOW_H

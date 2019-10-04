#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTextStream>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "SerialPortManager.h"
#include "settingsdialog.h"

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
  bool parseFileContent(QString &fileContent);
  void getCommandsFromJson(const QJsonObject &jsonObject);
  void fillTableWidgetFromJson(const QJsonObject &jsonObject);
  void handleOpenedFile(QString &fileContent);
  void fillTableRow(const int &currentRow, const QJsonObject &previewObject);
  QMessageLogger *mLogger = nullptr;
  QJsonDocument mFileContent = {};
  QList<QByteArray> mCommandList;
  SettingsDialog *mSettings = nullptr;
  SerialPortManager mSerialPortManager;
  Ui::MainWindow *ui = nullptr;
};
#endif  // MAINWINDOW_H

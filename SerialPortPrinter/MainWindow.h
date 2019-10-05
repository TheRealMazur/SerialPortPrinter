#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDragEnterEvent>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
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
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  void openSerialPort();
  void closeSerialPort();
  void handleError(const QString &error);
  void showAboutDialog();
  void showAboutQtDialog();

  void on_portOpenButton_released();
  void on_fileOpenButton_released();
  void on_sendButton_released();
  void on_cancelButton_released();

 private:
  char getCheckSum(const QByteArray &data);
  void makeConnections();
  void showStatusMessage(const QString &message);
  void openFileAndReadContent(const QString &fileName);
  void handleOpenedFile(QString &fileContent);
  bool parseFileContent(QString &fileContent);
  void getCommandsFromJson(const QJsonObject &jsonObject);
  void fillTableWidgetFromJson(const QJsonObject &jsonObject);
  void fillTableRow(const int &currentRow, const QJsonObject &previewObject);
  void clearTableWidget();
  void handleDataFromPort(const QByteArray &data);

  QJsonDocument mFileContent = {};
  QList<QByteArray> mCommandList;
  SettingsDialog *mSettings = nullptr;
  SerialPortManager mSerialPortManager;
  Ui::MainWindow *ui = nullptr;
};
#endif  // MAINWINDOW_H

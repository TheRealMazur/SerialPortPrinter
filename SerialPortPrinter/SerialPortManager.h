#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QMessageBox>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "settingsdialog.h"

class SerialPortManager : public QObject {
  Q_OBJECT
 public:
  SerialPortManager();
  bool openSerialPort(const SettingsDialog::Settings& settings);
  void closeSerialPort();
  void writeData(const QByteArray& data);
  bool isPortOpen();
 public slots:
  void readData();
  void handleError(QSerialPort::SerialPortError error);
 signals:
  void serialPortError(QString error);

 private:
  void getPortsInfo();
  void logPortInfo(const QSerialPortInfo& info);
  QSerialPort mSerial;
};

#endif  // SERIALPORTMANAGER_H

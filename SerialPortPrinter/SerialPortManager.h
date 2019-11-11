#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QMessageBox>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


class SerialPortManager : public QObject {
  Q_OBJECT
 public:
  SerialPortManager();
  bool openSerialPort(const QString& portName);
  void closeSerialPort();
  bool isPortOpen();
  void writeCommands(const QList<QByteArray>& commandsList);
 public slots:
  void readData();
  void handleError(QSerialPort::SerialPortError error);
 signals:
  void serialPortError(const QStringList& error);

 private:
  char getCheckSum(const QByteArray &data);
  void writeData(const QByteArray& data);
  QSerialPort mSerial;
  QStringList mErrorBuffer;
};

#endif  // SERIALPORTMANAGER_H

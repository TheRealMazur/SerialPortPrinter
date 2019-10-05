#include "SerialPortManager.h"

SerialPortManager::SerialPortManager() {
  connect(&mSerial, &QSerialPort::errorOccurred, this,
          &SerialPortManager::handleError);
  connect(&mSerial, &QSerialPort::readyRead, this,
          &SerialPortManager::readData);
}

bool SerialPortManager::openSerialPort(
    const SettingsDialog::Settings& settings) {
  mSerial.setPortName(settings.name);
  mSerial.setBaudRate(settings.baudRate);
  mSerial.setDataBits(settings.dataBits);
  mSerial.setParity(settings.parity);
  mSerial.setStopBits(settings.stopBits);
  mSerial.setFlowControl(settings.flowControl);
  if (mSerial.open(QIODevice::ReadWrite)) {
    return true;
  } else {
    emit serialPortError(mSerial.errorString());
  }
  return false;
}

void SerialPortManager::closeSerialPort() {
  if (mSerial.isOpen()) mSerial.close();
}

void SerialPortManager::writeData(const QByteArray& data) {
  qint64 numOfBytesWritten = mSerial.write(data);
  if (numOfBytesWritten != data.size()) {
    emit serialPortError(
        QString("Did not write all bytes. Wrote %1 byte(s) from %2.")
            .arg(numOfBytesWritten)
            .arg(data.size()));
  }
}

bool SerialPortManager::isPortOpen() { return mSerial.isOpen(); }

void SerialPortManager::readData() {
  const QByteArray data = mSerial.readAll();
  emit readDataFromPort(data);
}

void SerialPortManager::handleError(QSerialPort::SerialPortError error) {
  if (error != QSerialPort::NoError) {
    if (error == QSerialPort::ResourceError) {
      closeSerialPort();
    }
    emit serialPortError(mSerial.errorString());
  }
}

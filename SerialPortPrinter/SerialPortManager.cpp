#include "SerialPortManager.h"

SerialPortManager::SerialPortManager() {
  connect(&mSerial, &QSerialPort::errorOccurred, this,
          &SerialPortManager::handleError);
  connect(&mSerial, &QSerialPort::readyRead, this,
          &SerialPortManager::readData);
  getPortsInfo();
}
void SerialPortManager::getPortsInfo() {
  for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts()) {
    logPortInfo(info);
  }
}

void SerialPortManager::logPortInfo(const QSerialPortInfo& info) {
  QString s =
      QObject::tr("Port: ") + info.portName() + "\n" +
      QObject::tr("Location: ") + info.systemLocation() + "\n" +
      QObject::tr("Description: ") + info.description() + "\n" +
      QObject::tr("Manufacturer: ") + info.manufacturer() + "\n" +
      QObject::tr("Serial number: ") + info.serialNumber() + "\n" +
      QObject::tr("Vendor Identifier: ") +
      (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16)
                                  : QString()) +
      "\n" + QObject::tr("Product Identifier: ") +
      (info.hasProductIdentifier()
           ? QString::number(info.productIdentifier(), 16)
           : QString()) +
      "\n" + QObject::tr("Busy: ") +
      (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
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

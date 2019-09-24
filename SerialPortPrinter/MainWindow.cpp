#include "MainWindow.h"

#include "settingsdialog.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      mSettings(new SettingsDialog),
      mSerial(new QSerialPort(this)),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->fileNameLabel->hide();
  ui->sendButton->setEnabled(false);
  getPortsInfo();
  connect(ui->actionUstawienia_portu, &QAction::triggered, mSettings,
          &SettingsDialog::show);
  connect(mSerial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
  connect(mSerial, &QSerialPort::readyRead, this, &MainWindow::readData);
}

MainWindow::~MainWindow() {
  delete ui;
  delete mSettings;
}

void MainWindow::getPortsInfo() {
  for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts()) {
    logPortInfo(info);
  }
}

void MainWindow::logPortInfo(const QSerialPortInfo& info) {
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
  mLogger.debug("%s", qPrintable(s));
}

void MainWindow::openSerialPort() {
  const SettingsDialog::Settings p = mSettings->settings();
  mSerial->setPortName(p.name);
  mSerial->setBaudRate(p.baudRate);
  mSerial->setDataBits(p.dataBits);
  mSerial->setParity(p.parity);
  mSerial->setStopBits(p.stopBits);
  mSerial->setFlowControl(p.flowControl);
  if (mSerial->open(QIODevice::ReadWrite)) {
    ui->portOpenButton->setText("Rozłącz");
    if (!mFileContent.isEmpty()) {
      ui->sendButton->setEnabled(true);
    }
    ui->actionUstawienia_portu->setEnabled(false);
    mLogger.debug("%s",
                  qPrintable(QString(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                         .arg(p.name)
                                         .arg(p.stringBaudRate)
                                         .arg(p.stringDataBits)
                                         .arg(p.stringParity)
                                         .arg(p.stringStopBits)
                                         .arg(p.stringFlowControl))));
    showStatusMessage(tr("Połączono"));
  } else {
    QMessageBox::critical(this, tr("Error"), mSerial->errorString());

    showStatusMessage(tr("Open error"));
  }
}

void MainWindow::on_portOpenButton_released() {
  mLogger.debug("on_portOpenButton_released");
  if (mSerial->isOpen()) {
    closeSerialPort();
  } else {
    openSerialPort();
  }
}

void MainWindow::openFileAndReadContent(const QString& fileName) {
  QFile file(fileName);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  if (file.isOpen()) {
    this->statusBar()->showMessage("Otwarto plik.");
    QTextStream textStream(&file);
    mFileContent = textStream.readAll();
    ui->fileContentBrowser->setText(mFileContent);
    if(mSerial->isOpen()){
        ui->sendButton->setEnabled(true);
    }
  }
}

void MainWindow::on_fileOpenButton_released() {
  mLogger.debug("on_fileOpenButton_released");
  QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik"), "",
                                                  tr("Pliki druku (*.rct)"));
  mLogger.debug("File chosen: %s", qPrintable(fileName));
  if (!fileName.isEmpty()) {
    ui->fileNameLabel->show();
    ui->fileNameLabel->setText(fileName);
    openFileAndReadContent(fileName);
  }
}

void MainWindow::on_sendButton_released() {
  QByteArray data("\x1bP");
  data += mFileContent.toStdString().c_str();
  // TODO: checksum goes here
  data += "\x1b\\";
  writeData(data);
}

void MainWindow::showStatusMessage(const QString& message) {
  this->statusBar()->showMessage(message);
}

void MainWindow::closeSerialPort() {
  if (mSerial->isOpen()) mSerial->close();
  ui->portOpenButton->setText("Połącz");
  ui->sendButton->setEnabled(false);

  ui->actionUstawienia_portu->setEnabled(true);
  showStatusMessage(tr("Rozłączono"));
}

void MainWindow::writeData(const QByteArray& data) {
  mLogger.debug("Write data: %s", qPrintable(data));
  mSerial->write(data);
}

void MainWindow::readData() {
  const QByteArray data = mSerial->readAll();
  mLogger.debug("Read data: %s", qPrintable(data));
}

void MainWindow::handleError(QSerialPort::SerialPortError error) {
  if (error == QSerialPort::ResourceError) {
    QMessageBox::critical(this, tr("Critical Error"), mSerial->errorString());
    closeSerialPort();
  }
}

#include "MainWindow.h"

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      mLogger(new QMessageLogger),
      mSettings(new SettingsDialog),
      mSerialPortManager(mLogger),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->fileNameLabel->hide();
  ui->sendButton->setEnabled(false);
  connect(&mSerialPortManager, &SerialPortManager::serialPortError, this,
          &MainWindow::handleError);
  connect(ui->actionUstawienia_portu, &QAction::triggered, mSettings,
          &SettingsDialog::show);

  connect(ui->actionAbout_Qt, &QAction::triggered, this,
          &MainWindow::showAboutQtDialog);
  connect(ui->actionInformacje, &QAction::triggered, this,
          &MainWindow::showAboutDialog);
}

MainWindow::~MainWindow() {
  delete ui;
  delete mSettings;
}

void MainWindow::openSerialPort() {
  if (mSerialPortManager.openSerialPort(mSettings->settings())) {
    ui->portOpenButton->setText("Rozłącz");
    if (!mFileContent.isEmpty()) {
      ui->sendButton->setEnabled(true);
    }
    ui->actionUstawienia_portu->setEnabled(false);
    showStatusMessage(tr("Połączono"));
  } else {
    ui->portOpenButton->setText("Połącz");
    ui->sendButton->setEnabled(false);
    ui->actionUstawienia_portu->setEnabled(true);
    showStatusMessage(tr("Błąd połączenia"));
  }
}

void MainWindow::on_portOpenButton_released() {
  mLogger->debug("on_portOpenButton_released");
  if (mSerialPortManager.isPortOpen()) {
    closeSerialPort();
  } else {
    openSerialPort();
  }
}

void MainWindow::openFileAndReadContent(const QString& fileName) {
  QFile file(fileName);
  file.open(QIODevice::ReadOnly);
  if (file.isOpen()) {
    this->statusBar()->showMessage("Otwarto plik.");
    mFileContent = file.readAll();
    mCommandList = mFileContent.split(';');
    ui->fileContentBrowser->setText(mFileContent);
    if (mSerialPortManager.isPortOpen()) {
      ui->sendButton->setEnabled(true);
    }
  }
}

char MainWindow::getCheckSum(QByteArray data) {
  char out = static_cast<char>(0xFF);
  for (char ch : data) {
    out = out ^ ch;
  }
  return out;
}

void MainWindow::on_fileOpenButton_released() {
  mLogger->debug("on_fileOpenButton_released");
  QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik"), "",
                                                  tr("Pliki druku (*.rct)"));
  mLogger->debug("File chosen: %s", qPrintable(fileName));
  if (!fileName.isEmpty()) {
    ui->fileNameLabel->show();
    ui->fileNameLabel->setText(fileName);
    openFileAndReadContent(fileName);
  }
}

void MainWindow::on_sendButton_released() {
  for (auto& command : mCommandList) {
    QByteArray data("\x1bP");
    data += command;
    data += getCheckSum(command);
    data += "\x1b\\";
    mSerialPortManager.writeData(data);
  }
}

void MainWindow::showStatusMessage(const QString& message) {
  this->statusBar()->showMessage(message);
}

void MainWindow::showAboutDialog() {
  QMessageBox::about(this, "O programie",
                     "This software is licensed under LGPLv3 License\n"
                     "Created with Qt 5.13.1\n"
                     "More info under:\n"
                     "https://github.com/krzmazur1/SerialPortPrinter");
}

void MainWindow::showAboutQtDialog() { QMessageBox::aboutQt(this); }

void MainWindow::closeSerialPort() {
  mSerialPortManager.closeSerialPort();
  ui->portOpenButton->setText("Połącz");
  ui->sendButton->setEnabled(false);

  ui->actionUstawienia_portu->setEnabled(true);
  showStatusMessage(tr("Rozłączono"));
}

void MainWindow::handleError(QString error) {
  QMessageBox::critical(this, tr("Critical Error"), error);
}

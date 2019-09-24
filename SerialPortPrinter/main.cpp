#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QMessageLogger>


#include "MainWindow.h"

static QString LogFileName;

void customMessageHandler(QtMsgType type, const QMessageLogContext &context,
                          const QString &msg) {
  Q_UNUSED(context)

  QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
  QString txt = QString("[%1] ").arg(dt);

  switch (type) {
    case QtInfoMsg:
      txt += QString("{Info} \t\t %1").arg(msg);
      break;
    case QtDebugMsg:
      txt += QString("{Debug} \t\t %1").arg(msg);
      break;
    case QtWarningMsg:
      txt += QString("{Warning} \t %1").arg(msg);
      break;
    case QtCriticalMsg:
      txt += QString("{Critical} \t %1").arg(msg);
      break;
    case QtFatalMsg:
      txt += QString("{Fatal} \t\t %1").arg(msg);
      abort();
  }

  QFile outFile(LogFileName);
  outFile.open(QIODevice::WriteOnly | QIODevice::Append);

  QTextStream textStream(&outFile);
  textStream << txt << endl;
}

int main(int argc, char *argv[]) {
  LogFileName =
      QDateTime::currentDateTime().toString("dd-MM-yyyy-hhmmss") + ".log";
  qInstallMessageHandler(customMessageHandler);

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

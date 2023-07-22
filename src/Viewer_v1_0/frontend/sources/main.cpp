#include <QApplication>
#include <QStyleFactory>

#include "../headers/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qApp->setStyle(QStyleFactory::create("Fusion"));
  setlocale(LC_NUMERIC, "C");
  QSurfaceFormat format;
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);
  MainWindow w;
  w.show();
  return a.exec();
}

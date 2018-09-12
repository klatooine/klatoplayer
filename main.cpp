#include "klatoplayer.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  klatoplayer w;
  w.show();

  return a.exec();
}

#include "mainwindow.h"

int main(int argc, char *argv[]){

  QApplication app(argc, argv);
  app.setStyle("breeze");
  app.setFont(QFont("helvetica", 11));
  MainWindow *mainWindow = new MainWindow();
  mainWindow->show();
  return app.exec();
}

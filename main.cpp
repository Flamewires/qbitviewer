#include "mainwindow.h"
#include <QApplication>
//#include "QDebug"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (argc >= 1) {
        //qDebug() << "Setting filename" << argv[1];
        w.setFilename(QString(argv[1]));
    }
    w.show();


    return a.exec();
}

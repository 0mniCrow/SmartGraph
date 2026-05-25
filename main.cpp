#include "mainwindow.h"
#include <QApplication>

//GViewAbstractTimeObject::_time_ = Q_UINT64_C(0);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

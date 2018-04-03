
#include "mainwindow.h"
#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication _app( argc, argv );

    viz::MainWindow _mw;
    _mw.show();

    return _app.exec();
}

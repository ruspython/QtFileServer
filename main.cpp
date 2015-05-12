#include "myserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedHeight(200);
    w.setFixedWidth(300);
    w.show();

    return a.exec();
}

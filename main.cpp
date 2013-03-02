#include <QtGui>
#include "qconnect.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QConnect w;
    w.show();

    return a.exec();
}

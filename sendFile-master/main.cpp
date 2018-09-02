#include <QApplication>
#include "network.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FileServer fileServer;
    fileServer.start();

    return a.exec();
}

#include "mainwindow.h"

#include <QApplication>
#include "codeeditor.h"
#include "textEditor.h"
#include "serialconnection.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    serialconnection serial;
    serial.show();
    return a.exec();
}

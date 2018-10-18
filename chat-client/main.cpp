#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);//最后一个窗口退出时主窗口退出

    MainWindow w;
    w.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    //w.setWindowTitle("");
    w.show();

    return a.exec();
}

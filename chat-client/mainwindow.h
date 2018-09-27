#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include <QDebug>

#include <iostream>
using namespace std;

#define SERVER_IP "192.168.0.108"
#define PORT 8884

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonLogin_clicked();

private:
    Ui::MainWindow *ui;
    SOCKET m_socket;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
using namespace std;

#include <QMainWindow>
#include <winsock2.h>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>

#include "constant-macro.h"
#include "user.h"
#include "cchatgui.h"
#pragma pack(1)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButtonLogin_clicked();

private:
    Ui::MainWindow *ui;
    CUser *m_user;
    CchatGui *m_chatGui;
};

#endif // MAINWINDOW_H

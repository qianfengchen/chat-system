#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>

#include "constant-macro.h"
#include "user.h"
#include "cchatgui.h"
#include "io.h"
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

private slots:
    void on_pushButtonLogin_clicked();

private:
    Ui::MainWindow *ui;
    CUser *m_user;
    CchatGui *m_chatGui;
    CIo io;
};

#endif // MAINWINDOW_H

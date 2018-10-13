#ifndef CCHATGUI_H
#define CCHATGUI_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>

#include "constant-macro.h"

namespace Ui {
class CchatGui;
}

class CchatGui : public QDialog
{
    Q_OBJECT

public:
    explicit CchatGui(QWidget *parent = 0);
    ~CchatGui();

    void closeEvent(QCloseEvent *event);
    void setUserList(char *userList);
    int getUserNum(char *userList);
    void setUsernameIdMap(char *userList);

private slots:
    void on_pushButtonQuit_clicked();

private:
    Ui::CchatGui *ui;
    map<int, char*> m_mapUserNameId;
};

#endif // CCHATGUI_H

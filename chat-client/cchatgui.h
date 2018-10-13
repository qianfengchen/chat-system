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
    void setUserList(string userList);
    int getUserNum(string userList);
    void setUsernameIdMap(string userList);
    int substring(char *strAll, const char *strFind);
    void setUserListstrToList(string userList);

private slots:
    void on_pushButtonQuit_clicked();

private:
    Ui::CchatGui *ui;
    map<string, string> m_mapUserNameId;
    int m_onLineUserNumwithoutMe;
    list<string> m_userlist;
};

#endif // CCHATGUI_H

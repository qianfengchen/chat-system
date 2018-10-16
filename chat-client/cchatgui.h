#ifndef CCHATGUI_H
#define CCHATGUI_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

#include "constant-macro.h"
#include "user.h"
#include "savelist.h"

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
    void setUserFromMainwindow(CUser *user);
    void setReadListFromIO(CSaveList *list);

private slots:
    void on_pushButtonQuit_clicked();
    void on_pushButtonSend_clicked();
    void dealWithMsg();

private:
    Ui::CchatGui *ui;
    map<int, string> m_mapUserNameId;
    int m_onLineUserNumwithoutMe;
    list<string> m_userlist;
    CUser *m_user;
    CSaveList *m_readList;
    QTimer *m_dealMsgTime;
};

#endif // CCHATGUI_H

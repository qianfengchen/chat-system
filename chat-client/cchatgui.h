#ifndef CCHATGUI_H
#define CCHATGUI_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QTextBrowser>
#include <QTableWidgetItem>

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
    void addUser(string userStr);
    void deleteUser(string userStr);

private slots:
    void on_pushButtonQuit_clicked();
    void on_pushButtonSend_clicked();
    void dealWithMsg();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::CchatGui *ui;
    map<int, string> m_mapUserNameId;
    map<int, QTableWidgetItem*> m_mapUserIdItem;
    map<string, QTextBrowser*> m_mapUserNameBrowser;
    int m_onLineUserNum;
    list<string> m_userlist;
    CUser *m_user;
    CSaveList *m_readList;
    QTimer *m_dealMsgTime;
    int nowSendtoWhichId;
};

#endif // CCHATGUI_H

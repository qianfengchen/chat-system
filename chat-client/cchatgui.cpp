#include "cchatgui.h"
#include "ui_cchatgui.h"

CchatGui::CchatGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CchatGui)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(1);//表格只有一列
    QStringList mHeader;
    mHeader << QString("在线列表");
    ui->tableWidget->setHorizontalHeaderLabels(mHeader);
    //ui->tableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);//表格中的文字左对齐
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);//表格自动扩展到充满表格
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑

    m_dealMsgTime = new QTimer;
    connect(m_dealMsgTime, SIGNAL(timeout()), this, SLOT(dealWithMsg()));
    m_dealMsgTime->start(500);

    nowSendtoWhichId = 0;
    ui->pushButtonSend->setDisabled(true);
}

CchatGui::~CchatGui()
{
    delete ui;
}

void CchatGui::setReadListFromIO(CSaveList *list)
{
    m_readList = list;
}

int CchatGui::substring(char *strAll, const char *strFind)
{
    int num = 0;
    char *p = NULL;//任意附个初始值
    do {
        p = strstr(strAll, strFind);//1.p指针指向strstr的返回值。3.再一次循环到 这里函数的参数发生变化，p重新指向strstr返回值，如此循环。
        if (p != NULL) {
            strAll = p + 1;//2.str同样指向strstr返回值p的下一个地址。
            num = num + 1;
        }
    } while (p != NULL);
    return num;
}

int CchatGui::getUserNum(string userList)
{
    int userNum = 0;
    char strUserList[20];

    int fi = userList.find("&&", 0);
    while (fi != (int)userList.npos)
    {
        userNum ++;
        fi = userList.find("&&", fi + 1);
    }
    sprintf(strUserList, "%d", userNum);
    ui->labelOnlineNum->setText(strUserList);
    return userNum;
}

void CchatGui::setUserListstrToList(string userList)
{
    int start = 0;
    int end = userList.find("&&", 0);
    while (end != (int)userList.npos)
    {
        m_userlist.push_back(userList.substr(start, end - start));
        start = end + 2;
        end = userList.find("&&", end + 1);
    }
}

void CchatGui::setUsernameIdMap(string userList)
{
    string strName;
    string strUserid;
    setUserListstrToList(userList);
    list<string>::iterator iter;
    for (iter = m_userlist.begin(); iter != m_userlist.end(); iter++) {
        strName = (*iter).substr(0, (*iter).find(",", 0));
        strUserid = (*iter).substr((*iter).find(",", 0) + 1, (*iter).size());
        m_mapUserNameId.insert(pair<int, string>(atoi(strUserid.c_str()), strName));
        QTextBrowser *browser = new QTextBrowser;
        m_mapUserNameBrowser.insert(pair<string, QTextBrowser*>(strName, browser));
    }
}

void CchatGui::setUserList(string userList)
{
    ui->tableWidget->clearContents();//首先清空
    m_onLineUserNum = getUserNum(userList);//设置界面在线人数显示
    ui->tableWidget->setRowCount(m_onLineUserNum);//设置行数

    if (m_onLineUserNum > 0)
    {
        setUsernameIdMap(userList);
        map<int, string>::iterator iter;
        int i = 1;
        for(iter = m_mapUserNameId.begin(); iter != m_mapUserNameId.end(); iter++) {

            QTableWidgetItem *item = new QTableWidgetItem(iter->second.data());
            m_mapUserIdItem.insert(pair<int, QTableWidgetItem*>(iter->first, item));
            ui->stackedWidget->addWidget(m_mapUserNameBrowser[iter->second.data()]);
            if (iter->second.data() == m_user->m_userName)
                ui->tableWidget->setItem(0, 0, item);
            else
                ui->tableWidget->setItem(i, 0, item);
            i++;
        }
    }
}

void CchatGui::addUser(string userStr)
{
    string strName = userStr.substr(0, userStr.find(",", 0));
    string strUserid = userStr.substr(userStr.find(",", 0) + 1, userStr.size());

    cout << strName << " " << strUserid << endl;

    m_mapUserNameId.insert(pair<int, string>(atoi(strUserid.c_str()), strName));
    QTextBrowser *browser = new QTextBrowser;
    m_mapUserNameBrowser.insert(pair<string, QTextBrowser*>(strName, browser));
    ui->stackedWidget->addWidget(browser);

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    m_onLineUserNum++;
    QTableWidgetItem *item = new QTableWidgetItem(strName.data());
    m_mapUserIdItem.insert(pair<int, QTableWidgetItem*>(atoi(strUserid.c_str()), item));
    ui->tableWidget->setItem(m_onLineUserNum-1, 0, item);

    char strUserList[20];
    sprintf(strUserList, "%d", m_onLineUserNum);
    ui->labelOnlineNum->setText(strUserList);
}

void CchatGui::deleteUser(string userStr)
{
    string strName = userStr.substr(0, userStr.find(",", 0));
    string strUserid = userStr.substr(userStr.find(",", 0) + 1, userStr.size());

    m_onLineUserNum--;
    char strUserList[20];
    sprintf(strUserList, "%d", m_onLineUserNum);
    ui->labelOnlineNum->setText(strUserList);

    delete m_mapUserNameBrowser[strName];
    m_mapUserNameBrowser.erase(strName);

    int row = ui->tableWidget->row(m_mapUserIdItem[atoi(strUserid.c_str())]);
    ui->tableWidget->removeRow(row);

    m_mapUserIdItem.erase(atoi(strUserid.c_str()));
    m_mapUserNameId.erase(atoi(strUserid.c_str()));
}

void CchatGui::closeEvent(QCloseEvent *event)
{
    switch(QMessageBox::information(this, tr("exit tip"), tr("Do you really want exit?"), tr("Yes"), tr("No"), 0, 1))
    {
        case 0:
             event->accept();
             break;
        case 1:
        default:
             event->ignore();
             break;
    }
}

void CchatGui::on_pushButtonQuit_clicked()
{
    this->close();
}

void CchatGui::setUserFromMainwindow(CUser *user)
{
    m_user = user;
}

void CchatGui::on_pushButtonSend_clicked()
{
    QString qmsg = ui->textEdit->toPlainText();
    string msg = qmsg.toStdString();

    m_user->get_send();
    m_user->m_MsgSend = (struct messageSend *)m_user->sendbuff;
    strcpy(m_user->m_MsgSend->msg, msg.data());
    m_user->m_MsgSend->head.version = 1;
    m_user->m_MsgSend->head.userId  = m_user->m_userId;
    m_user->m_MsgSend->head.length  = 16 + 4 + strlen(m_user->m_MsgSend->msg) + 1;
    m_user->remainLengthofSend = m_user->m_MsgSend->head.length;
    m_user->m_MsgSend->head.cmdId = SENDtoONE;
    m_user->m_MsgSend->sayToClientId = nowSendtoWhichId;
    m_user->haveSendFlag = 1;

    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss");
    QString tmpStr = "[me] " + timeStr + "\n" + qmsg + "\n";

    m_mapUserNameBrowser[m_mapUserNameId[nowSendtoWhichId]]->setAlignment(Qt::AlignRight);
    m_mapUserNameBrowser[m_mapUserNameId[nowSendtoWhichId]]->insertPlainText(tmpStr);

    ui->textEdit->clear();
}

void CchatGui::dealWithMsg()
{
    if (m_readList->getPackSize() > 0)
    {
        char *pack = m_readList->packPop();
        struct baseHandle *tmp = (struct baseHandle *)pack;
        switch(tmp->cmdId)
        {
            case USERLIST:
                m_user->m_userList = (struct userList *)pack;
                setUserList(m_user->m_userList->userlist);
                delete pack;
            break;
            case USERQUIT:
                m_user->m_userList = (struct userList *)pack;
                deleteUser(m_user->m_userList->userlist);
                delete pack;
            break;
            case USERLOGIN:
                m_user->m_userList = (struct userList *)pack;
                addUser(m_user->m_userList->userlist);
                delete pack;
            break;
            case SENDtoONE:
                m_user->m_MsgRecv = (struct messageRecv *)pack;
                QDateTime time = QDateTime::currentDateTime();
                QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss");
                QString tmpStr = "[" + QString::fromStdString(m_mapUserNameId[m_user->m_MsgRecv->recvFromWhichId]) + "] " +
                              timeStr + "\n" + QString(m_user->m_MsgRecv->msg) + "\n";
                m_mapUserNameBrowser[m_mapUserNameId[m_user->m_MsgRecv->recvFromWhichId]]->setAlignment(Qt::AlignLeft);
                m_mapUserNameBrowser[m_mapUserNameId[m_user->m_MsgRecv->recvFromWhichId]]->insertPlainText(tmpStr);
                delete pack;
            break;
//            case SENDtoOTHERS:
//                m_user->m_MsgRecv = (struct messageRecv *)pack;
//                QDateTime time = QDateTime::currentDateTime();
//                QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss");
//                QString tmp = "[" + QString::fromStdString(m_mapUserNameId[m_user->m_MsgRecv->recvFromWhichId]) + "] " +
//                              timeStr + "\n" + QString(m_user->m_MsgRecv->msg) + "\n";
//                map<string, QTextBrowser*>::iterator iter;
//                for(iter = m_mapUserNameBrowser.begin(); iter != m_mapUserNameBrowser.end(); iter++) {
//                    iter->second->insertPlainText(tmp);
//                }
//                delete pack;
//            break;
        }
    }
}

void CchatGui::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setFocus();
}

void CchatGui::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->textEdit->setFontPointSize(arg1.toDouble());
    ui->textEdit->setFocus();
}

void CchatGui::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    QString nowSelectUsername;
    ui->stackedWidget->setCurrentWidget(m_mapUserNameBrowser[item->text().toStdString()]);
    map<int, string>::iterator iter;
    for(iter = m_mapUserNameId.begin(); iter != m_mapUserNameId.end(); iter++) {
        if (iter->second == item->text().toStdString()) {
            nowSendtoWhichId = iter->first;
            nowSelectUsername = QString::fromStdString(iter->second);
        }
    }
    ui->labelWho->setText(nowSelectUsername);
    ui->pushButtonSend->setEnabled(true);
}

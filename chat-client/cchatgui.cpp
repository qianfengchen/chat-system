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
}

CchatGui::~CchatGui()
{
    delete ui;
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
    if (strcmp(userList.data(), "nouser") == 0) {//没有在线用户
        ui->labelOnlineNum->setText("0");//自己一个人在线
        return 0;
    }

    int fi = userList.find("&&", 0);
    while (fi != userList.npos)
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
    while (end != userList.npos)
    {
        m_userlist.push_back(userList.substr(start, end - start));
        start = end + 2;
        end = userList.find("&&", end + 1);
    }
}

void CchatGui::setUsernameIdMap(string userList)
{
    int userid;
    string strName;
    string strUserid;
    setUserListstrToList(userList);
    //cout << m_userlist.size() << endl;
    list<string>::iterator iter;
    for (iter = m_userlist.begin(); iter != m_userlist.end(); iter++) {
        //cout << *iter << endl;
        strName = (*iter).substr(0, (*iter).find(",", 0));
        strUserid = (*iter).substr((*iter).find(",", 0) + 1, (*iter).size());
        //cout << strName << " " << strUserid << endl;
        m_mapUserNameId.insert(pair<string, string>(strUserid, strName));
    }
}


void CchatGui::setUserList(string userList)
{
    m_onLineUserNumwithoutMe = getUserNum(userList);//设置界面在线人数显示
    if (m_onLineUserNumwithoutMe != 0)
    {
        setUsernameIdMap(userList);

        ui->tableWidget->setRowCount(m_onLineUserNumwithoutMe);//设置行数

        map<string, string>::iterator iter;
        int i = 0;
        for(iter = m_mapUserNameId.begin(); iter != m_mapUserNameId.end(); iter++) {

            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(iter->second.data()));
            i++;
        }
    }
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

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

int substring(char *strAll, const char *strFind)
{
    int num = 0;
    char *p = NULL;//任意附个初始值
    do {
        p = strstr(strAll, strFind);//1.p指针指向strstr的返回值。3.再一次循环到 这里函数的参数发生变化，p重新指向strstr返回值，如此循环。
        if(p != NULL) {
            strAll = p + 1;//2.str同样指向strstr返回值p的下一个地址。
            num = num + 1;
        }
    } while(p != NULL);
    return num;
}


int CchatGui::getUserNum(char *userList)
{
    int userNum = 0;
    char strUserNum[20];
    if (strcmp(userList, "nouser") == 0) {//没有在线用户
        ui->labelOnlineNum->setText("1");//自己一个人在线
        return 0;
    }

    userNum = substring(userList, "&&");
    sprintf(strUserNum, "%d", userNum + 1);//加一是自己
    ui->labelOnlineNum->setText(strUserNum);
    return userNum;
}

void CchatGui::setUsernameIdMap(char *userList)
{
    char *strName = new char[50];
    strcpy(strName, "abc");
    m_mapUserNameId.insert(pair<int, char*>(1, strName));
    m_mapUserNameId.insert(pair<int, char*>(2, strName));
    m_mapUserNameId.insert(pair<int, char*>(3, strName));
    m_mapUserNameId.insert(pair<int, char*>(4, strName));
}


void CchatGui::setUserList(char *userList)
{
    char tmpUserList[USERLISTSTRLENGTH];
    strcpy(tmpUserList, userList);

    int userNum = getUserNum(tmpUserList);//设置界面在线人数显示
    setUsernameIdMap(tmpUserList);

    ui->tableWidget->setRowCount(userNum);//设置行数
    //int row = ui->tableWidget->rowCount();//插入行之前先得获取表的行数
    //ui->tableWidget->insertRow(row);//插入一行
    for (int i=0; i<userNum; i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(tmpUserList));
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

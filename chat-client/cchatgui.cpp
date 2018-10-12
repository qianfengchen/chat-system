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

int CchatGui::getUserNum(char *userList)
{
    int userNum;


    return userNum;
}

void CchatGui::setUserList(char *userList)
{
    char tmpUserList[USERLISTLENGTH];
    strcpy(tmpUserList, userList);

    ui->tableWidget->setRowCount(3);//设置行数
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);//插入一行

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(tmpUserList));
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

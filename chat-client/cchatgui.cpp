#include "cchatgui.h"
#include "ui_cchatgui.h"

CchatGui::CchatGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CchatGui)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(1);
    QStringList mHeader;
    mHeader<<QString("在线列表");
    ui->tableWidget->setHorizontalHeaderLabels(mHeader);
    ui->tableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("name"));
}

CchatGui::~CchatGui()
{
    delete ui;
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

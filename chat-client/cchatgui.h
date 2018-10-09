#ifndef CCHATGUI_H
#define CCHATGUI_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>

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

private slots:
    void on_pushButtonQuit_clicked();

private:
    Ui::CchatGui *ui;
};

#endif // CCHATGUI_H

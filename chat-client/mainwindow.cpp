#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "WSAStartup failed" << endl;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
        cout << "invalid socket" << endl;
    }

    SOCKADDR_IN servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

    if (::connect(m_socket, (sockaddr *)&servaddr, sizeof(servaddr)) == SOCKET_ERROR)
    {
        cout << "connect error !" << endl;
        cout << WSAGetLastError() << endl;
        closesocket(m_socket);
    } else {
        cout << "connect success !" << endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonLogin_clicked()
{

}

#include "mainwindow.h"
#include "ui_mainwindow.h"

CRITICAL_SECTION CriticalSection;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_user = new CUser;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "WSAStartup failed" << endl;
    }

    m_user->m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_user->m_sockfd == INVALID_SOCKET)
    {
        cout << "invalid socket" << endl;
    }

    m_user->m_servaddr.sin_family = AF_INET;
    m_user->m_servaddr.sin_port = htons(PORT);


    ui->lineEditUsername->setText("admin");
    ui->lineEditPasswd->setText("admin");
    ui->lineEditServerIp->setText("192.168.");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_user;
    delete m_chatGui;
}

void MainWindow::on_pushButtonLogin_clicked()
{
    m_user->m_servaddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
    //m_user->m_servaddr.sin_addr.S_un.S_addr = inet_addr(ui->lineEditServerIp->text().toLatin1());
    if (::connect(m_user->m_sockfd, (sockaddr *)&m_user->m_servaddr, sizeof(m_user->m_servaddr)) == SOCKET_ERROR)
    {
        cout << "connect error !" << endl;
        cout << WSAGetLastError() << endl;
        closesocket(m_user->m_sockfd);
    } else {
        cout << "connect success !" << endl;
    }

    memset(m_user->m_loginSendMsg.name, 0, 50);
    strcpy(m_user->m_loginSendMsg.name, ui->lineEditUsername->text().toLatin1());
    memset(m_user->m_loginSendMsg.passwd, 0, 20);
    strcpy(m_user->m_loginSendMsg.passwd, ui->lineEditPasswd->text().toLatin1());

    int ret;
    m_user->m_loginSendMsg.loginHead.cmdId = LOGIN;
    m_user->m_loginSendMsg.loginHead.version = 1;
    m_user->m_loginSendMsg.loginHead.length  = sizeof(loginMessageSend);
    ret = send(m_user->m_sockfd, (const char *)&m_user->m_loginSendMsg, m_user->m_loginSendMsg.loginHead.length, 0);
    if (ret <= 0)
    {
        return;
    }
    ret = recv(m_user->m_sockfd, (char *)&m_user->m_loginRecvMsg, sizeof(loginMessageRecv), 0);
    if (ret <= 0)
    {
        return;
    }
    if(strcmp(m_user->m_loginRecvMsg.result, "N") == 0)
    {
        cout << "input error" << endl;
        exit(0);
    }else
    {
        InitializeCriticalSection(&CriticalSection);

        unsigned long ul = 1;
        int nRet = ioctlsocket(m_user->m_sockfd, FIONBIO, (unsigned long*)&ul);
        //设置套接字非阻塞模式,必须在connect以后设置成非阻塞不然会connect失败返回10035
        //fcntl(m_user->m_sockfd, F_SETFL, O_NONBLOCK);//linux
        if (nRet == SOCKET_ERROR)
        {
            cout << "设置套接字非阻塞模式，失败处理" << endl;
        }

        cout << "登录成功！" << endl;
        cout << "your userId is: " << m_user->m_loginRecvMsg.loginHead.userId << endl;
        m_user->m_userId = m_user->m_loginRecvMsg.loginHead.userId;
        cout << m_user->m_loginRecvMsg.result << endl;

        io.setUserFromMainwindow(m_user);
        io.ioStart();

        this->hide();
        m_chatGui = new CchatGui();
        m_chatGui->setUserList(m_user->m_loginRecvMsg.result);
        m_chatGui->setUserFromMainwindow(m_user);
        m_chatGui->setReadListFromIO(io.getReadList());
        m_chatGui->show();
    }
}

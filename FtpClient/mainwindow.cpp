#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>

#include <fstream>
#include <QFile>

const int portNumber = 1488;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = nullptr;

    init();
}

void MainWindow::doConnect()
{
    if(!socket)
    {
        socket = new QTcpSocket(this);

        connect(socket, &QAbstractSocket::connected,
                this, &MainWindow::slotConnected);
        connect(socket, static_cast<void(QTcpSocket::*)
                (QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                this, &MainWindow::slotError);
        connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onChangeDir);
        connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onDownloadFile);
        connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onUploadFile);
    }

    qDebug() << "connecting...";

    socket->connectToHost("localhost", portNumber);
    socket->write("Hello");
}

void MainWindow::slotError(QAbstractSocket::SocketError err)
{
    switch(err)
    {
    case QAbstractSocket::HostNotFoundError :
        qDebug() << "Error: The host was not found.";
        break;
    case QAbstractSocket::RemoteHostClosedError :
        qDebug() << "Error: The remote host is closed.";
        break;
    case QAbstractSocket::ConnectionRefusedError :
        qDebug() << "Error: The connection was refused.";
        break;
    default:
        qDebug() << "The following error occured: " << socket->errorString();
    }
}

void MainWindow::onChangeDir()
{
    std::string buffer;

    buffer.resize(2);

    *reinterpret_cast<short*>(&buffer[0]) = 0;

    buffer += "Change dir plz";

    socket->write( buffer.c_str(), buffer.size() );
}

void MainWindow::onDownloadFile()
{
    std::string buffer;

    buffer.resize(2);

    *reinterpret_cast<short*>(&buffer[0]) = 1;

    buffer += "Download file plz";

    socket->write( buffer.c_str(), buffer.size() );
}

void MainWindow::onUploadFile()
{

    QFile readStream("data.png");

    if( !readStream.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "Can`t open file";
        return;
    }

    qDebug() << readStream.size();

    QByteArray data( 2, '0' );

    *reinterpret_cast<short*>( data.data() ) = 2;

    data.append( readStream.readAll() );

    socket->write( data.data(), data.size() );
}

void MainWindow::slotConnected()
{
    qDebug() << "connected...";
}

void MainWindow::init()
{
    connect(ui->connectButton, &QPushButton::clicked,
            this, &MainWindow::doConnect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

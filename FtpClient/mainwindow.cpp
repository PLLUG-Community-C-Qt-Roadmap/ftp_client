#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>

#include <fstream>
#include <QFile>

#include <QMessageBox>

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
    buffer.resize(6);

    buffer[0] = static_cast<char>(0);  // Operation code (1 byte)
    buffer[1] = static_cast<char>(0);  // Error code (1 byte)
    int *intPtr = reinterpret_cast<int *>(&buffer[2]); // Additional options (4 bytes)
    intPtr[0]=0;
     buffer += "C:\\FTPclient"; // Data (max: 1024 bytes)
//    buffer += "C:\\ERROR_PATH"; // Data (max: 1024 bytes)

    socket->write( buffer.c_str(), buffer.size() );

    socket->waitForReadyRead();
    QByteArray receivedData = socket->readAll();

    short operationCode = static_cast<short>(receivedData[0]);
    short errorCode = static_cast<short>(receivedData[1]);
    intPtr = reinterpret_cast<int *>(receivedData.data()+2);
    int additionalOptions = intPtr[0];

    QString data(receivedData.data() + 6);

    if(errorCode)
    {
        QMessageBox *errorMessage = new QMessageBox(this);
        errorMessage->setWindowTitle("ERROR");
        errorMessage->setText(data);
        errorMessage->setGeometry(650,250,600,300);
        errorMessage->show();
    }
    else
    {
        qDebug () << data;
    }
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

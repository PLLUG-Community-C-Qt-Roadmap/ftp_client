#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>
#include <fstream>
#include <QFile>
#include <QMessageBox>
#include <stdexcept>

#include "listviewcontroller.h"

const int portNumber = 1488;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
<<<<<<< HEAD
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

        connect(ui->changeDirButton, &QPushButton::clicked, this, &MainWindow::onChangeDir);
        connect(ui->downloadFileButton, &QPushButton::clicked, this, &MainWindow::onDownloadFile);
        connect(ui->uploadFileButton, &QPushButton::clicked, this, &MainWindow::onUploadFile);
    }
=======
>>>>>>> 893b028e9dddd65d3c343974f89d4790d0a95d02

    connect(ui->connectButton, &QPushButton::clicked,
            this, &MainWindow::connectButtonClicked);
    connect(ui->uploadFileButton, &QPushButton::clicked,
            this, &MainWindow::uploadButtonClicked);
    connect(ui->filesView, &QTreeView::doubleClicked,
            this, &MainWindow::viewItemClicked);

    new ListViewController(this, this);
}

void MainWindow::setModel(QAbstractItemModel *model)
{
    ui->filesView->setModel(model);
}

void MainWindow::showConnectButton()
{
    ui->connectButton->show();
}

void MainWindow::hideConnectButton()
{
    ui->connectButton->hide();
}

void MainWindow::showUploadButton()
{
    ui->uploadFileButton->show();
}

void MainWindow::hideUploadButton()
{
    ui->uploadFileButton->hide();
}

void MainWindow::showFilesView()
{
    ui->filesView->show();
}

void MainWindow::hideFilesView()
{
     ui->filesView->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

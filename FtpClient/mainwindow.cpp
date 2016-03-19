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

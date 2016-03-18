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

    new ListViewController(this, this);
}

void MainWindow::setModel(QAbstractItemModel *model)
{
    ui->filesView->setModel(model);
}

QPushButton *MainWindow::getUploadPushButton()
{
    return ui->uploadFileButton;
}

QPushButton *MainWindow::getConnectPushButton()
{
    return ui->connectButton;
}

QTreeView *MainWindow::getFilesView()
{
    return ui->filesView;
}

MainWindow::~MainWindow()
{
    delete ui;
}

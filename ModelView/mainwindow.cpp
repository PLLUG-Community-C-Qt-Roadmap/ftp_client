#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listviewcontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->treeView, &QTreeView::doubleClicked, this, &MainWindow::itemDoubleClicked);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddItem);

    new ListViewController(this, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setModel(QAbstractItemModel *model)
{
    ui->treeView->setModel(model);
}

void MainWindow::onAddItem()
{
    bool isOk = false;
    int value = ui->ageEdit->text().toInt(&isOk);
    if(isOk && !ui->textEdit->text().isEmpty())
    {
        addItemClicked(ui->textEdit->text(), value);
    }
    else
        qDebug("Empty fields");


}

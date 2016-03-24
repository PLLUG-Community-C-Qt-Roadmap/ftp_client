#include "listviewcontroller.h"

#include "mainwindow.h"

#include <QDebug>

ListViewController::ListViewController(MainWindow *view, QObject *parent) : QObject(parent)
{
    _view = view;


    connect(_view, &MainWindow::itemDoubleClicked, this, [](const QModelIndex &index) {
        qDebug() << "Item clicked" << index.row() << index.column();
    });

//    connect(_view, &MainWindow::addItemClicked, this, [=](QString name, int age) {
//        _model->insertRow(_model->rowCount());
//        auto nameColumn = _model->index(_model->rowCount() - 1, 0);
//        auto ageColumn = _model->index(_model->rowCount() - 1, 1);

//        _model->setData(nameColumn, name, Qt::DisplayRole);
//        _model->setData(ageColumn, age, Qt::DisplayRole);
//    });


    auto model = new ListModel(this);

    _view->setModel(model);

    _model = model;

    _model->testMethod();
}

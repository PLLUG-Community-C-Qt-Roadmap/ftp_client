#ifndef LISTVIEWCONTROLLER_H
#define LISTVIEWCONTROLLER_H

#include <QObject>

#include "listmodel.h"

class MainWindow;

class ListViewController : public QObject
{
    Q_OBJECT
public:
    explicit ListViewController(MainWindow *view, QObject *parent = 0);

signals:

public slots:

private:
    ITest *_model;
    MainWindow *_view;
};

#endif // LISTVIEWCONTROLLER_H

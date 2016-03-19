#ifndef LISTVIEWCONTROLLER_H
#define LISTVIEWCONTROLLER_H

#include <QObject>
#include <QMessageBox>
#include "listmodel.h"
#include "clientcontext.h"
#include "mainwindow.h"

class MainWindow;
const QString initialFolder = "C:\\FTPclient";

class ListViewController : public QObject
{
    Q_OBJECT
public:
    explicit ListViewController(MainWindow *view, QObject *parent = 0);

public slots:
    void doConnect();
    void itemDoubleClicked(const QModelIndex &index);
    void slotConnected();
    void slotDisconnected();

private:
    void makeUIConnections();
    void initializeView();

private:
    IListModel *mModel;
    MainWindow *mView;
    ClientContext *mClient;
    QString mCurrDir;
};

#endif // LISTVIEWCONTROLLER_H


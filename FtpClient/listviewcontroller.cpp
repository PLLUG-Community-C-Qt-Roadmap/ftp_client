#include "listviewcontroller.h"

ListViewController::ListViewController(MainWindow *view, QObject *parent) : QObject(parent)
{
    mView = view;
    mClient = new ClientContext(view);
    mCurrDir = initialFolder;

    mView->setWindowTitle("FtpClient");
    mView->hideFilesView();
    mView->hideUploadButton();

    connect(mView, &MainWindow::connectButtonClicked, this, [=](){
        mClient->doConnect();
    });

    connect(mClient, &ClientContext::signalConnected,
            this, &ListViewController::slotConnected);
    connect(mClient, &ClientContext::signalDisconnected,
            this, &ListViewController::slotDisconnected);
}

void ListViewController::itemDoubleClicked(const QModelIndex &index)
{
    try
    {
        if(index.model()->index(index.row(),2).data().toString() == "Folder")
        {
            QString folderName = index.model()->index(index.row(),0).data().toString();
            mCurrDir += "\\" + folderName;
            mModel->refreshData(mClient->changeDir(mCurrDir));
            mView->setWindowTitle(folderName);
        }
        else
        {
            mClient->downloadFile();
        }
    }
    catch(const std::exception &e)
    {
        QMessageBox::information(mView, "ERROR", e.what());
    }
}

void ListViewController::slotConnected()
{
    QMessageBox::information(mView, "Connected", "Connection is successfull!");
    initializeView();
    mView->hideConnectButton();
    mView->showFilesView();
    mView->showUploadButton();
    mView->setWindowTitle(mCurrDir);
    makeUIConnections();
}

void ListViewController::slotDisconnected() // In case of reconnection displays last accessed folder
{
     QMessageBox::information(mView, "Disconnected", "Connection failed!");
     mView->hideFilesView();
     mView->hideUploadButton();
     mView->showConnectButton();
     mView->setWindowTitle("FtpClient");
}

void ListViewController::makeUIConnections()
{
    connect(mView, &MainWindow::uploadButtonClicked,
            mClient, &ClientContext::onUploadFile);

    connect(mView, &MainWindow::viewItemClicked,
            this, &ListViewController::itemDoubleClicked);
}

void ListViewController::initializeView()
{
    mModel = new ListModel(mClient->changeDir(mCurrDir), mView);
    mView->setModel(dynamic_cast<QAbstractItemModel *>(mModel));
}

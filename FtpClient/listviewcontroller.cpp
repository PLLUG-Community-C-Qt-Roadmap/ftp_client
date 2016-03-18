#include "listviewcontroller.h"

ListViewController::ListViewController(MainWindow *view, QObject *parent) : QObject(parent)
{
    mView = view;
    mClient = new ClientContext(view);
    mCurrDir = initialFolder;

    mView->getFilesView()->hide();  // We cannot display files before the connection is established
    mView->getUploadPushButton()->hide();   // Also there's no need in upload button yet

    connect(mView->getConnectPushButton(), &QPushButton::clicked,
            this, &ListViewController::doConnect);
}

ListViewController::~ListViewController()
{
    delete mModel;
    delete mView;
    delete mClient;
}

void ListViewController::doConnect()
{
    mClient->doConnect(mView);

    initializeView();

    makeConnections();

    mView->getConnectPushButton()->hide();
    mView->getFilesView()->show();
    mView->getUploadPushButton()->show();
    mView->setWindowTitle(mCurrDir);
}

void ListViewController::itemDoubleClicked(const QModelIndex &index)
{
    if(index.model()->index(index.row(),2).data().toString() == "Folder")
    {
        QString folderName = index.model()->index(index.row(),0).data().toString();
        mCurrDir += "\\" + folderName;
        mModel->refreshData(mClient->changeDir(mCurrDir, mView));
        mView->setWindowTitle(folderName);
    }
    else
    {
        mClient->downloadFile();
    }
}

void ListViewController::makeConnections()
{
    connect(mView->getUploadPushButton(), &QPushButton::clicked,
            mClient, &ClientContext::onUploadFile);

    connect(mView->getFilesView(), &QTreeView::doubleClicked,
            this, &ListViewController::itemDoubleClicked);
}

void ListViewController::initializeView()
{
    mModel = new ListModel(mClient->changeDir(mCurrDir, mView), mView);
    mView->setModel(dynamic_cast<QAbstractItemModel *>(mModel));
}

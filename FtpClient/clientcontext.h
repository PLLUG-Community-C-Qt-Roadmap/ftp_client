#ifndef CLIENTCONTEXT_H
#define CLIENTCONTEXT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <vector>
#include <QStringList>
#include "listmodel.h"
#include "packet.h"
#include "mainwindow.h"

class ClientContext: public QObject
{
    Q_OBJECT

public:
    explicit ClientContext(QObject *parent = 0);
    virtual ~ClientContext();

    void doConnect(MainWindow *mainWindow);
    std::vector<ModelEntity> changeDir(QString path, MainWindow *mainWindow);
    void downloadFile();

public slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError err);
    void onUploadFile();

private:
    std::vector<ModelEntity> getFilesVector(const QString& files, MainWindow *mainWindow);
    void sendPath(const QString& path);

private:
    QTcpSocket * mSocket;
};

#endif // CLIENTCONTEXT_H

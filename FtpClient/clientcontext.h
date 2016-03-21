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

class ClientContext: public QObject
{
    Q_OBJECT

public:
    explicit ClientContext(QObject *parent = 0);
    virtual ~ClientContext();

    void doConnect();
    std::vector<ModelEntity> changeDir(QString path);
    void downloadFile();

signals:
   void signalConnected();
   void signalDisconnected();

public slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError err);
    void onUploadFile();

private:
    std::vector<ModelEntity> getFilesVector(const QString& files);
    std::vector<ModelEntity> getFilesVector(const std::vector<Packet> &packets);
    void sendPath(const QString& path);

private:
    QTcpSocket * mSocket;
};

#endif // CLIENTCONTEXT_H

#ifndef TASKS_HPP
#define TASKS_HPP

#include <QObject>
#include <iostream>
#include <fstream>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

static const size_t numberOfBytes = 1024;

class ClientTask : public QObject
{
    Q_OBJECT
public:
    ClientTask(QObject *parent = 0) : QObject(parent) {}

public slots:
    void run()
    {
        connect(&client, &QTcpSocket::connected, this, &ClientTask::sendFile);
        connect(&client, SIGNAL(error(QAbstractSocket::SocketError)), this,
                SLOT(handleError(QAbstractSocket::SocketError)));
        //connect(&client, &QTcpSocket::disconnected, this, &ClientTask::finished);
        client.connectToHost("localhost", 1488);
    }

signals:
    void finished();

private slots:
    void handleError(QAbstractSocket::SocketError err)
    {
        qDebug() << err;
    }

private:
    void sendFile()
    {
        long data = 1488;
        char *ptr = reinterpret_cast<char*>(&data);
        qDebug() << "File sent" << client.write(ptr, 4);
    }

private:
    QTcpSocket client;
};

class ServerTask : public QObject
{
    Q_OBJECT
public:
    ServerTask(QObject *parent = 0) : QObject(parent)
    {
    }

public slots:
    void run()
    {
        connect(&serverSocket, &QTcpServer::newConnection, this, &ServerTask::recieveFile, Qt::DirectConnection);
        serverSocket.listen(QHostAddress::LocalHost, 1488);
    }

    void recieveFile()
    {
        qDebug() << "New connection";
        auto socket = serverSocket.nextPendingConnection();

        long size;

        qDebug() << "Wait: " << socket->waitForReadyRead(300);

        auto sizeResult = socket->read(reinterpret_cast<char*>(&size), 4);

        qDebug() << sizeResult << size;

        if(sizeResult != 4)
            throw std::runtime_error("funcking wrong");

        qDebug() << "File received";
    }

signals:
    void finished();

private:
    QTcpServer serverSocket;
};

#endif // TASKS_HPP


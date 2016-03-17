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
        // Do processing here
        std::cout << "CLient\n";
        emit finished();
    }

signals:
    void finished();
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
        connect(&serverSocket, &QTcpServer::newConnection, this, &ServerTask::recieveFile);
    }

    void recieveFile()
    {
        auto socket = serverSocket.nextPendingConnection();

        long size;

        auto sizeResult = socket->read(reinterpret_cast<char*>(&size), 4);

        if(sizeResult != 4)
            throw std::runtime_error("funcking wrong");
    }

signals:
    void finished();

private:
    QTcpServer serverSocket;
};

#endif // TASKS_HPP


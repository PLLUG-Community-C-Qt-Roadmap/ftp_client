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
        connect(&client, &QTcpSocket::disconnected, this, &ClientTask::finished);
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
        std::ifstream readStream("test.jpg", std::ios::binary);
        readStream.seekg(0, readStream.end);
        long size = readStream.tellg();
        readStream.seekg(0, readStream.beg);

        char *ptr = reinterpret_cast<char*>(&size);
        qDebug() << "File sent" << client.write(ptr, 4);
        client.waitForBytesWritten(300);



        std::vector<char> data(size);

        readStream.read(&data[0], size);

        size_t written = 0;

        while( written < size )
        {
            size_t temp = client.write(&data[written], std::min(numberOfBytes, data.size() - written));

            qDebug() << temp;
            written += temp;
        }

        qDebug() << client.flush();
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
        connect(&serverSocket, &QTcpServer::newConnection, this, &ServerTask::recieveFile);
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

        socket->waitForReadyRead(300);

        std::vector<char> data(size);

        std::ofstream writeStream("output.jpg", std::ios::binary);

        size_t read = 0;

        while(read < size)
        {
            if(socket->bytesAvailable())
            {
                size_t temp = socket->read(&data[read], numberOfBytes);
                read += temp;
            }
        }

        writeStream.write(&data[0], data.size());
        qDebug() << "File received";
    }

signals:
    void finished();

private:
    QTcpServer serverSocket;
};

#endif // TASKS_HPP


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
        client.setSocketOption(QAbstractSocket::LowDelayOption, 1); // don't need this, but can be usefull
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
        qDebug() << "File sent" << client.write(ptr, 4); // send size
        client.waitForBytesWritten(300);

        std::vector<char> data(size); // buffer for file, not really neaded, can be readed directly from stream

        readStream.read(&data[0], size);

        size_t written = 0;

        while( written < size )
        {
            // write data to stream
            size_t temp = client.write(&data[written], std::min(numberOfBytes, data.size() - written));

            qDebug() << temp;
            written += temp;
        }
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

        data.resize(size);

        // when the data is ready perform operation
        connect(socket, &QTcpSocket::readyRead, this, [=] {

            static size_t read = 0;
            qDebug() << socket->bytesAvailable();
            // read as much data as there is available
            size_t temp = socket->read(&data[read], data.size());
            read += temp;

            // if all data is available, write it to file.
            if(read >= size)
            {
                qDebug() << "File received";
                std::ofstream writeStream("output.jpg", std::ios::binary);
                writeStream.write(&data[0], data.size());
                emit finished();
            }
        });
    }

signals:
    void finished();

private:
    QTcpServer serverSocket;
    std::vector<char> data;
};

#endif // TASKS_HPP


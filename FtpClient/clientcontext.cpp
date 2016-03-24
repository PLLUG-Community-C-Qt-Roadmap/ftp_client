#include "clientcontext.h"

#include <QTcpSocket>
#include <fstream>
#include <QFile>
#include <QMessageBox>
#include <stdexcept>

const int portNumber = 1488;

ClientContext::ClientContext(QObject *parent): QObject(parent)
{
    mSocket = nullptr;
}

void ClientContext::doConnect()
{
    if(!mSocket)
    {
        mSocket = new QTcpSocket;
        connect(mSocket, &QAbstractSocket::connected,
                this, &ClientContext::slotConnected);
        connect(mSocket, static_cast<void(QTcpSocket::*)
                (QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                this, &ClientContext::slotError);
    }

    qDebug() << "connecting...";

    mSocket->connectToHost("localhost", portNumber);
}

std::vector<ModelEntity> ClientContext::changeDir(QString path)
{
    sendPath(path);

    mSocket->waitForReadyRead();
    QByteArray receivedData = mSocket->readAll();
    Packet pack(receivedData);

    if(pack.getErrorCode())
    {
        throw std::runtime_error(pack.getData().toStdString());
    }
    else
    {
        int quantity = pack.getAdditionalOptions(); // Get the number of packets
        std::vector<Packet> packets;
        for(int i = 0; i < quantity; ++i)
        {
            mSocket->waitForReadyRead();
            QByteArray packetReceived = mSocket->readAll();
            packets.push_back(Packet(packetReceived));
        }
        return getFilesVector(packets);
    }
}

void ClientContext::downloadFile()
{
    std::string buffer;

    buffer.resize(2);

    *reinterpret_cast<short*>(&buffer[0]) = 1;

    buffer += "Download file plz";

    mSocket->write( buffer.c_str(), buffer.size() );
}

void ClientContext::slotConnected()
{
    qDebug() << "connected...";
    emit signalConnected();
}

void ClientContext::slotError(QAbstractSocket::SocketError err)
{
    switch(err)
    {
    case QAbstractSocket::HostNotFoundError :
        qDebug() << "Error: The host was not found.";
        break;
    case QAbstractSocket::RemoteHostClosedError :
        qDebug() << "Error: The remote host is closed.";
        break;
    case QAbstractSocket::ConnectionRefusedError :
        qDebug() << "Error: The connection was refused.";
        break;
    default:
        qDebug() << "The following error occured: " << mSocket->errorString();
    }

    emit signalDisconnected();
}

void ClientContext::onUploadFile()
{
    QFile readStream("data.png");

    if( !readStream.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "Can`t open file";
        return;
    }

    qDebug() << readStream.size();

    QByteArray data( 2, '0' );

    *reinterpret_cast<short*>( data.data() ) = 2;

    data.append( readStream.readAll() );

    mSocket->write( data.data(), data.size() );
}

std::vector<ModelEntity> ClientContext::getFilesVector(const QString &files)
{

    // We receive a vector with sets of files. A set contains 4 prorepties : name, date modified, type, sizes
    std::vector<ModelEntity> filesVector;
    QStringList list  = files.split('\n');
    list.pop_back(); // We need to delete the last element, besause it's always empty and useless

    if(list.size()%4 == 0) // Check whether the list is valid.
    {
        for(auto it = list.begin(); it != list.end(); it += 4)
        {
            ModelEntity item(QStringList{*it, *(it + 1), *(it + 2), *(it + 3)});
            filesVector.push_back(item);
        }
        return filesVector;
    }
    else
    {
        throw std::runtime_error("Invalid list of files received.");
    }
}

std::vector<ModelEntity> ClientContext::getFilesVector(const std::vector<Packet> &packets)
{
    std::vector<ModelEntity> filesVector;
    for(const Packet &p: packets)
    {
        std::vector<ModelEntity> vec = getFilesVector(p.getData());
        filesVector.insert(filesVector.end(), vec.begin(), vec.end());
    }
    return filesVector;
}

void ClientContext::sendPath(const QString &path)
{
    Packet pack(0, 0, 0, path);
    std::string buffer = pack.toStdString();
    mSocket->write(buffer.c_str(), buffer.size());
}

ClientContext::~ClientContext()
{
    delete mSocket;
}

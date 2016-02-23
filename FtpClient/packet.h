#ifndef PACKET_H
#define PACKET_H

#include<QString>
#include<QByteArray>
#include<string>

class Packet
{
public:
    Packet();
    Packet(const std::string &packet);
    Packet(const QByteArray &packet);
    Packet(const short &oc, const short &ec, const int &ad, const QString &dt);
    Packet(const short &oc, const short &ec, const int &ad, const std::string &dt);

    Packet(const Packet& pack);

    bool isValid() const;

    QString toQString() const;
    std::string toStdString() const;

    short getOperationCode() const;
    short getErrorCode() const;
    int getAdditionalOptions() const;
    QString getData() const;

private:
    short operationCode;
    short errorCode;
    int additionalOptions;
    QString data;
};

#endif // PACKET_H

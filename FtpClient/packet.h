#ifndef PACKET_H
#define PACKET_H

#include <QString>
#include <QByteArray>
#include <string>
#include <stdexcept>

class Packet
{
public:
    Packet();
    Packet(const std::string &packet);
    Packet(const QByteArray &packet);
    Packet(const short oc, const short ec, const int ad, const QString &dt);
    Packet(const short oc, const short ec, const int ad, const std::string &dt);

    Packet(const Packet& pack);

    QString toQString() const;
    std::string toStdString() const;

    short getOperationCode() const;
    short getErrorCode() const;
    int getAdditionalOptions() const;
    QString getData() const;

private:
    bool isValid() const;
    void parse(const char *data, const size_t length);

private:
    short mOperationCode;
    short mErrorCode;
    int mAdditionalOptions;
    QString mData;
};

#endif // PACKET_H

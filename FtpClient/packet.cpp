#include "packet.h"

Packet::Packet():
    operationCode(),
    errorCode(),
    additionalOptions(),
    data()
{
}

Packet::Packet(const std::string &packet)
{
    if(packet.length() >= 7 && packet.length() <= 1030)
    {
        operationCode = static_cast<const short>(packet[0]);
        errorCode = static_cast<const short>(packet[1]);
        int *intPtr = const_cast<int *>(reinterpret_cast<const int *>(packet.data()+2));
        additionalOptions = intPtr[0];
        data = QString(packet.data() + 6);
    }
}

Packet::Packet(const QByteArray &packet)
{
    if(packet.length() >= 7 && packet.length() <= 1030)
    {
        operationCode = static_cast<short>(packet[0]);
        errorCode = static_cast<short>(packet[1]);
        int *intPtr = const_cast<int *>(reinterpret_cast<const int *>(packet.data()+2));
        additionalOptions = intPtr[0];
        data = QString(packet.data() + 6);
    }
}

Packet::Packet(const short &oc, const short &ec, const int &ad, const QString &dt):
    operationCode(oc),
    errorCode(ec),
    additionalOptions(ad),
    data(dt)
{
}

Packet::Packet(const short &oc, const short &ec, const int &ad, const std::string &dt):
    operationCode(oc),
    errorCode(ec),
    additionalOptions(ad),
    data(QString::fromStdString(dt))
{
}

Packet::Packet(const Packet &pack):
    operationCode(pack.operationCode),
    errorCode(pack.errorCode),
    additionalOptions(pack.additionalOptions),
    data(pack.data)
{
}

bool Packet::isValid() const
{
    return (operationCode >= 0 && operationCode <= 2) &&
           (errorCode >= 0 && errorCode <= 3);
}

QString Packet::toQString() const
{
    return QString::fromStdString(toStdString());
}

std::string Packet::toStdString() const
{
    std::string str;
    str.resize(6);

    str[0] = static_cast<char>(operationCode);
    str[1] = static_cast<char>(errorCode);
    int *intPtr = reinterpret_cast<int *>(&str[2]);
    intPtr[0]=additionalOptions;
    str += data.toStdString();

    return str;
}

short Packet::getOperationCode() const
{
    return operationCode;
}

short Packet::getErrorCode() const
{
    return errorCode;
}

int Packet::getAdditionalOptions() const
{
    return additionalOptions;
}

QString Packet::getData() const
{
    return data;
}

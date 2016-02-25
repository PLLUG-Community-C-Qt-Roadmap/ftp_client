#include "packet.h"

Packet::Packet():
    mOperationCode(),
    mErrorCode(),
    mAdditionalOptions(),
    mData()
{
}

Packet::Packet(const std::string &packet)
{
   parse(&packet[0], packet.length());
   if(!isValid())
   {
       throw std::invalid_argument("Error: Unable to create a packet out of these data.");
   }
}

Packet::Packet(const QByteArray &packet)
{
    parse(packet.data(),packet.length());
    if(!isValid())
    {
        throw std::invalid_argument("Error: Unable to create a packet out of these data.");
    }
}

Packet::Packet(const short oc, const short ec, const int ad, const QString &dt):
    mOperationCode(oc),
    mErrorCode(ec),
    mAdditionalOptions(ad),
    mData(dt)
{
    if(!isValid())
    {
        throw std::invalid_argument("Error: Unable to create a packet out of these data.");
    }
}

Packet::Packet(const short oc, const short ec, const int ad, const std::string &dt):
    mOperationCode(oc),
    mErrorCode(ec),
    mAdditionalOptions(ad),
    mData(QString::fromStdString(dt))
{
    if(!isValid())
    {
        throw std::invalid_argument("Error: Unable to create a packet out of these data.");
    }
}

Packet::Packet(const Packet &pack):
    mOperationCode(pack.mOperationCode),
    mErrorCode(pack.mErrorCode),
    mAdditionalOptions(pack.mAdditionalOptions),
    mData(pack.mData)
{
}

QString Packet::toQString() const

{
    return QString::fromStdString(toStdString());
}

std::string Packet::toStdString() const
{
    std::string str;
    str.resize(6);

    str[0] = static_cast<char>(mOperationCode);
    str[1] = static_cast<char>(mErrorCode);
    int *intPtr = reinterpret_cast<int *>(&str[2]);
    intPtr[0] = mAdditionalOptions;
    str += mData.toStdString();

    return str;
}

short Packet::getOperationCode() const
{
    return mOperationCode;
}

short Packet::getErrorCode() const
{
    return mErrorCode;
}

int Packet::getAdditionalOptions() const
{
    return mAdditionalOptions;
}

QString Packet::getData() const
{
    return mData;
}

bool Packet::isValid() const
{
    return (mOperationCode >= 0 && mOperationCode <= 2) &&
            (mErrorCode >= 0 && mErrorCode <= 3);
}

void Packet::parse(const char *data, const size_t length)
{
    if(length >= 7 && length <= 1030)
    {
        mOperationCode = static_cast<short>(data[0]);
        mErrorCode = static_cast<short>(data[1]);
        int *intPtr = const_cast<int *>(reinterpret_cast<const int *>(&data[2]));
        mAdditionalOptions = intPtr[0]; // system stops working
        mData = QString(data+6);
    }
    else
    {
        throw std::invalid_argument("Error: Unable to create a packet out of these data.");
    }
}

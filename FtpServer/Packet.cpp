#include "Packet.h"


Packet::Packet() :
	operationCode(),
	errorCode(),
	additionalOptions(),
	data()
{
}

Packet::Packet(const std::vector<char> &packet)
{
	if (packet.size() >= 7 && packet.size() <= 1030)
	{
		operationCode = static_cast<const short>(packet[0]);
		errorCode = static_cast<const short>(packet[1]);
		int *intPtr = const_cast<int *>(reinterpret_cast<const int *>(packet.data() + 2));
		additionalOptions = intPtr[0];
		data = std::string(packet.data() + 6);
	}
}

Packet::Packet(const short & oc, const std::vector<char>& packet):
	operationCode(oc)
{
	if (packet.size() >= 7 && packet.size() <= 1030)
	{
		errorCode = static_cast<const short>(packet[0]);
		int *intPtr = const_cast<int *>(reinterpret_cast<const int *>(packet.data() + 1));
		additionalOptions = intPtr[0];
		data = std::string(packet.data() + 5);
	}
}

Packet::Packet(const short &oc, const short &ec, const int &ad, const std::string &dt) :
	operationCode(oc),
	errorCode(ec),
	additionalOptions(ad),
	data(dt)
{
}

Packet::Packet(const Packet &pack) :
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

std::string Packet::toString() const
{
	std::string str;
	str.resize(6);

	str[0] = static_cast<char>(operationCode);
	str[1] = static_cast<char>(errorCode);
	int *intPtr = reinterpret_cast<int *>(&str[2]);
	intPtr[0] = additionalOptions;
	str += data;

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

std::string Packet::getData() const
{
	return data;
}
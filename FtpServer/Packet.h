#pragma once
#include<string>
#include<vector>

class Packet
{
public:
	Packet();
	Packet(const std::vector<char> &packet);
	Packet(const short &oc, const std::vector<char> &packet); 
	Packet(const short &oc, const short &ec, const int &ad, const std::string &dt);

	Packet(const Packet& pack);

	bool isValid() const;

	std::string toString() const;

	short getOperationCode() const;
	short getErrorCode() const;
	int getAdditionalOptions() const;
	std::string getData() const;

private:
	short operationCode;
	short errorCode;
	int additionalOptions;
	std::string data;
};

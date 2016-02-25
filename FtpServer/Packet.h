#ifndef PACKET_H
#define PACKET_H

#include<string>
#include<vector>
#include<stdexcept>

class Packet
{
public:
	Packet();
	Packet(const std::vector<char> &packet);
	Packet(const short oc, const std::vector<char> &packet); 
	Packet(const short oc, const short ec, const int ad, const std::string &dt);

	Packet(const Packet& pack);

	std::string toString() const;

	short getOperationCode() const;
	short getErrorCode() const;
	int getAdditionalOptions() const;
	std::string getData() const;

private:
	bool isValid() const;
	void parse(const char *data, const size_t length);
	void parse(const short oc, const char *data, const size_t length);

private:
	short mOperationCode;
	short mErrorCode;
	int mAdditionalOptions;
	std::string mData;
};
#endif // PACKET_H
// FTP-Server

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include "Packet.h"

using boost::asio::ip::tcp;
using namespace boost::filesystem;

const int max_length = 1030;

std::string getDateModified(const boost::filesystem::directory_entry &p)
{
	std::time_t date = last_write_time(p.path());

	return asctime(gmtime(&date));
}

std::string getFileInfo(const boost::filesystem::directory_entry &p)
{
	std::string fileInfo;

	fileInfo += p.path().filename().string(); //name
	fileInfo += "\n";

	fileInfo += getDateModified(p); // date modified

	if (is_directory(p))
	{
		fileInfo += "Folder\n"; //type
		fileInfo += "\n"; //size should not be stated, so we leave the empty space
	}
	else
	{
		fileInfo += "File\n"; //type
		fileInfo += boost::lexical_cast<std::string>(file_size(p.path())); //size
		fileInfo += '\n';
	}

	return fileInfo;
}

struct Executor
{
	static void ChangeDir(tcp::socket &sock)
	{
		std::cout << "Dispathed change dir" << std::endl;

		boost::system::error_code error;
		std::vector<char> data(max_length - 1);

		size_t length = sock.read_some(boost::asio::buffer(data), error);
		data[length] = '\0';
		
		Packet pack(0, data);

		if (pack.getErrorCode())
		{
			Packet toSend(1, 1, 0, "Error: Server could not work properly.\n");
			sock.write_some(boost::asio::buffer(toSend.toString()), error);

			throw std::runtime_error("Client error (" + pack.getData() + ")");
		}
		else
		{
			path dirPath(pack.getData());

			if (exists(dirPath) && is_directory(dirPath))
			{
				std::cout << "\nIt's a directory that contains:\n";

				std::string dirContent;
				for (directory_entry& file : directory_iterator(dirPath))
				{
					dirContent += getFileInfo(file);

					path filePath = file.path();
					std::cout << "    " << filePath.native() << '\n';
				}
				Packet toSend(0, 0, 0, dirContent);
				sock.write_some(boost::asio::buffer(toSend.toString()), error);
			}
			else
			{
				Packet toSend(1, 1, 0, "Error: Non-existing path\n");
				sock.write_some(boost::asio::buffer(toSend.toString()), error);
				
				throw std::runtime_error("Non-existing path.");
			}			
		}
	}

	static void DownloadFile(tcp::socket &sock)
	{
		boost::system::error_code error;
		std::vector<char> data(max_length);
		size_t length = sock.read_some(boost::asio::buffer(data), error);
		data[length] = '\0';

		std::cout << "Dispathed download file" << std::endl;
		std::cout << &data[0] << std::endl;
	}

	static void UploadFile(tcp::socket &sock)
	{
		boost::system::error_code error;
		std::vector<char> data(6000);
		size_t length = sock.read_some(boost::asio::buffer(data), error);

		std::cout << "Dispathed upload file" << std::endl;

		std::ofstream file("myImg.png", std::ios::binary);

		file.write(&data[0], length);
	}
};

struct Dispather
{
	Dispather(tcp::socket &socket) : _socket(socket)
	{
	}
	void Dispatch(short op)
	{
		if (op < 0 || op > 2)
		{
			std::cout << "Unknown operation" << std::endl;
			return;
		}

		_operations[static_cast<MessageT>(op)](_socket);
	}

	enum class MessageT { ChangeDir=0, DownloadFile, UploadFile };
	typedef std::function<void(tcp::socket&)> Action;
	static std::map < MessageT, Action > _operations;

	tcp::socket &_socket;
};

std::map<Dispather::MessageT, Dispather::Action> Dispather::_operations { 
	{ MessageT::ChangeDir, Executor::ChangeDir },
	{ MessageT::DownloadFile, Executor::DownloadFile },
	{ MessageT::UploadFile, Executor::UploadFile }
};

void session(tcp::socket sock)
{
	try
	{
		auto dispatcher = std::make_unique<Dispather>(sock);
		for (;;)
		{
			char data[1];

			boost::system::error_code error;
			size_t length = sock.read_some(boost::asio::buffer(data), error);
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			
			short op = static_cast<short>(data[0]);
			dispatcher->Dispatch(op);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		tcp::socket sock(io_service);
		a.accept(sock);
		std::thread(session, std::move(sock)).detach();
	}
}

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;
		server(io_service, 1488);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	system("pause");
	return 0;
}
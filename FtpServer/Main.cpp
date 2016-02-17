// FTP-Server

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

using boost::asio::ip::tcp;
using namespace boost::filesystem;

const int max_length = 1024;

struct Executor
{
	static void ChangeDir(tcp::socket &sock)
	{
		boost::system::error_code error;
		std::vector<char> data(max_length);

		size_t length = sock.read_some(boost::asio::buffer(data), error);
		data[length] = '\0';
		
		short errorCode = static_cast<short>(data[0]);

		int *intPtr = reinterpret_cast<int *>(&data[1]);
		int additionalOptions = intPtr[0];

		std::cout << "Dispathed change dir" << std::endl;
		std::cout << "\nError code " << errorCode << "\nAdditional Options " << additionalOptions << std::endl;

		std::string dataToSend;
		dataToSend.resize(6);

		if (errorCode) // If there's an error
		{
			std::string errorMessage(data.begin() + 5, data.begin() + length);
			std::clog << errorMessage;

			dataToSend[0] = static_cast<char>(1); // Operation code (1 byte)
			dataToSend[1] = static_cast<char>(1); // Error code (1 byte)
			*reinterpret_cast<int *>(&dataToSend[2]) = 0; // Additional options (4 bytes)
			dataToSend += "An error occured\n"; // Data (1024 bytes)

			std::cout << "An error occured\n";
		}
		else
		{
			std::string strPath(data.begin() + 5, data.begin() + length);
			path dirPath(strPath);

			if (exists(dirPath) && is_directory(dirPath))
			{
				std::cout << "\nIt's a directory that contains:\n";

				dataToSend[0] = static_cast<char>(0); // Operation code (1 byte)
				dataToSend[1] = static_cast<char>(0); // Error code (1 byte)
				*reinterpret_cast<int *>(&dataToSend[2]) = 0; // Additional options (4 bytes)

				dataToSend += "It's a directory that contains:\n"; // Data (1024 bytes)
				for (directory_entry& file : directory_iterator(dirPath))
				{
					dataToSend += file.path().string();
					dataToSend += "\n";

					path filePath = file.path();
					std::cout << "    " << filePath.native() << '\n';
				}
			}
			else
			{
				dataToSend[0] = static_cast<char>(1); // Operation code (1 byte)
				dataToSend[1] = static_cast<char>(1); // Error code (1 byte)
				*reinterpret_cast<int *>(&dataToSend[2]) = 0; // Additional options (4 bytes)
				dataToSend += "Error path\n"; // Data (1024 bytes)

				std::cout << "Error path \n";
			}

			sock.write_some(boost::asio::buffer(dataToSend), error);
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

			
			//short op = *reinterpret_cast<short*>(&data[0]);
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
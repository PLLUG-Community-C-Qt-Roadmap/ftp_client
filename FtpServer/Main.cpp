// FTP-Server

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
using namespace boost::filesystem;
using boost::asio::ip::tcp;

const int max_length = 1024;

struct Executor
{
	static void ChangeDir(tcp::socket &sock)
	{
		boost::system::error_code error;
		std::vector<char> data(max_length);
		size_t length = sock.read_some(boost::asio::buffer(data), error);
		data[length] = '\0';

		std::cout << "Dispathed change dir" << std::endl;
		std::cout << &data[0] << std::endl;
	}

	static void DownloadFile(tcp::socket &sock)
	{
		boost::system::error_code error;
		std::vector<char> data(max_length);
		size_t length = sock.read_some(boost::asio::buffer(data), error);
		
		data[length] = '\0';

		short errorCode = static_cast<short>(data[0]);

		int* pointer = reinterpret_cast<int*>(&data[1]);
		int addData = *pointer;
		
		std::string filePath(data.begin() + 5, data.begin() + length);
		int size = 0;
		path Path(filePath);
		if (exists(Path))    // does path p actually exist?
		{
			if (is_regular_file(Path))        // is path p a regular file?
			{
				size = file_size(Path); std::cout << file_size(Path) << '\n'; std::cout << Path << std::endl;
			}
			else if (is_directory(Path))      // is path p a directory?
				std::cout << Path << " is a directory\n";
			else
				std::cout << Path << " exists, but is not a regular file or directory\n";
		}
		else
			std::cout << Path << " does not exist\n";
		ifstream file(filePath, std::ios::binary);
		std::vector<char> array1;
		array1.resize(1030);
		array1[0] = static_cast<char>(3);
		array1[1] = static_cast<char>(0);
		int* newArrPointer = reinterpret_cast<int*> (&array1[2]);
		*newArrPointer = size;
		
		int count = 0;
		
		while (size > 0)
		{
			file.read(&array1[6], 1024);
			size_t l = sock.write_some(boost::asio::buffer(array1, array1.size()));
			size -= max_length;
		}
	//	std::cout << "Dispathed download file" << std::endl;
	//	std::cout << errorCode << " " << addData << std::endl;
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

	enum class MessageT { ChangeDir = 0, DownloadFile = 1, UploadFile = 2 };
	typedef std::function<void(tcp::socket&)> Action;
	static std::map < MessageT, Action > _operations;

	tcp::socket &_socket;
};

std::map<Dispather::MessageT, Dispather::Action> Dispather::_operations{
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


//	short op = *reinterpret_cast<short*>(data);
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
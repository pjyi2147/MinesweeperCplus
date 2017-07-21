#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost\array.hpp>

#include "json.hpp"
#include "minesweeper.h"

using boost::asio::ip::tcp;
using namespace std;
using json = nlohmann::json;

// code from the link
// string split
template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

// isdigits
bool isdigits1(const string s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

/*
// the script parser
void scriptParser(string* command, int* col, int* row, MineSweeper* m)
{
	int c, r;
	cout << "Input command separated by space [command col row]: ";
	string str;
	getline(cin, str);
	if (tokens.size() != 3)
	{
		cout << "Error: too many arguments" << endl;
		// readScript(command, col, row, m);
		return;
	}
	if (tokens[0] != "E" && tokens[0] != "F" && tokens[0] != "D")
	{
		cout << "Error: unknown command, try again" << endl;
		// readScript(command, col, row, m);
		return;
	}
	if (!isdigits1(tokens[1]) || !isdigits1(tokens[2]))
	{
		cout << "Error: Row or Col is not numbers. Try again." << endl;
		// readScript(command, col, row, m);
		return;
	}
	if (stoi(tokens[1]) >= m->returnCol() || stoi(tokens[1]) < 0 ||
		stoi(tokens[2]) >= m->returnRow() || stoi(tokens[2]) < 0)
	{
		cout << "Error: Row or Col out of range. Try again." << endl;
		// readScript(command, col, row, m);
		return;
	}
	
}
*/

void scriptExecutor(string line, MineSweeper* m)
{
	vector<string> tokens;
	tokens = split(line, ' ');
	
	string command = tokens[0];
	int col = stoi(tokens[1]);
	int row = stoi(tokens[2]);
	
	if (command == "E")
	{
		m->revealTile(col, row);
	}
	if (command == "F")
	{
		m->setFlag(col, row);
	}
	if (command == "D")
	{
		m->revealDoubleClick(col, row);
	}
}

string infoMinefield(MineSweeper* m)
{
	json infoMinefieldJson;
	infoMinefieldJson["win"] = m->returnWin();
	infoMinefieldJson["GameEnd"] = m->returnGameEnd();
	infoMinefieldJson["mineField"] = m->returnMineState();

	return infoMinefieldJson.dump();
}

void server()
{
	cout << "Server boot up on port 1234" << endl;
	try
	{
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

		tcp::socket socket(io_service);
		acceptor.accept(socket);
		
		cout << "Client connected" << endl;

		boost::array<char, 1000> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		std::cout.write(buf.data(), len);

		std::string message = "Test";
		boost::system::error_code ignored_error;
		boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void startGamefunction(int& order)
{
	cout << "Server boot up on port 1234" << endl;
	try
	{
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

		tcp::socket socket(io_service);
		acceptor.accept(socket);

		cout << "Client connected" << endl;

		// read
		boost::array<char, 1000> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);
		auto order_str = string(buf.begin(), buf.end());

		order = stoi(order_str);

		cout << "Message read:" << order << endl;
		
		// write
		cout << "writing messages..." << endl;
		json message_json;
		switch (order)
		{
		case 1:
			message_json["col"] = 9;
			message_json["row"] = 9;
			message_json["mineNum"] = 10;
			break;
		case 2:
			message_json["col"] = 16;
			message_json["row"] = 16;
			message_json["mineNum"] = 40;
			break;
		case 3:
			message_json["col"] = 30;
			message_json["row"] = 16;
			message_json["mineNum"] = 99;
			break;
		}
		auto message = message_json.dump();
		boost::system::error_code ignored_error;
		boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
	}

	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void firstScriptTransfer(int& col, int& row, string& command, MineSweeper* m)
{
	cout << "Server boot up on port 1234" << endl;
	try
	{
		// boot up
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

		tcp::socket socket(io_service);
		acceptor.accept(socket);

		cout << "Client connected" << endl;

		// read
		boost::array<char, 1000> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);
		auto order_str = string(buf.begin(), buf.begin() + len);
		auto json_rec = json::parse(order_str);
		
		col = json_rec["col"];
		row = json_rec["row"];
		command = json_rec["command"];

		// execute

		m->createMinefield(col, row);

		// write
		cout << "writing messages..." << endl;
		
		auto message = infoMinefield(m);
		boost::system::error_code ignored_error;
		boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
	}

	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void ingameTransfer(json* orders, MineSweeper *m)
{
	cout << "Server boot up on port 1234" << endl;
	try
	{
		// boot up
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

		tcp::socket socket(io_service);
		acceptor.accept(socket);

		cout << "Client connected" << endl;

		// read
		boost::array<char, 2000> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);
		auto order_str = string(buf.begin(), buf.begin() + len);
		*orders = json::parse(order_str);

		cout << "Message received: " << order_str << endl;
		vector<string> order = (*orders)["orders"];

		for (auto& i : order)
		{
			scriptExecutor(i, m);
		}

		// write
		cout << "writing messages..." << endl;

		auto message = infoMinefield(m);
		boost::system::error_code ignored_error;
		boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
	}

	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
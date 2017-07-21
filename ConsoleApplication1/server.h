#pragma once

#include <vector>
#include "json.hpp"

template<typename Out>
void split(const std::string &s, char delim, Out result);
std::vector<std::string> split(const std::string &s, char delim);
bool isdigits1(const string s);
// void scriptParser(string* command, int* col, int* row, MineSweeper* m);
void scriptExecutor(string command, int col, int row, MineSweeper* m);
string infoMinefield(MineSweeper* m);

void server();
void startGamefunction(int& order);
void loopServer(std::vector<string> &command_rec, string& to_send);
void firstScriptTransfer(int& col, int& row, string& command, MineSweeper* m);
void ingameTransfer(nlohmann::json* orders, MineSweeper *m);
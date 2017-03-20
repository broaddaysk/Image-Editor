#pragma once

#include "command.h"
#include <string>

class InputParser { //parses input str into cmd/arg for Command::doCommand()
public:
	static InputParser* Instance();
	void parse(const string &consoleInput, string &cmd, string &arg); //overload parse(string filePath)?

protected:
	InputParser();

private:
	static InputParser* sInstance;
};
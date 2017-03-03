#include "inputParser.h"

inputParser* inputParser::instance = 0;

inputParser* inputParser::createInstance() {
	if (instance == 0) {
		instance = new inputParser;
	}
	return instance;
}

inputParser::inputParser() {
	commandPtr = Command::createInstance();
}

void inputParser::parse(string consoleInput) {
	size_t len = consoleInput.find("(");
	string cmd = consoleInput.substr(0, len);
	string arg = consoleInput.substr(len + 1, consoleInput.find(")") - len - 1);
	commandPtr->doCommand(cmd, arg);
}
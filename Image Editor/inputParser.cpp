#include "inputParser.h"

InputParser* InputParser::sInstance = 0;

InputParser* InputParser::Instance() {
	if (sInstance == 0) {
		sInstance = new InputParser;
	}
	return sInstance;
}

InputParser::InputParser() {}

void InputParser::parse(const string &consoleInput, string &cmd, string &arg) {
	size_t len = consoleInput.find("(");
	cmd = consoleInput.substr(0, len);
	arg = consoleInput.substr(len + 1, consoleInput.find(")") - len - 1);
}
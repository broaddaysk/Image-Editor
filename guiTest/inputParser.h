#pragma once

#include "command.h"
#include <string>

class inputParser {
public:
	static inputParser* createInstance();
	void parse(string consoleInput);

protected:
	inputParser();

private:
	static inputParser* instance;
	Command* commandPtr;
};
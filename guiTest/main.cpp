//FLTK
#include <FL/Fl.H>



#include <iostream>
#include <string>

#include <fstream>

#include "image.h"
#include "editor.h"
#include "imageStack.h"
#include "command.h"
#include "inputParser.h"

using namespace std;

int main() {
	inputParser* inputPtr = inputParser::createInstance();
	
	ifstream file("C:/Users/broaddaysk/Desktop/ppm/testcommands2.txt");


	string rawInput;
	while (getline(file, rawInput)) { //cin if console
		inputPtr->parse(rawInput);
	}




	return 0;
}


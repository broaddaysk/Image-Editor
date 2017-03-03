#include "command.h"
#include <iostream>

Command* Command::instance = 0;

Command* Command::createInstance() {
	if (instance == 0) {
		instance = new Command;
	}
	return instance;
}

Command::Command() {
	imageStackPtr = ImageStack::createInstance();
	editorPtr = Editor::createInstance();
}

void Command::addImage(string filePath) {
	Image* temp = new Image(filePath);
	imageStackPtr->addImagePtr(temp);
}

void Command::selectImage(string fileName) {
	currentImagePtr = imageStackPtr->getImagePtr(fileName);
}

void Command::blur(int rad) {
	editorPtr->blur(currentImagePtr, rad);
}

void Command::edgeDetect() {
	editorPtr->edgeDetect(currentImagePtr);
}

void Command::undo() {
	editorPtr->undo(imageStackPtr);
	string fileName = currentImagePtr->getName();
	selectImage(fileName);
}

void Command::writeFile(string filePath) {
	currentImagePtr->writeFile(filePath);
}

bool Command::isInt(const string s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

void Command::doCommand(string cmd, string arg) { 
	int intArg;
	if (arg.length() > 0 && isInt(arg)) {
		intArg = stoi(arg);
	}

	if (cmd == "addImage") {
		addImage(arg);
	}
	else if (cmd == "selectImage") {
		selectImage(arg);
	}
	else if (cmd == "blur") {
		blur(intArg);
	}
	else if (cmd == "edgeDetect") {
		edgeDetect();
	}
	else if (cmd == "undo") {
		undo();
	}
	else if (cmd == "writeFile") {
		writeFile(arg);
	}
}
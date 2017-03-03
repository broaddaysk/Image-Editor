#pragma once

//change main.h to these methods!
#include "image.h"
#include "editor.h"
#include "imageStack.h"

#include <string>
#include <unordered_map>

class Command {
public:
	static Command* createInstance();
	void doCommand(string cmd, string arg);

protected:
	Command();

private:
	static Command* instance;
	ImageStack* imageStackPtr;
	Editor* editorPtr;
	Image* currentImagePtr;

	void addImage(string filePath);
	void selectImage(string fileName);
	void blur(int rad);
	void edgeDetect();
	void undo();
	void writeFile(string filePath);

	bool isInt(string s);
};
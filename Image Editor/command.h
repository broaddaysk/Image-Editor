#pragma once

//change main.h to these methods!
#include "image.h"
#include "imagePtrVector.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <stack>

class Command { //image editor commands
public:
	static Command* Instance();
	void doCommand(const string &cmd, const string &arg); //executes corresp cmd w/arg

	void addImage(const string &filePath); //add to imagePtrVector
	void selectImage(const string &fileName); //select from imagePtrVector

	void blur(const int rad); //blur in square w/arg "radius"
	void edgeDetect(); //outlines image with black border, set param as args?

	void undo(); //undo last edit, pulls top from undoStack and replaces equivalent in imagePtrVector

	void writeFile(const string &filePath); //creates .ppm file at filePath location

	bool isCurrentPtrNull();
	void setCurrentPtrNull();

	Image* getCurrentImagePtr();

protected:
	Command();

private:
	static Command* sInstance;
	Image* mCurrentImagePtr;

	stack<Image*> mUndoStack;

	vector<RGB> surroundingPixelRGB(Image &image, const int currentRow, const int currentCol, int rad = 1);
	void setAverage(vector<RGB> pixels, RGB &rgb);
	void setByContrast(vector<RGB> pixels, RGB &rgb);

	bool isInt(const string &s);
};
#pragma once
#include <vector>

#include "image.h"
#include "imageStack.h"

using namespace std;

class Editor
{
public:
	static Editor* createInstance();

	void blur(Image* image, int rad);
	void edgeDetect(Image* image);

	void undo(ImageStack* stack);

protected:
	Editor();

private:
	vector<RGB> surroundingPixelRGB(Image &image, int currentRow, int currentCol, int rad = 1);
	void setAverage(vector<RGB> pixels, RGB &rgb);
	void setByContrast(vector<RGB> pixels, RGB &rgb);

	static Editor* instance;
	vector<Image*> undoStack;
};
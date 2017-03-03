#pragma once

#include <string>
#include <vector>

#include "image.h"

using namespace std;

class ImageStack {
public:
	static ImageStack* createInstance();

	void addImagePtr(Image* ptr);
	Image* getImagePtr(int index);								//see definition
	Image* getImagePtr(string name);

	void replaceImagePtr(Image* newImage);

protected:
	ImageStack();

private:
	static ImageStack* instance;
	vector<Image*> imageStackVector;
};


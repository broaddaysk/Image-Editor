#include "imageStack.h"

#include <iostream>
#include <fstream>

using namespace std;


ImageStack* ImageStack::instance = 0;

ImageStack* ImageStack::createInstance() {
	if (instance == 0) {
		instance = new ImageStack;
	}
	return instance;
}

ImageStack::ImageStack() {}

void ImageStack::addImagePtr(Image* ptr) {
	bool isIn = 0;
	for (unsigned int i = 0; i < imageStackVector.size(); i++) {
		if (*ptr == *imageStackVector[i]) {
			cout << "image already in stack\n";
			isIn = 1;
			break;
		}
	}
	if (isIn == 0) {
		imageStackVector.push_back(ptr);
	}
}

Image* ImageStack::getImagePtr(int index) {									//search by index any use?
	return imageStackVector[index];
}

Image* ImageStack::getImagePtr(string name) {
	for (unsigned int i = 0; i < imageStackVector.size(); i++) {
		if (imageStackVector[i]->getName() == name) {
			return imageStackVector[i];
		}
	}
	cout << "image not found\n";
	return nullptr;
}

void ImageStack::replaceImagePtr(Image* newImage) {
	string name = newImage->getName();
	
	for (unsigned int i = 0; i < imageStackVector.size(); i++) {
		if (imageStackVector[i]->getName() == name) {
			imageStackVector[i] = newImage;
		}
	}
}

#include "imagePtrVector.h"

#include <iostream>
#include <fstream>

using namespace std;

ImagePtrVector* ImagePtrVector::sInstance = 0;

ImagePtrVector* ImagePtrVector::Instance() {
	if (sInstance == 0) {
		sInstance = new ImagePtrVector;
	}
	return sInstance;
}

ImagePtrVector::ImagePtrVector() {
	added=false;
}

bool ImagePtrVector::isDuplicate(Image* ptr) {
	for (unsigned int i = 0; i < mPtrVector.size(); i++) {
		if (*ptr == *mPtrVector[i]) {
			cout << "image already in stack" << endl;
			return true;
		}
	}
	return false;
}

void ImagePtrVector::addImagePtr(Image* ptr) {
	if (!isDuplicate(ptr)) {
		mPtrVector.push_back(ptr);
		added = true;
	}
	else {
		added = false;
	}
}

Image* ImagePtrVector::getImagePtr(const string &name) {
	for (unsigned int i = 0; i < mPtrVector.size(); i++) {
		if (mPtrVector[i]->getName() == name) {
			return mPtrVector[i];
		}
	}
	cout << "image not found" << endl;
	return nullptr;
}

void ImagePtrVector::replaceImagePtr(Image* newImage) {
	string name = newImage->getName();
	
	for (unsigned int i = 0; i < mPtrVector.size(); i++) {
		if (mPtrVector[i]->getName() == name) {
			mPtrVector[i] = newImage;
		}
	}
}

string ImagePtrVector::getLastName() {
	return mPtrVector[mPtrVector.size() - 1]->getName();
}

bool ImagePtrVector::imageAdded() {
	return added;
}
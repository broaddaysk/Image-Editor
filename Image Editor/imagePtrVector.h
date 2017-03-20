#pragma once

#include <string>
#include <vector>

#include "image.h"

using namespace std;

class ImagePtrVector { //stores unique image instances for manip and return by Command
public:
	static ImagePtrVector* Instance();

	bool isDuplicate(Image* ptr);
	void addImagePtr(Image* ptr);
	Image* getImagePtr(const string &name);

	void replaceImagePtr(Image* newImage); //for Command::undo()
	string getLastName();
	bool imageAdded(); //for check in main::addCB()

protected:
	ImagePtrVector();

private:
	static ImagePtrVector* sInstance;
	vector<Image*> mPtrVector;
	bool added;
};


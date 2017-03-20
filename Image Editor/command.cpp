#include "command.h"
#include <iostream>
#include <algorithm>
#include <iterator>

Command* Command::sInstance = 0;

Command* Command::Instance() {
	if (sInstance == 0) {
		sInstance = new Command;
	}
	return sInstance;
}

Command::Command() {
	mCurrentImagePtr = nullptr;
}

bool Command::isCurrentPtrNull() {
	if (mCurrentImagePtr == nullptr) {
		return true;
	}
	return false;
}

void Command::addImage(const string &filePath) {
	Image* temp = new Image(filePath);
	ImagePtrVector::Instance()->addImagePtr(temp);
}

void Command::selectImage(const string &fileName) {
	mCurrentImagePtr = ImagePtrVector::Instance()->getImagePtr(fileName);
}

void Command::blur(const int rad) {															//reimplement with memcpy array stack algorithm
	Image* copyPtr = new Image(mCurrentImagePtr);

	RGB** oldArray = copyPtr->getRGBArray();
	RGB** newArray = mCurrentImagePtr->getRGBArray();

	for (int i = 0; i < copyPtr->getNumRow(); i++) {
		for (int j = 0; j < copyPtr->getNumCol(); j++) {
			setAverage(surroundingPixelRGB(*copyPtr, i, j, rad), newArray[i][j]);
		}
	}
	mCurrentImagePtr->setRGBArray(newArray); //update mCurrentImagePtr's RGBArray
	mUndoStack.push(copyPtr); //add old image to undoStack
}

void Command::edgeDetect() {
	Image* copyPtr = new Image(mCurrentImagePtr);

	RGB** oldArray = copyPtr->getRGBArray();
	RGB** newArray = mCurrentImagePtr->getRGBArray();

	for (int i = 0; i < copyPtr->getNumRow(); i++) {
		for (int j = 0; j < copyPtr->getNumCol(); j++) {
			setByContrast(surroundingPixelRGB(*copyPtr, i, j), newArray[i][j]);
		}
	}
	mCurrentImagePtr->setRGBArray(newArray); //update mCurrentImagePtr's RGBArray
	mUndoStack.push(copyPtr); //add old image to undoStack
}

void Command::undo() {
	if (mUndoStack.size() == 0) {
		cout << "nothing to undo" << endl;
	}
	else {
		Image* oldImagePtr = mUndoStack.top(); //pull top from undoStack
		ImagePtrVector::Instance()->replaceImagePtr(oldImagePtr); //replace equivalent in imagePtrVector
		mUndoStack.pop();
	}
	string currentImageName = mCurrentImagePtr->getName();
	selectImage(currentImageName); //set top as new current image ptr
}

void Command::writeFile(const string &filePath) {
	mCurrentImagePtr->writeFile(filePath);
}

bool Command::isInt(const string &s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

void Command::doCommand(const string &cmd, const string &arg) { 
	if (cmd == "addImage") {
		addImage(arg);
	}
	else if (cmd == "selectImage") {
		selectImage(arg);
	}
	else if (cmd == "blur") {
		int intArg;
		if (isInt(arg) && (arg.length() > 0)) {
			intArg = stoi(arg);
		}
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
	else {
		cout << "cmd not found!" << endl;
	}
}

vector<RGB> Command::surroundingPixelRGB(Image &image, const int currentRow, const int currentCol, int rad) { //generates vector of surrounding pixel RGB vals
	int imin, imax, jmin, jmax;
	imin = 0 > (currentRow - rad) ? 0 : (currentRow - rad);
	imax = image.getNumRow() < (currentRow + rad + 1) ? image.getNumRow() : (currentRow + rad + 1);
	jmin = 0 > (currentCol - rad) ? 0 : (currentCol - rad);
	jmax = image.getNumCol() < (currentCol + rad + 1) ? image.getNumCol() : (currentCol + rad + 1);

	vector<RGB> surroundingPixels;

	for (int i = imin; i < imax; i++) {
		for (int j = jmin; j < jmax; j++) {
			if (!(i == currentRow && j == currentCol)) {
				surroundingPixels.push_back(image.getRGBArray()[i][j]);
			}
		}
	}
	return surroundingPixels;
}

void Command::setAverage(vector<RGB> pixels, RGB &rgb) {
	int size, rSum = 0, gSum = 0, bSum = 0;
	size = pixels.size();
	for (int i = 0; i < size; i++) {
		int intR = static_cast<int>(pixels[i].R);
		int intG = static_cast<int>(pixels[i].G);
		int intB = static_cast<int>(pixels[i].B);

		rSum += intR;
		gSum += intG;
		bSum += intB;
	}

	rgb.R = static_cast<unsigned char>(rSum / size);
	rgb.G = static_cast<unsigned char>(gSum / size);
	rgb.B = static_cast<unsigned char>(bSum / size);
}

void Command::setByContrast(vector<RGB> pixels, RGB &rgb) {
	int size, rDiff, gDiff, bDiff, rCount = 0, gCount = 0, bCount = 0;
	vector<int> rDiffVec, gDiffVec, bDiffVec;
	size = pixels.size();
	for (int i = 0; i < size; i++) {

		int intLR = static_cast<int>(pixels[i].R);
		int intLG = static_cast<int>(pixels[i].G);
		int intLB = static_cast<int>(pixels[i].B);
		
		int intRR = static_cast<int>(rgb.R);
		int intRG = static_cast<int>(rgb.G);
		int intRB = static_cast<int>(rgb.B);

		rDiff = intLR - intRR;
		rDiffVec.push_back(rDiff);
		gDiff = intLG - intRG;
		gDiffVec.push_back(gDiff);
		bDiff = intLB - intRB;
		bDiffVec.push_back(bDiff);
	}

	int contrastDiff = 14;														//edgeDetect parameters!!!
	int    threshold = 1;

	for (int i = 0; i < size; i++) {
		if (rDiffVec[i] > contrastDiff) {
			rCount++;
		}
		if (gDiffVec[i] > contrastDiff) {
			gCount++;
		}
		if (bDiffVec[i] > contrastDiff) {
			bCount++;
		}
	}

	if (rCount > threshold || gCount > threshold || bCount > threshold) {
		rgb.R = static_cast<unsigned char>(0);
		rgb.G = static_cast<unsigned char>(0);
		rgb.B = static_cast<unsigned char>(0);
	}

}

void Command::setCurrentPtrNull() {
	mCurrentImagePtr = nullptr;
}

Image* Command::getCurrentImagePtr() {
	return mCurrentImagePtr;
}
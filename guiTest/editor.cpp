#include <iostream>
#include <algorithm>
#include <iterator>

#include "editor.h"

using namespace std;

Editor* Editor::instance = 0;

Editor* Editor::createInstance() {
	if (instance == 0) {
		instance = new Editor;
	}
	return instance;
}

Editor::Editor() {}

vector<RGB> Editor::surroundingPixelRGB(Image &image, int currentRow, int currentCol, int rad) {
	vector<RGB> surroundingPixels;
	RGB** rgbArray = image.getRGBArray();

	vector<int> iCoord, jCoord, iRange, jRange, iValid, jValid;
	for (int i = (currentRow - rad); i <= (currentRow + rad); i++) {
		iCoord.push_back(i);
	}
	for (int i = (currentCol - rad); i <= (currentCol + rad); i++) {
		jCoord.push_back(i);
	}
	for (int i = 0; i < image.getNumRow(); i++) {
		iRange.push_back(i);
	}
	for (int i = 0; i < image.getNumCol(); i++) {
		jRange.push_back(i);
	}

	set_intersection(iCoord.begin(), iCoord.end(), iRange.begin(), iRange.end(), back_inserter(iValid));
	set_intersection(jCoord.begin(), jCoord.end(), jRange.begin(), jRange.end(), back_inserter(jValid));

	for (int i = iValid.front(); i <= iValid.back(); i++) {
		for (int j = jValid.front(); j <= jValid.back(); j++) {

			if (!(i == currentRow && j == currentCol)) {
				surroundingPixels.push_back(rgbArray[i][j]);
			}
		}
	}
	return surroundingPixels;
}

void Editor::setAverage(vector<RGB> pixels, RGB &rgb) {
	int size, rSum=0, gSum=0, bSum=0;
	size = pixels.size();
	for (int i = 0; i < size; i++) {
		rSum += pixels[i].R;
		gSum += pixels[i].G;
		bSum += pixels[i].B;
	}
	rgb.R = (rSum / size);
	rgb.G = (gSum / size);
	rgb.B = (bSum / size);
}

void Editor::setByContrast(vector<RGB> pixels, RGB &rgb) {
	int size, rDiff, gDiff, bDiff, rCount = 0, gCount = 0, bCount = 0;
	vector<int> rDiffVec, gDiffVec, bDiffVec;
	size = pixels.size();
	for (int i = 0; i < size; i++) {
		rDiff = pixels[i].R - rgb.R;
		rDiffVec.push_back(rDiff);
		gDiff = pixels[i].G - rgb.G;
		gDiffVec.push_back(gDiff);
		bDiff = pixels[i].B - rgb.B;
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
		rgb.B = 0;
		rgb.G = 0;
		rgb.R = 0;
	}
}

void Editor::blur(Image* imagePtr, int rad) { //avg surrounding blocks, modify input image and add copy to undoStack
	Image* copyPtr = new Image(imagePtr);

	RGB** oldArray = copyPtr->getRGBArray();
	RGB** newArray = imagePtr->getRGBArray();

	for (int i = 0; i < copyPtr->getNumRow(); i++) {
		for (int j = 0; j < copyPtr->getNumCol(); j++) {
			setAverage(surroundingPixelRGB(*copyPtr, i, j, rad), newArray[i][j]);
		}
	}
	imagePtr->setRGBArray(newArray);
	undoStack.push_back(copyPtr);
}

void Editor::edgeDetect(Image* imagePtr) { //something about comparing pixel to surrounding pixels
	Image* copyPtr = new Image(imagePtr);

	RGB** oldArray = copyPtr->getRGBArray();
	RGB** newArray = imagePtr->getRGBArray();

	for (int i = 0; i < copyPtr->getNumRow(); i++) {
		for (int j = 0; j < copyPtr->getNumCol(); j++) {
			setByContrast(surroundingPixelRGB(*copyPtr, i, j), newArray[i][j]);
		}
	}
	imagePtr->setRGBArray(newArray);
	undoStack.push_back(copyPtr);
}

void Editor::undo(ImageStack* stack) {
	if (undoStack.size() == 0) {
		cout << "nothing to undo\n";
	}
	else {
		Image* oldImagePtr = undoStack.back();
		stack->replaceImagePtr(oldImagePtr);
		undoStack.pop_back();
	}
}
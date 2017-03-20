#include "image.h"
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <cstring>
#include <algorithm>

using namespace std;

//#define MCOPY
//#define COPY
#define DEFAULT

istream& operator >> (istream& in, RGB& rgb) {
	in >> rgb.R;
	in >> rgb.G;
	in >> rgb.B;
	return in;
}

ostream& operator<<(ostream& out, RGB& rgb) {
	out << rgb.R << "\n"
		<< rgb.G << "\n"
		<< rgb.B << "\n";
	return out;
}

Image::Image(const string &filePath) {
	string delims = "//\\.";
	vector<string> pathComponents;
	int start = filePath.find_first_not_of(delims), end = filePath.find_first_of(delims, start);
	while (start != string::npos) {
		pathComponents.push_back(filePath.substr(start, end - start));
		start = filePath.find_first_not_of(delims, end);
		end = filePath.find_first_of(delims, start);
	}

	mImageName = pathComponents[pathComponents.size()-2]; 

	ifstream file(filePath);
	if (file.is_open()) {
		string line;
		for (int i = 0; i < 4; i++) {
			getline(file, line);
			if (i == 2) { //assuming typical PPM formatting
				istringstream(line) >> mNumCol >> mNumRow;
			}
		}

		mRGBArray = new RGB*[mNumRow];
		for (int i = 0; i < mNumRow; i++) {
			mRGBArray[i] = new RGB[mNumCol];
		}

		RGB rgb;
		int intVal;
		unsigned char chVal;

		for (int i = 0; i < mNumRow; i++) {
			for (int j = 0; j < mNumCol; j++) {
				for (int k = 0; k < 3; k++) {
					file >> intVal;
					chVal = static_cast<unsigned char>(intVal);
					switch (k) {
					case 0:rgb.R = chVal;
						break;
					case 1:rgb.G = chVal;
						break;
					default:rgb.B = chVal;
						break;
					}
				}
				mRGBArray[i][j] = rgb;
			}
		}
	}
}

Image::Image(Image* im) {
	mImageName = im->mImageName;
	mNumCol = im->mNumCol;
	mNumRow = im->mNumRow;

#ifdef MCOPY
	mRGBArray = new RGB*[mNumRow];															
	for (int i = 0; i < mNumRow; i++) {
		mRGBArray[i] = new RGB[mNumCol];
	}
	
	memcpy(mRGBArray, im->getRGBArray(), mNumCol * mNumRow * sizeof *(im->getRGBArray()));
#endif

#ifdef COPY
	/*
	mRGBArray = new RGB*[mNumRow];
	for (int i = 0; i < mNumRow; i++) {
		mRGBArray[i] = new RGB[mNumCol];
	}*/
	copy(&(im->getRGBArray())[0][0], &(im->getRGBArray())[0][0] + mNumRow*mNumCol, &mRGBArray[0][0]);
#endif

#ifdef DEFAULT
	mRGBArray = new RGB*[mNumRow];															
	for (int i = 0; i < mNumRow; i++) {
		mRGBArray[i] = new RGB[mNumCol];
	}

	for (int i = 0; i < mNumRow; i++) {
		for (int j = 0; j < mNumCol; j++) {
			mRGBArray[i][j] = im->mRGBArray[i][j];
		}
	}
#endif
}

Image::~Image() {
	for (int i = 0; i < mNumRow; i++) {
		delete[] mRGBArray[i];
	}
	delete[] mRGBArray;
}

string Image::getName() {
	return mImageName;
}

int Image::getNumCol() {
	return mNumCol;
}

int Image::getNumRow() {
	return mNumRow;
}

RGB** Image::getRGBArray() {
	return mRGBArray;
}

void Image::setRGBArray(RGB** newArray) {
	mRGBArray = newArray;
}

bool operator==(const Image& lhs, const Image& rhs) { //so no duplicate images are added to imagePtrVector
	if (lhs.mNumCol != rhs.mNumCol || lhs.mNumRow != rhs.mNumRow) {
		return false;
	}
	if ((sizeof(lhs.mRGBArray)/sizeof(lhs.mRGBArray[0])) == (sizeof(rhs.mRGBArray) / sizeof(rhs.mRGBArray[0]))) {
		for (int i = 0; i < lhs.mNumRow; i++) {
			for (int j = 0; j < lhs.mNumCol; j++) {
				if (lhs.mRGBArray[i][j].R != rhs.mRGBArray[i][j].R || lhs.mRGBArray[i][j].G != rhs.mRGBArray[i][j].G || lhs.mRGBArray[i][j].B != rhs.mRGBArray[i][j].B) {
					return false;
				}
			}
		}
	}
	return true;
}

void Image::writeFile(const string &filePath) {
	ofstream file(filePath);
	file << "P3\n"
		<< "# comment line\n"
		<< mNumCol << " " << mNumRow << "\n"
		<< 255 << "\n";

	for (int i = 0; i < mNumRow; i++) {
		for (int j = 0; j < mNumCol; j++) {
			int val;
			for (int k = 0; k < 3; k++) {
				switch (k) {
				case 0:val = static_cast<int>(mRGBArray[i][j].R);
					break;
				case 1:val = static_cast<int>(mRGBArray[i][j].G);
					break;
				default:val = static_cast<int>(mRGBArray[i][j].B);
					break;
				}
				file << val << endl;
			}
		}
	}
	file.close();
}
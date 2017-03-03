#include "image.h"
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

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

Image::Image(string strFile) {
	string delims = "/.";
	vector<string> pathComponents;
	int start = strFile.find_first_not_of(delims), end = strFile.find_first_of(delims, start);
	while (start != string::npos) {
		pathComponents.push_back(strFile.substr(start, end - start));
		start = strFile.find_first_not_of(delims, end);
		end = strFile.find_first_of(delims, start);
	}

	name = pathComponents[5];

	ifstream file(strFile);
	if (file.is_open()) {
		string line;
		for (int i = 0; i < 4; i++) {
			getline(file, line);
			if (i == 2) {
				istringstream(line) >> numCol >> numRow;
			}
		}

		rgbArray = new RGB*[numRow];
		for (int i = 0; i < numRow; i++) {
			rgbArray[i] = new RGB[numCol];
		}

		RGB rgb;
		int colIdx = 0, rowIdx = 0;
		while (file >> rgb) {
			rgbArray[rowIdx][colIdx] = rgb;

			if (colIdx == (numCol - 1)) {
				colIdx = 0;
				rowIdx++;
			}
			else {
				colIdx++;
			}
		}
	}
}

Image::Image(const Image* im) {
	name = im->name;
	numCol = im->numCol;
	numRow = im->numRow;
	
	rgbArray = new RGB*[numRow];
	for (int i = 0; i < numRow; i++) {
		rgbArray[i] = new RGB[numCol];
	}
	
	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numCol; j++) {
			rgbArray[i][j] = im->rgbArray[i][j];
		}
	}
}

Image::~Image() {
	for (int i = 0; i < numRow; i++) {
		delete[] rgbArray[i];
	}
	delete[] rgbArray;
}

string Image::getName() {
	return name;
}

int Image::getNumCol() {
	return numCol;
}

int Image::getNumRow() {
	return numRow;
}

RGB** Image::getRGBArray() {
	return rgbArray;
}

void Image::setRGBArray(RGB** newArray) {
	rgbArray = newArray;
}

bool operator==(const Image& lhs, const Image& rhs) {
	if (lhs.numCol != rhs.numCol || lhs.numRow != rhs.numRow) {
		return 0;
	}
	if ((sizeof(lhs.rgbArray)/sizeof(lhs.rgbArray[0])) == (sizeof(rhs.rgbArray) / sizeof(rhs.rgbArray[0]))) {
		for (int i = 0; i < lhs.numRow; i++) {
			for (int j = 0; j < lhs.numCol; j++) {
				if (lhs.rgbArray[i][j].R != rhs.rgbArray[i][j].R || lhs.rgbArray[i][j].G != rhs.rgbArray[i][j].G || lhs.rgbArray[i][j].B != rhs.rgbArray[i][j].B) {
					return 0;
				}
			}
		}
	}
	return 1;
}

void Image::print(int row, int col) {
	cout << rgbArray[row][col] << endl;
}

void Image::writeFile(string strFile) {
	ofstream file(strFile);
	file << "P3\n"
		<< "# comment line\n"
		<< numCol << " " << numRow << "\n"
		<< 255 << "\n";

	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numCol; j++) {
			file << rgbArray[i][j];
		}
	}
	file.close();
}
//FLTK
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Image.H>

#include <iostream>
#include <string>
#include <fstream>

#include "inputParser.h"

//#define DEBUG
#define DEFAULT

//for future reference: assert (esp for pointers and vars) used in debug, try/catch used in larger blocks
//ifdef should be def in soln>properties>preprocessor def

using namespace std;

void addCB(Fl_Widget* obj, void*) {	
	Fl_Button* button = (Fl_Button*)obj;

	Fl_Native_File_Chooser fileChooser;
	fileChooser.title("Pick a ppm image");
	fileChooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
	fileChooser.filter("PPM Files\t*.ppm");

#ifdef DEFAULT
	fileChooser.show();
#endif

#ifdef DEBUG
	switch (fileChooser.show()) {
	case -1:printf("ERROR: %s/n", fileChooser.errmsg());
	case 1:printf("CANCEL/n");
	default:printf("PICKED: %s/n", fileChooser.filename());
	}
#endif

	const char* chFilePath = fileChooser.filename();
	string strfilePath = string(chFilePath);
	if (!strfilePath.empty()) { //check if a ppm file was selected
		Command::Instance()->addImage(strfilePath);
		if (ImagePtrVector::Instance()->imageAdded()) { //check if image was actually added
			string strName = ImagePtrVector::Instance()->getLastName();
			const char* chName = strName.c_str();
			((Fl_Hold_Browser*)(button->parent()->child(8)))->add(chName); //populate browser
		}
	}
}

void browserCB(Fl_Widget* obj) {
	Fl_Hold_Browser* browser = (Fl_Hold_Browser*)obj;
	int location = browser->value();
	if (location != 0) {
		const char* option;
		option = browser->text(location);
		string file = string(option);
		Command::Instance()->selectImage(file);
	}
	else {
		Command::Instance()->setCurrentPtrNull();
	}
}

void writeCB(Fl_Widget* obj, void*) {
	if (!(Command::Instance()->isCurrentPtrNull())) { //check if image is currently selected
		Fl_Button* button = (Fl_Button*)obj;

		Fl_Native_File_Chooser fileChooser;
		fileChooser.title("Save as");
		fileChooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
		fileChooser.filter("PPM Files\t*.ppm");

		switch (fileChooser.show()) {

#ifdef DEBUG
		case -1:printf("ERROR: %s/n", fileChooser.errmsg());
		case 1:printf("CANCEL/n");
#endif
		default:
			const char* chFilePath = fileChooser.filename();
			string strFilePath = string(chFilePath);
			if (strFilePath.length() > 0) { //if save_as is not cancelled
				string delims = "\\.";
				vector<string> pathComponents;
				int start = strFilePath.find_first_not_of(delims), end = strFilePath.find_first_of(delims, start);
				while (start != string::npos) {
					pathComponents.push_back(strFilePath.substr(start, end - start));
					start = strFilePath.find_first_not_of(delims, end);
					end = strFilePath.find_first_of(delims, start);
				}
				string ext = pathComponents[pathComponents.size() - 1];
				if (ext != "ppm") {
					strFilePath += ".ppm";
				}
				Command::Instance()->writeFile(strFilePath);
			}
		}
	}
}

bool isInt(const string &s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

void blurCB(Fl_Widget* obj, void*) {
	Fl_Button* button = (Fl_Button*)obj;
	string radStr = ((Fl_Input*)(button->parent()->child(5)))->value();
	if (!(Command::Instance()->isCurrentPtrNull())) { //check if an image has been selected
		if ((radStr.length() > 0) && isInt(radStr)) {
			int rad = stoi(radStr);
			Command::Instance()->blur(rad);
		}
	}
}

void edgeCB(Fl_Widget* obj, void*) {
	if (!(Command::Instance()->isCurrentPtrNull())) {
		Command::Instance()->edgeDetect();
	}
}

void undoCB(Fl_Widget* obj, void*) {
	if (!(Command::Instance()->isCurrentPtrNull())) {
		Command::Instance()->undo();
	}
}

int main(int argc, char **argv) {
#ifdef DEBUG
	ifstream file("C:/Users/broaddaysk/Desktop/ppm/testcommands.txt"); //takes in .txt file that contains sequence of commands (with proper syntax), each separated by newlines

	string rawInput, cmd, arg;
	while (getline(file, rawInput)) { //cin if console
		InputParser::Instance()->parse(rawInput, cmd, arg);
		Command::Instance()->doCommand(cmd, arg);
	}
	return 0;
#endif
	
#ifdef DEFAULT
	Fl_Window* editWindow = new Fl_Window(600, 400);
	editWindow->begin();
		Fl_Button* addButton = new Fl_Button(20, 300, 90, 30, "Add Image");
		addButton->callback((Fl_Callback*)addCB);
		Fl_Button* writeButton = new Fl_Button(20, 350, 90, 30, "Write Image");
		writeButton->callback((Fl_Callback*)writeCB);

		Fl_Box *topRightBox = new Fl_Box(200, 20, 380, 260);
		topRightBox->box(FL_BORDER_BOX);
		Fl_Box *filterBox = new Fl_Box(320, 40, 140, 40, "FILTERS");
		filterBox->labelfont(FL_BOLD);
		filterBox->labelsize(30);

		Fl_Button* blurButton = new Fl_Button(220, 100, 90, 30, "Blur");
		blurButton->callback((Fl_Callback*)blurCB);
		Fl_Input* blurInput = new Fl_Input(330, 100, 30, 30);
		Fl_Button* edgeButton = new Fl_Button(220, 150, 90, 30, "Detect Edges");
		edgeButton->callback((Fl_Callback*)edgeCB);
		Fl_Button* undoButton = new Fl_Button(470, 230, 90, 30, "Undo");
		undoButton->callback((Fl_Callback*)undoCB);

		Fl_Browser* imageBrowser = new Fl_Hold_Browser(20, 20, 160, 260);
		imageBrowser->has_scrollbar(Fl_Browser::VERTICAL);
		imageBrowser->callback((Fl_Callback*)browserCB);
	editWindow->end();

	editWindow->show(argc, argv);
	return Fl::run();
#endif
}


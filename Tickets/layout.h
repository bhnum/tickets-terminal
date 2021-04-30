#pragma once

#include <string>
#include <vector>
#include "consolefunctions.h"

using namespace std;

// https://stackoverflow.com/questions/24708700/c-detect-when-user-presses-arrow-key
enum KeyboardKey {
	KeyEnter = '\r',
	KeyBackspace = '\b',
	KeyTab = '\t',
	KeyEscape = 27,
	KeyDelete = 83,
	KeyHome = 71,
	KeyEnd = 79,
	KeyUp = 72,
	KeyDown = 80,
	KeyLeft = 75,
	KeyRight = 77,
};

enum ElementType {
	ElementTypeText,
	ElementTypeTextBox,
	ElementTypeButton,
};

// Structure of each element. Elements can be placed in layouts for display, input or actions.
// Each element can be a text element, a textbox or a button which is understood by its type.
struct Element {
	ElementType type;
	string name;
	string text;
	string title;
	bool password = false;
	bool readonly = false;
	ConsoleColor color = BackColorWhite | ForeColorBlack;
	ConsoleColor focusedcolor = BackColorWhite | ForeColorBlack;
};

// Structure of each layout.
struct Layout {
	string caption;
	ConsoleColor color = BackColorWhite | ForeColorBlack;
	vector<Element> elements;
	int tabindex = 0; // Index of the focused element in elements excluding text elements.
	int colindex = 0; // The position of the cursor in the focused textbox of each layout.
};


// Create and open up a new layout.
void newlayout(string caption, ConsoleColor color = BackColorWhite | ForeColorBlack);

// Full redraw the layout by clearing the screan.
void showlayout();

// Fast layout redraw without clearing the screan.
void refreshlayout();

// Delete the current layout and jump to previous layout.
void closelayout();

// Empty layout elements.
void clearlayout();


// Add text to current layout.
void puttext(string text, ConsoleColor color = -1);

// Add text to current layout with can be accessed later by name.
void puttext(string name, string text, ConsoleColor color = -1);

// Add textbox to current layout whose value can be accessed by function gettextboxvalue().
void puttextbox(string name, string title, string text, bool password = false, bool readonly = false);

// Add button to layout. function waitforuserinput() return the name of the clicked button.
void putbutton(string name, string text, ConsoleColor color = BackColorBlack | ForeColorWhite, ConsoleColor focusedcolor = BackColorDarkGrey | ForeColorWhite);


// Returns the value of the element previously added by name.
Element &getelement(string name);

// Returns the value of the element previously added by name.
Layout &getlayout();

// Shows the layout and processes keyboard inputs until user clicks a button or presses the Esc key.
// Return value is the name of the clicked button or the "ESCAPE" string.
string waitforuserinput();

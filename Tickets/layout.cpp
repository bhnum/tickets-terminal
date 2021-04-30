#include "layout.h"

#include <iostream>
#include <conio.h>
#include <stack>

// Initialize a stack of layouts with at least one element.
stack<Layout> layouts({ Layout() });

// Short forms of certain widely used expressions.
#define currentlayout layouts.top()
#define currentelements layouts.top().elements

// ---------------------------------- private function implementations ----------------------------------------

bool isfocusable(Element &c)
{
	if (c.type == ElementTypeButton) return true;
	if (c.type == ElementTypeTextBox && !c.readonly) return true;
	return false;
}

int getfocusedelementindex()
{
	vector<int> focusables;

	for (int i = 0; i < currentelements.size(); i++)
	{
		Element& c = currentelements[i];
		if (isfocusable(c))
			focusables.push_back(i);
	}
	if (focusables.size() == 0)
		return 0;

	currentlayout.tabindex = (currentlayout.tabindex + focusables.size()) % focusables.size();
	return focusables[currentlayout.tabindex];
}

void gotonextelement()
{
	currentlayout.tabindex++;
	currentlayout.colindex = 0;
}

void gotopreviouselement()
{
	currentlayout.tabindex--;
	currentlayout.colindex = 0;
}

// Inputs which should be processed for the focused textbox are sent to this fuction.
void processtextbox(int ch, bool controlchar = false)
{
	Element &textbox = currentelements[getfocusedelementindex()];
	string &text = textbox.text;
	if (controlchar)
	{
		switch (ch)
		{
		case KeyDelete:
			if (currentlayout.colindex < text.size())
				text.erase(text.begin() + currentlayout.colindex);
			break;
		case KeyLeft:
			if (currentlayout.colindex > 0)
				currentlayout.colindex--;
			break;
		case KeyRight:
			if (currentlayout.colindex < text.size())
				currentlayout.colindex++;
			break;
		case KeyHome: // Go to the first character of the text.
			currentlayout.colindex = 0;
			break;
		case KeyEnd: // Go after the last character of the text.
			currentlayout.colindex = text.size();
			break;
		}
	}
	else
	{
		switch (ch)
		{
		case KeyBackspace:
			if (currentlayout.colindex > 0) {
				text.erase(text.begin() + currentlayout.colindex - 1);
				currentlayout.colindex--;
			}
			break;
		default: // Insert character in the text.
			text.insert(text.begin() + currentlayout.colindex, ch);
			currentlayout.colindex++;
			break;
		}
	}
}

// ---------------------------------- public function implementations -----------------------------------------

void newlayout(string caption, ConsoleColor color)
{
	Layout l;
	l.caption = caption;
	l.color = color;
	layouts.push(l);
}

void showlayout()
{
	clearscreen(currentlayout.color);
	refreshlayout();
}

void closelayout()
{
	layouts.pop();
	showlayout();
}

void clearlayout()
{
	currentelements.clear();
}

void refreshlayout()
{
	// Get the focused element index.
	int focus = getfocusedelementindex();

	// The coordinates of the focused element.
	short consoleinputx = 0, consoleinputy = 0;

	// The cursor should be visible if the focused element is a textbox, invisible otherwise.
	bool cursorvisible = false;

	// Hide the cursor while rendering the elements.
	showcursor(false);

	// Start drawing from the top left corner of the screen.
	setcursorposition(0, 0);

	// Draw the layout caption with red background.
	setlinecolor(BackColorRed | ForeColorWhite);
	cout << currentlayout.caption;

	for (int i = 0; i < currentelements.size(); i++)
	{
		switch (currentelements[i].type)
		{
		case ElementTypeText:
			// Draw text in the corressping color.
			setconsolecolor(currentelements[i].color);
			cout << currentelements[i].text;
			break;

		case ElementTypeTextBox:
		{
			// Draw the title of the textbox in grey with maximum width of 20 chracters.
			setconsolecolor(BackColorWhite | ForeColorDarkGrey);
			string title = currentelements[i].title;
			title.resize(30, ' ');
			cout << endl << endl << title;

			if (currentelements[i].readonly) 
			{
				// Draw the text of the textbox as regular text.
				setconsolecolor(currentlayout.color);
				cout << currentelements[i].text;
			}
			else
			{
				// Draw the text of the textbox in magenta if focused or blue otherwise
				// with maximum width of 80 chracters.
				if (i == focus)
				{
					cursorvisible = true;
					getcursorposition(consoleinputy, consoleinputx);
					setconsolecolor(BackColorBlue | ForeColorWhite);
				}
				else
					setconsolecolor(BackColorMagenta | ForeColorWhite);
				string value = currentelements[i].text;

				// Password should be typed eith *.
				if (currentelements[i].password)
					value = string(value.size(), '*');

				value.resize(70, ' ');
				cout << value;
			}
			break;
		}

		case ElementTypeButton:
			// Buttons have a padding of 20 chracters.
			setconsolecolor(currentlayout.color);
			cout << endl << endl << string(29, ' ');

			for (; i < currentelements.size() && currentelements[i].type == ElementTypeButton; i++)
			{
				cout << " ";
				if (i == focus)
				{
					cursorvisible = false;
					setconsolecolor(currentelements[i].focusedcolor);
				}
				else
					setconsolecolor(currentelements[i].color);
				cout << " " << currentelements[i].text << " ";
				setconsolecolor(currentlayout.color);
			}
			i--;
			break;
		}
		/*if (i < currentelements.size() - 1)
			cout << endl << endl;*/
	}
	// Set the cursor position on the focused element.
	setcursorposition(consoleinputy, consoleinputx + currentlayout.colindex);
	showcursor(cursorvisible);
}

void puttext(string text, ConsoleColor color)
{
	puttext("", text, color);
}

void puttext(string name, string text, ConsoleColor color)
{
	Element c;
	c.type = ElementTypeText;
	c.name = name;
	c.text = text;
	if (color == -1)
		c.color = currentlayout.color;
	else
		c.color = color;
	currentelements.push_back(c);
}

void puttextbox(string name, string title, string text, bool password, bool readonly)
{
	Element c;
	c.type = ElementTypeTextBox;
	c.name = name;
	c.title = title;
	c.text = text;
	c.password = password;
	c.readonly = readonly;
	currentelements.push_back(c);
}

void putbutton(string name, string text, ConsoleColor color, ConsoleColor focusedcolor)
{
	Element c;
	c.type = ElementTypeButton;
	c.name = name;
	c.text = text;
	c.color = color;
	c.focusedcolor = focusedcolor;
	currentelements.push_back(c);
}

Element &getelement(string name)
{
	// Find the index of the textbox.
	int index = -1;
	for (int i = 0; i < currentelements.size(); i++)
		if (currentelements[i].name == name)
		{
			index = i;
			break;
		}

	return currentelements[index];
}

Layout & getlayout()
{
	return currentlayout;
}

string waitforuserinput()
{
	// Clear the screen and draw the layout.
	showlayout();
	while (true)
	{
		int ch = _getch();

		// Get the focused element.
		Element &c = currentelements[getfocusedelementindex()];
		switch (ch)
		{
		case KeyTab: // Jump to next focusable element.
			gotonextelement();
			break;

		case KeyEnter: // Enter key for button indicates button click,
					   // and for textbox indicates jump to next element.
			if (c.type == ElementTypeButton)
				return c.name;
			else
				gotonextelement();
			break;

		case KeyBackspace: // Backspace is processed in the focused textbox.
			if (c.type == ElementTypeTextBox)
				processtextbox(ch);
			break;

		case KeyEscape: // Escape key usualy indicates the cancel or exit key.
			return "ESCAPE";

		case 224: // Input character 224 indicates control character.
				  // Here, we process up, down, left, right, delete, home and end keys.
			ch = _getch();
			switch (ch)
			{
			case KeyUp: // Jump to previous focusable element.
				gotopreviouselement();
				break;

			case KeyDown: // Jump to next focusable element.
				gotonextelement();
				break;

			case KeyLeft: // For textbox move to next character,
						  // otherwise jump to next focusable element.
				if (c.type == ElementTypeButton)
					gotopreviouselement();
				else
					processtextbox(ch, true);
				break;

			case KeyRight: // For textbox move to previous character,
						   // otherwise jump to previous focusable element.
				if (c.type == ElementTypeButton)
					gotonextelement();
				else
					processtextbox(ch, true);
				break;

			case KeyDelete: // Delete key is processed in the focused textbox.
				if (c.type == ElementTypeTextBox)
					processtextbox(ch, true);
				break;

			default: // Other control characters including home and end keys for processing in the focused textbox.
				if (c.type == ElementTypeTextBox)
					processtextbox(ch, true);
				break;
			}
		case 0: // Ignore
			break;

		default: // Other characters for processing in the focused textbox.
			if (c.type == ElementTypeTextBox)
				processtextbox(ch);
			break;
		}
		// Fast redraw the layout without clearing the screen.
		refreshlayout();
	}
}

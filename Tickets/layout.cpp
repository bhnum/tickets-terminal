#include "layout.h"

#include <iostream>
#include <conio.h>
#include <stack>

// Initialize a stack of layouts with at least one element.
stack<Layout> layouts({ Layout() });

// Short forms of certain widely used expressions.
#define currentlayout layouts.top()
#define currentcomponents layouts.top().components

// ---------------------------------- private function implementations ----------------------------------------

bool isfocusable(Component &c)
{
	if (c.type == ComponentTypeButton) return true;
	if (c.type == ComponentTypeTextBox && !c.readonly) return true;
	return false;
}

int getfocusedcomponentindex()
{
	int focusables = 0;
	for (int i = 0; i < currentcomponents.size(); i++)
	{
		Component& c = currentcomponents[i];
		if (isfocusable(c))
			focusables++;
	}
	if (focusables == 0)
		return 0;

	currentlayout.tabindex = (currentlayout.tabindex + focusables) % focusables;
	int index = 0;
	for (int t = currentlayout.tabindex; index < currentcomponents.size() && t > 0 ; index++)
	{
		Component& c = currentcomponents[index];
		if (isfocusable(c))
			t--;
	}
	return index;
}

void nextcomponent()
{
	currentlayout.tabindex++;
	currentlayout.colindex = 0;
}

void previouscomponent()
{
	currentlayout.tabindex--;
	currentlayout.colindex = 0;
}

// Inputs which should be processed for the focused textbox are sent to this fuction.
void processtextbox(int ch, bool controlchar = false)
{
	Component &textbox = currentcomponents[getfocusedcomponentindex()];
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
	currentcomponents.clear();
}

void refreshlayout()
{
	// Get the focused component index.
	int focus = getfocusedcomponentindex();

	// The coordinates of the focused component.
	short consoleinputx = 0, consoleinputy = 0;

	// The cursor should be visible if the focused component is a textbox, invisible otherwise.
	bool cursorvisible = false;

	// Hide the cursor while rendering the components.
	showcursor(false);

	// Start drawing from the top left of the screen.
	setcursorposition(0, 0);

	// Draw the layout caption with red background.
	setlinecolor(BackColorRed | ForeColorWhite);
	cout << currentlayout.caption << endl << endl;

	for (int i = 0; i < currentcomponents.size(); i++)
	{
		switch (currentcomponents[i].type)
		{
		case ComponentTypeText:
			// Empty text components mean new line and are used to put buttons in a new line.
			if (currentcomponents[i].text == "")
				continue;

			// Draw text in the corressping color.
			setconsolecolor(currentcomponents[i].color);
			cout << currentcomponents[i].text;
			break;

		case ComponentTypeTextBox:
		{
			// Draw the title of the textbox in grey with maximum width of 20 chracters.
			setconsolecolor(BackColorWhite | ForeColorDarkGrey);
			string title = currentcomponents[i].title;
			title.resize(20, ' ');
			cout << title;

			if (currentcomponents[i].readonly) 
			{
				// Draw the text of the textbox as regular text.
				setconsolecolor(currentlayout.color);
				cout << currentcomponents[i].text;
			}
			else
			{
				// Draw the text of the textbox in magenta if focused or blue otherwise
				// with maximum width of 80 chracters.
				if (i == focus)
				{
					cursorvisible = true;
					getcursorposition(consoleinputy, consoleinputx);
					setconsolecolor(BackColorMagenta | ForeColorWhite);
				}
				else
					setconsolecolor(BackColorBlue | ForeColorWhite);
				string value = currentcomponents[i].text;

				// Password should be typed eith *.
				if (currentcomponents[i].password)
					value = string(value.size(), '*');

				value.resize(80, ' ');
				cout << value;
			}
			break;
		}

		case ComponentTypeButton:
			// Buttons have a padding of 20 chracters.
			setconsolecolor(BackColorWhite | ForeColorBlack);
			cout << string(19, ' ');

			for (; i < currentcomponents.size() && currentcomponents[i].type == ComponentTypeButton; i++)
			{
				cout << " ";
				if (i == focus)
				{
					cursorvisible = false;
					setconsolecolor(currentcomponents[i].focusedcolor);
				}
				else
					setconsolecolor(currentcomponents[i].color);
				cout << " " << currentcomponents[i].text << " ";
				setconsolecolor(BackColorWhite | ForeColorBlack);
			}
			i--;
			break;
		}
		if (i < currentcomponents.size() - 1)
			cout << endl << endl;
	}
	// Set the cursor position on the focused component.
	setcursorposition(consoleinputy, consoleinputx + currentlayout.colindex);
	showcursor(cursorvisible);
}

void puttext(string text, ConsoleColor color)
{
	puttext("", text, color);
}

void puttext(string name, string text, ConsoleColor color)
{
	Component c;
	c.type = ComponentTypeText;
	c.name = name;
	c.text = text;
	if (color == -1)
		c.color = currentlayout.color;
	else
		c.color = color;
	currentcomponents.push_back(c);
}

void puttextbox(string name, string title, string text, bool password, bool readonly)
{
	Component c;
	c.type = ComponentTypeTextBox;
	c.name = name;
	c.title = title;
	c.text = text;
	c.password = password;
	c.readonly = readonly;
	currentcomponents.push_back(c);
}

void putbutton(string name, string text, ConsoleColor color, ConsoleColor focusedcolor)
{
	Component c;
	c.type = ComponentTypeButton;
	c.name = name;
	c.text = text;
	c.color = color;
	c.focusedcolor = focusedcolor;
	currentcomponents.push_back(c);
}

Component &getcomponent(string name)
{
	// Find the index of the textbox.
	int index = -1;
	for (int i = 0; i < currentcomponents.size(); i++)
		if (currentcomponents[i].name == name)
		{
			index = i;
			break;
		}

	return currentcomponents[index];
}

string waitforuserinput()
{
	// Clear the screen and draw the layout.
	showlayout();
	while (true)
	{
		int ch = _getch();

		// Get the focused component.
		Component &c = currentcomponents[getfocusedcomponentindex()];
		switch (ch)
		{
		case KeyTab: // Jump to next focusable component.
			nextcomponent();
			break;

		case KeyEnter: // Enter key for button indicates button click,
					   // and for textbox indicates jump to next component.
			if (c.type == ComponentTypeButton)
				return c.name;
			else
				nextcomponent();
			break;

		case KeyBackspace: // Backspace is processed in the focused textbox.
			if (c.type == ComponentTypeTextBox)
				processtextbox(ch);
			break;

		case KeyEscape: // Escape key usualy indicates the cancel or exit key.
			return "ESCAPE";

		case 224: // Input character 224 indicates control character.
				  // Here, we process up, down, left, right, delete, home and end keys.
			ch = _getch();
			switch (ch)
			{
			case KeyUp: // Jump to previous focusable component.
				previouscomponent();
				break;

			case KeyDown: // Jump to next focusable component.
				nextcomponent();
				break;

			case KeyLeft: // For textbox move to next character,
						  // otherwise jump to next focusable component.
				if (c.type == ComponentTypeButton)
					previouscomponent();
				else
					processtextbox(ch, true);
				break;

			case KeyRight: // For textbox move to previous character,
						   // otherwise jump to previous focusable component.
				if (c.type == ComponentTypeButton)
					nextcomponent();
				else
					processtextbox(ch, true);
				break;

			case KeyDelete: // Delete key is processed in the focused textbox.
				if (c.type == ComponentTypeTextBox)
					processtextbox(ch, true);
				break;

			default: // Other control characters including home and end keys for processing in the focused textbox.
				if (c.type == ComponentTypeTextBox)
					processtextbox(ch, true);
				break;
			}
		case 0: // Ignore
			break;

		default: // Other characters for processing in the focused textbox.
			if (c.type == ComponentTypeTextBox)
				processtextbox(ch);
			break;
		}
		// Fast redraw the layout without clearing the screen.
		refreshlayout();
	}
}

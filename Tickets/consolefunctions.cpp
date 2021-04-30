#include "consolefunctions.h"

#include <iostream>
#include <Windows.h>

using namespace std;

// Source: https://stackoverflow.com/questions/5866529/how-do-we-clear-the-console-in-assembly/5866648#5866648
void clearscreen(ConsoleColor color)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	GetConsoleScreenBufferInfo(hConsole, &info);
	//if (color != -1) info.wAttributes = color;

	DWORD written, cells = info.dwSize.X * 50;
	FillConsoleOutputCharacter(hConsole, ' ', cells, { 0,0 }, &written);
	FillConsoleOutputAttribute(hConsole, info.wAttributes, cells, { 0,0 }, &written);
	SetConsoleCursorPosition(hConsole, { 0, 0 });
}

void clearallscreen(ConsoleColor color)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	GetConsoleScreenBufferInfo(hConsole, &info);
	//if (color != -1) info.wAttributes = color;

	DWORD written, cells = info.dwSize.X * info.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, ' ', cells, { 0,0 }, &written);
	FillConsoleOutputAttribute(hConsole, info.wAttributes, cells, { 0,0 }, &written);
	SetConsoleCursorPosition(hConsole, { 0, 0 });
}

// Source: https://docs.microsoft.com/en-us/windows/console/setconsolecursorposition
void setcursorposition(short r, short c)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, { c, r });
}

void getcursorposition(short &r, short &c)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	r = info.dwCursorPosition.Y;
	c = info.dwCursorPosition.X;
}

// Source: https://stackoverflow.com/questions/18028808/blinking-underscore-with-console/18028927#18028927
void showcursor(bool show)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = show; // set the cursor visibility
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Source: https://docs.microsoft.com/en-us/windows/console/setconsoletextattribute
void setconsolecolor(ConsoleColor color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

// Source: https://docs.microsoft.com/en-us/windows/console/scrolling-a-screen-buffer-s-window
int getconsolewidth()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	return info.dwSize.X;
}

void setlinecolor(ConsoleColor color, int n)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	//info.wAttributes = color;

	DWORD written, cells = info.dwSize.X * n - info.dwCursorPosition.X;
	FillConsoleOutputAttribute(hConsole, info.wAttributes, cells, info.dwCursorPosition, &written);
	SetConsoleCursorPosition(hConsole, info.dwCursorPosition);
}
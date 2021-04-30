#pragma once

enum ForeColor {
	ForeColorBlack = 0,
	ForeColorBlue = 1,
	ForeColorGreen = 2,
	ForeColorCyan = 3,
	ForeColorRed = 4,
	ForeColorMagenta = 5,
	ForeColorBrown = 6,
	ForeColorLightGrey = 7,
	ForeColorDarkGrey = 8,
	ForeColorLightBlue = 9,
	ForeColorLightGreen = 10,
	ForeColorLightCyan = 11,
	ForeColorLightRed = 12,
	ForeColorLightMagenta = 13,
	ForeColorLightBrown = 14,
	ForeColorWhite = 15,
};

enum BackColor {
	BackColorBlack = 0 << 4,
	BackColorBlue = 1 << 4,
	BackColorGreen = 2 << 4,
	BackColorCyan = 3 << 4,
	BackColorRed = 4 << 4,
	BackColorMagenta = 5 << 4,
	BackColorBrown = 6 << 4,
	BackColorLightGrey = 7 << 4,
	BackColorDarkGrey = 8 << 4,
	BackColorLightBlue = 9 << 4,
	BackColorLightGreen = 10 << 4,
	BackColorLightCyan = 11 << 4,
	BackColorLightRed = 12 << 4,
	BackColorLightMagenta = 13 << 4,
	BackColorLightBrown = 14 << 4,
	BackColorWhite = 15 << 4,
};

typedef int ConsoleColor;

void clearscreen(ConsoleColor color = -1);

void setcursorposition(short r, short c);

void getcursorposition(short &r, short &c);

void showcursor(bool show);

void setconsolecolor(ConsoleColor color);

int getconsolewidth();

void setlinecolor(ConsoleColor color, int n = 1);

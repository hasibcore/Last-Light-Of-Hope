#ifndef CREDITS_H
#define CREDITS_H

#include "Variables.h"

// Credits screen rendering
void drawCreditsPage()
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	iShowBMP(0, 0, "Images\\credits.bmp");
	if (hoverBack)
	{
		iSetColor(255, 0, 0);
		char text[] = "BACK";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(532 + (120 - textWidth) / 2, 47, text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(526, 10, "Images\\button4.bmp", 0);
		iSetColor(235, 205, 150);
		char text[] = "BACK";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(532 + (120 - textWidth) / 2, 47, text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

#endif // CREDITS_H

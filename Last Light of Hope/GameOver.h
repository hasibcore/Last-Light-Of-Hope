#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Variables.h"

// Game over screen rendering
void drawGameOverPage()
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	iSetColor(255, 0, 0);
	char goText[] = "GAME OVER";
	int goWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)goText);
	iText((screen_width - goWidth) / 2, screen_height / 2, goText, GLUT_BITMAP_TIMES_ROMAN_24);
	
	iSetColor(255, 255, 255);
	char clickText[] = "Click to Restart Level";
	int clickWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)clickText);
	iText((screen_width - clickWidth) / 2, screen_height / 2 - 50, clickText, GLUT_BITMAP_HELVETICA_18);
}

#endif // GAME_OVER_H

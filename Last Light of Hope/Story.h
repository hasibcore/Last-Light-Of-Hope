#ifndef STORY_H
#define STORY_H

#include "Variables.h"

// Story drawing logic
void drawStoryPage()
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, 1200, 700);
	if (storyStep < 6)
	{
		char text[100];
		if (storyStep == 0)
		{
			sprintf(text, "Why is he running?");
		}
		else if (storyStep == 1)
		{
			sprintf(text, "Why is he chasing?");
		}
		else if (storyStep == 2)
		{
			sprintf(text, "Why is he saving?");
		}
		else if (storyStep == 3)
		{
			sprintf(text, "But who?");
		}
		else if (storyStep == 4)
		{
			sprintf(text, "Want to know?");
		}
		else if (storyStep == 5)
		{
			sprintf(text, "Okay!");
		}
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		int startX = (1200 - textWidth) / 2;
		int startY = 340;
		iSetColor(200, 0, 0);
		// The following lines were identified as belonging to a different file (e.g., GameOver.h)
		iText(startX, startY, text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (storyStep >= 6 && storyStep <= 15)
	{
		char file[50];
		sprintf(file, "Images\\story%d.bmp", storyStep - 5);
		iShowBMP(0, 0, file);
	}
	iSetColor(255, 255, 255);
	char skipText[] = "Click to skip";
	iText(20, 665, skipText, GLUT_BITMAP_HELVETICA_18);
}

#endif // STORY_H

#ifndef MENU_H
#define MENU_H

#include "Variables.h"
#include "Sound.h"

// Forward declarations
void loadLevelMaze(int level);
void skipStoryStep();
void resetLevel(int level);

// Button click handlers
inline void startButtonClickHandler()
{
	homePage = 0;
	instructionPage = 1;
	instructionTimer = 0;
	playerX = 60;
	playerY = 60;
	playerDirection = 0;
	gameTime = INITIAL_TIME;
	playerHealth = 100;
	startLevelHealth = 100;
	monsterHealth = 100;
	fightPlayerX = 157;
	monsterX = 765;
	isWin = false;
	attackCooldown = 0;
	storyFinished = false;
	storyStep = 0;
	stopSound();
	mazeMusicPlaying = false;
	fightMusicPlaying = false;
	storyMusicPlaying = false;
	currentMusic = MUSIC_NONE;
}

inline void creditsButtonClickHandler()
{
	homePage = 0;
	creditsPage = 1;
}

// Menu drawing logic
inline void drawHomePage()
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	iShowBMP(0, 0, (char*)"Images\\interface.bmp");
	if (hoverStart)
	{
		iSetColor(255, 0, 0);
		char* text = (char*)"START";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(213.0 + (double)(222 - textWidth) / 2.0, 173.0, (char*)text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(186, 94, (char*)"Images\\button1.bmp", 0);
		iSetColor(235, 205, 150);
		char* text = (char*)"START";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(213.0 + (double)(222 - textWidth) / 2.0, 173.0, (char*)text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (hoverCredits)
	{
		iSetColor(255, 0, 0);
		char* text = (char*)"CREDITS";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(488.0 + (double)(222 - textWidth) / 2.0, 173.0, (char*)text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(471, 89, (char*)"Images\\button2.bmp", 0);
		iSetColor(220, 215, 200);
		char* text = (char*)"CREDITS";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(488.0 + (double)(222 - textWidth) / 2.0, 173.0, (char*)text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (hoverQuit)
	{
		iSetColor(255, 0, 0);
		char* text = (char*)"QUIT";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(765.0 + (double)(225 - textWidth) / 2.0, 173.0, (char*)text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(753, 83, (char*)"Images\\button3.bmp", 0);
		iSetColor(230, 225, 210);
		char* text = (char*)"QUIT";
		int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
		iText(765.0 + (double)(225 - textWidth) / 2.0, 173.0, (char*)text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

inline void handleMenuPassiveMouseMove(int mx, int my) {
  extern double pauseScale;
  hoverLvl1 = (levelPage == 1 && mx >= 100 && mx <= 400 && my >= 250 && my <= 450);
  hoverLvl2 = (levelPage == 1 && mx >= 450 && mx <= 750 && my >= 250 && my <= 450 && unlockedLevel >= 2);
  hoverLvl3 = (levelPage == 1 && mx >= 800 && mx <= 1100 && my >= 250 && my <= 450 && unlockedLevel >= 3);
  if (homePage == 1) {
    hoverStart = (mx >= 213 && mx <= 435 && my >= 157 && my <= 204);
    hoverCredits = (mx >= 488 && mx <= 710 && my >= 157 && my <= 204);
    hoverQuit = (mx >= 765 && mx <= 990 && my >= 157 && my <= 204);
  } else if (creditsPage == 1) {
    hoverBack = (mx >= 540 && mx <= 660 && my >= 35 && my <= 75);
  }
  if (isPaused && pauseScale > 0.5) {
    // Correctly calculating the menu center based on screen_height (700)
    // to match drawPauseOverlay in Maze.h exactly.
    int bx = (1200 - 450) / 2;
    int by = (700 - 350) / 2; 
    int btnX = bx + 50;
    int btnY = by + 80;
    int btnW = 350;
    int btnH = 90;
    hoverPauseLeave = (mx >= btnX && mx <= btnX + btnW && my >= btnY && my <= btnY + btnH);
  } else {
    hoverPauseLeave = false;
  }

  // Finale Hover Detection (Maze 3 Victory)
  if (isWin && currentLevel == 3)
  {
      int bx = 1200 / 2 - 200;
      int by = 700 / 2 - 70;
      hoverNewGame = (mx >= bx && mx <= bx + 400 && my >= by && my <= by + 80);
  }
}

inline void handleMenuMouseClick(int button, int state, int mx, int my) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if (isPaused && hoverPauseLeave) {
      isPaused = false;
      startPage = 0;
      homePage = 1;
      unlockedLevel = 1; // Option: reset or keep? Plan said reset level state.
      stopSound();
      return;
    }
    if (homePage == 1 && hoverStart) startButtonClickHandler();
    else if (homePage == 1 && hoverCredits) creditsButtonClickHandler();
    else if (homePage == 1 && hoverQuit) exit(0);
    else if (creditsPage == 1 && mx >= 540 && mx <= 660 && my >= 35 && my <= 75) {
      homePage = 1;
      creditsPage = 0;
    } else if (gameOverPage == 1) {
      gameOverPage = 0;
      resetLevel(currentLevel);
      startPage = 1;
    } else if (instructionPage == 1) {
      instructionPage = 0;
      levelPage = 1;
      instructionTimer = 0;
    } else if (levelPage == 1) {
      if (mx >= 100 && mx <= 400 && my >= 250 && my <= 450) {
        currentLevel = 1; levelPage = 0; startPage = 1; loadLevelMaze(1);
        playerX = 60; playerY = 60; gameTime = INITIAL_TIME;
        startLevelHealth = playerHealth;
        obj1Solved = obj2Solved = obj3Solved = false;
      } else if (mx >= 450 && mx <= 750 && my >= 250 && my <= 450 && unlockedLevel >= 2) {
        currentLevel = 2; levelPage = 0; startPage = 1; loadLevelMaze(2);
        playerX = 60; playerY = 60; gameTime = INITIAL_TIME;
        startLevelHealth = playerHealth;
        obj1Solved = obj2Solved = obj3Solved = false;
      } else if (mx >= 800 && mx <= 1100 && my >= 250 && my <= 450 && unlockedLevel >= 3) {
        currentLevel = 3; levelPage = 0; startPage = 1; loadLevelMaze(3);
        playerX = 60; playerY = 60; gameTime = INITIAL_TIME;
        startLevelHealth = playerHealth;
        obj1Solved = obj2Solved = obj3Solved = false;
      }
    } else if (isWin) {
      if (currentLevel == 3 && hoverNewGame) {
        // FULL RESET FOR NEW GAME
        isWin = false;
        winTimer = 0;
        fightPage = 0;
        homePage = 1;
        unlockedLevel = 1;
        currentLevel = 1;
        storyFinished = false;
        playerHealth = 100;
        playerAttackPower = 15;
        // Reset all maze level progress
        level1Finished = level2Finished = level3Finished = false;
        lv1P1Solved = lv1P2Solved = false;
        lv2P1Solved = lv2P2Solved = lv2P3Solved = lv2P4Solved = false;
        lv3P1Solved = lv3P2Solved = lv3P3Solved = lv3P4Solved = lv3P5Solved = lv3P6Solved = false;
        stopSound();
        return;
      }

      // Standard Victory logic for Mazes 1 and 2
      isWin = false;
      winTimer = 0;
      fightPage = 0;
      fightMusicPlaying = false;
      stopSound();
      if (currentLevel == 1) {
        level1Finished = true;
        if (unlockedLevel < 2) unlockedLevel = 2;
      } else if (currentLevel == 2) {
        level2Finished = true;
        if (unlockedLevel < 3) unlockedLevel = 3;
      } else if (currentLevel == 3) {
        level3Finished = true;
      }
      levelPage = 1;
      monsterHealth = 100;
      displayMonsterHealth = 100;
    } else if (storyPage == 1) {
      skipStoryStep();
    }
  }
}

#endif // MENU_H

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "resource.h"
#include "Variables.h"
#include "Common.h"
#include "Credits.h"
#include "Fight.h"
#include "GameOver.h"
#include "Logic.h"
#include "Maze.h"
#include "Menu.h"
#include "Story.h"

// Global variables definitions
bool menuMusicPlaying = false;
bool mazeMusicPlaying = false;
bool fightMusicPlaying = false;
bool storyMusicPlaying = false;
bool heartBeatPlaying = false;
bool walkSoundPlaying = false;
bool monsterRunSoundPlaying = false;
bool monsterAttackSoundPlaying = false;
bool swordSoundPlaying = false;
bool gameOverSoundPlaying = false;
MusicState currentMusic = MUSIC_NONE;
bool isPaused = false;

int playerX = 60;
int playerY = 60;
int playerSpeed = 2;
int gameTime = INITIAL_TIME;
char timeStr[20];
int playerDirection = 0;
int animIndex = 1;
bool isMoving = false;
int animTimer = 0;

bool hoverStart = false;
bool hoverCredits = false;
bool hoverQuit = false;
bool hoverBack = false;
bool hoverNewGame = false;

double fightPlayerX = 157.0;
float fightPlayerVelX = 0.0f;
double fightPlayerY = 176.0;
double monsterX = 765.0;
double monsterY = 176.0;
int playerHealth = 100;
int startLevelHealth = 100;
int playerAttackPower = 15;
int hpGainTimer = 0;
int hpGainValue = 0;
int monsterHealth = 100;
bool hoverPauseLeave = false;
double pauseScale = 0.0;
double displayPlayerHealth = 100.0;
double displayMonsterHealth = 100.0;
int fightWalkIndex = 1;
int fightAttackIndex = 0;
int monsterMoveIndex = 1;
int monsterAttackIndex = 1;
int monsterState = 0;
int damageFlashTimer = 0;
int attackCooldown = 0;
bool isWin = false;
int winTimer = 0;

int homePage = 1;
int instructionPage = 0;
int instructionTimer = 0;
int startPage = 0;
int creditsPage = 0;
int storyPage = 0;
int storyStep = 0;
int storyTimer = 0;
bool storyFinished = false;
int fightPage = 0;
int gameOverPage = 0;

// Puzzle variables
bool lv1P1Solved = false;
bool lv1P2Solved = false;
bool lv2P1Solved = false;
bool lv2P2Solved = false;
bool lv2P3Solved = false;
bool lv2P4Solved = false;
bool lv3P1Solved = false;
bool lv3P2Solved = false;
bool lv3P3Solved = false;
bool lv3P4Solved = false;
bool lv3P5Solved = false;
bool lv3P6Solved = false;
int activePuzzle = 0; 
char puzzleInput[20] = "";
int puzzleInputIndex = 0;
int puzzleTimer = 0;
int puzzle1R, puzzle1C;
int puzzle2R, puzzle2C;
int puzzle3R, puzzle3C;
int puzzle4R, puzzle4C;
int puzzle5R, puzzle5C;
int puzzle6R, puzzle6C;
int puzzleMoveTimer = 0;

// Level progression variables initialization
int unlockedLevel = 1;
int currentLevel = 1;
int levelPage = 0;
bool obj1Solved = false;
bool obj2Solved = false;
bool obj3Solved = false;
bool level1Finished = false, level2Finished = false, level3Finished = false;
bool hoverLvl1 = false, hoverLvl2 = false, hoverLvl3 = false;

char animNames[4][20] = {"playerTopFront", "playerTopBack", "playerTopLeft",
                         "playerTopRight"};

int maze[ROWS][COLS];

// --- iGraphics Callbacks ---

void iDraw() {
  iClear();
  if (gameOverPage == 1)
    drawGameOverPage();
  else if (homePage == 1)
    drawHomePage();
  else if (instructionPage == 1) {
    iShowBMP(0, 0, (char *)"Images\\instruction.bmp");
    iSetColor(255, 255, 255);
    iText(20, 665, (char *)"Click to skip", GLUT_BITMAP_HELVETICA_18);
  } else if (startPage == 1) {
    drawStartPage();
    if (activePuzzle > 0 && !isPaused)
      drawPuzzleOverlay();
    if (isPaused) {
      drawPauseOverlay();
    }
  } else if (creditsPage == 1)
    drawCreditsPage();
  else if (levelPage == 1)
    drawLevelSelectionPage();
  else if (storyPage == 1)
    drawStoryPage();
  else if (fightPage == 1)
    drawFightPage();
}

void iMouseMove(int mx, int my) {}

void iPassiveMouseMove(int mx, int my) {
  handleMenuPassiveMouseMove(mx, my);
}

void iMouse(int button, int state, int mx, int my) {
  handleMenuMouseClick(button, state, mx, my);
}

void iKeyboard(unsigned char key) {
  if (key == 27) { // ESC Key
    if (startPage == 1 || fightPage == 1) {
      isPaused = !isPaused;
      return;
    }
  }

  // Silently consume movement/action keys on maze and fight pages
  // to prevent the Windows error beep for unhandled key events.
  if (startPage == 1 || fightPage == 1) {
    unsigned char lk = key | 0x20; // lowercase
    if (lk == 'w' || lk == 'a' || lk == 's' || lk == 'd' || lk == 'f') {
      if (activePuzzle > 0)
        handlePuzzleKeyboard(key);
      return;
    }
  }

  if (activePuzzle > 0) {
    handlePuzzleKeyboard(key);
  }
}

void iSpecialKeyboard(unsigned char key) {
  if (key == GLUT_KEY_END) {
    exit(0);
  }
}

int main() {
  srand((unsigned int)time(NULL));
  loadLevelMaze(1); // Initialize the first level
  iInitialize(screen_width, screen_height, (char *)"Last Light of hope");
  HWND hwnd = FindWindow(NULL, "Last Light of hope");
  HICON hIcon = (HICON)LoadImage(
	  GetModuleHandle(NULL),
	  MAKEINTRESOURCE(IDI_ICON1),
	  IMAGE_ICON,
	  0, 0,
	  LR_DEFAULTSIZE
	  );
  SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
  SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
  iSetTimer(1000, countDown);
  iSetTimer(20, fixedUpdate);
  iStart();
  return 0;
}
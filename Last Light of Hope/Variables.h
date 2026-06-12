#ifndef VARIABLES_H
#define VARIABLES_H

#include "Common.h"

// Sound related variables
extern bool menuMusicPlaying;
extern bool mazeMusicPlaying;
extern bool fightMusicPlaying;
extern bool storyMusicPlaying;
extern bool heartBeatPlaying;
extern bool walkSoundPlaying;
extern bool monsterRunSoundPlaying;
extern bool monsterAttackSoundPlaying;
extern bool swordSoundPlaying;
extern bool gameOverSoundPlaying;
extern MusicState currentMusic;
extern int playerSpeed;

// Player/Game state
extern int playerX, playerY;
extern int playerDirection;
extern int animIndex;
extern bool isMoving;
extern int animTimer;
extern int playerHealth;
extern int startLevelHealth;
extern int playerAttackPower;
extern int hpGainTimer;
extern int hpGainValue;
extern int monsterHealth;
extern bool hoverPauseLeave;
extern double pauseScale;
extern double displayPlayerHealth;
extern double displayMonsterHealth;
extern int gameTime;
extern bool isPaused;

// UI/Button variables
extern bool hoverStart;
extern bool hoverCredits;
extern bool hoverQuit;
extern bool hoverBack;
extern bool hoverNewGame;

// Fight scene variables
extern double fightPlayerX;
extern double fightPlayerY;
extern float fightPlayerVelX;
extern double monsterX;
extern double monsterY;
extern int fightWalkIndex;
extern int fightAttackIndex;
extern int monsterMoveIndex;
extern int monsterAttackIndex;
extern int monsterState;
extern int damageFlashTimer;
extern int attackCooldown;
extern bool isWin;
extern int winTimer;

// Pages and Timers
extern int homePage;
extern int instructionPage;
extern int instructionTimer;
extern int startPage;
extern int creditsPage;
extern int storyPage;
extern int storyStep;
extern int storyTimer;
extern bool storyFinished;
extern int fightPage;
extern int gameOverPage;

// Puzzle variables
extern bool lv1P1Solved;
extern bool lv1P2Solved;
extern bool lv2P1Solved;
extern bool lv2P2Solved;
extern bool lv2P3Solved;
extern bool lv2P4Solved;
extern bool lv3P1Solved;
extern bool lv3P2Solved;
extern bool lv3P3Solved;
extern bool lv3P4Solved;
extern bool lv3P5Solved;
extern bool lv3P6Solved;
extern int activePuzzle;
extern char puzzleInput[20];
extern int puzzleInputIndex;
extern int puzzleTimer;
extern int puzzle1R, puzzle1C;
extern int puzzle2R, puzzle2C;
extern int puzzle3R, puzzle3C;
extern int puzzle4R, puzzle4C;
extern int puzzle5R, puzzle5C;
extern int puzzle6R, puzzle6C;
extern int puzzleMoveTimer;

// Level progression variables
extern int unlockedLevel;
extern int currentLevel;
extern int levelPage;
extern bool obj1Solved;
extern bool obj2Solved;
extern bool obj3Solved;
extern bool level1Finished, level2Finished, level3Finished;
extern bool hoverLvl1, hoverLvl2, hoverLvl3;

// Assets
extern char animNames[4][20];
extern int maze[14][24]; // Use constants directly if macros fail

#endif // VARIABLES_H

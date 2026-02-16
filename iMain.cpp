#define _CRT_SECURE_NO_WARNINGS  // Disables warnings for certain insecure functions (like sprintf)
#include "iGraphics.h"             // Includes the custom iGraphics library header for graphics functions
#include <stdio.h>                 // Includes standard input/output functions (e.g., sprintf)
#include <math.h>                  // Includes math functions (e.g., sqrt, pow, abs)
#include <windows.h>               // Includes Windows API (used by PlaySound)
#include <time.h>                  // Includes time functions (used for srand)

bool mazeMusicPlaying = false;     // Tracks whether maze background music is currently playing
bool fightMusicPlaying = false;    // Tracks whether fight background music is currently playing
bool storyMusicPlaying = false;    // Tracks whether story background music is currently playing
bool heartBeatPlaying = false;     // Tracks whether heartbeat sound is currently playing
bool walkSoundPlaying = false;     // Tracks whether walking sound effect is currently playing
bool monsterRunSoundPlaying = false; // Tracks whether monster running sound is currently playing
bool monsterAttackSoundPlaying = false; // Tracks whether monster attack sound is currently playing
bool swordSoundPlaying = false;    // Tracks whether sword attack sound is currently playing
bool gameOverSoundPlaying = false; // Tracks whether game over sound has been played

const int screen_width = 1200;     // Constant for window/screen width
const int screen_height = 700;     // Constant for window/screen height
const int TILE_SIZE = 50;          // Size of each maze tile (in pixels)
const int ROWS = 14;               // Number of rows in the maze grid
const int COLS = 24;               // Number of columns in the maze grid

int playerX = 60;                  // Player's X position in the maze
int playerY = 60;                  // Player's Y position in the maze
int playerSpeed = 2;               // Player movement speed (pixels per frame)
const int INITIAL_TIME = 80;       // Initial countdown timer value (seconds)
int gameTime = INITIAL_TIME;       // Current remaining game time
char timeStr[20];                  // Buffer to hold formatted time string for display

const int LIGHT_RADIUS = 90;       // Radius of the light circle around the player (fog of war effect)
const int charWidth = 55;          // Width of the player character sprite
const int charHeight = 35;         // Height of the player character sprite

int playerDirection = 0;           // Current direction the player is facing (0=down, 1=up, 2=left, 3=right)
int animIndex = 1;                 // Current frame index for player walking animation
bool isMoving = false;             // Flag indicating whether the player is currently moving
int animTimer = 0;                 // Timer to control animation frame rate

bool hoverStart = false;           // True when mouse hovers over START button
bool hoverCredits = false;         // True when mouse hovers over CREDITS button
bool hoverQuit = false;            // True when mouse hovers over QUIT button
bool hoverBack = false;            // True when mouse hovers over BACK button in credits

int fightPlayerX = 157;            // Player X position in the fight scene
int fightPlayerY = 176;            // Player Y position in the fight scene (fixed)
int monsterX = 765;                // Monster X position in the fight scene
int monsterY = 176;                // Monster Y position in the fight scene (fixed)

int playerHealth = 100;            // Current player health in fight
int monsterHealth = 100;           // Current monster health in fight
int displayPlayerHealth = 100;     // Smoothed health value for player health bar (visual lag)
int displayMonsterHealth = 100;    // Smoothed health value for monster health bar (visual lag)

int fightWalkIndex = 1;            // Animation frame for player walking in fight
int fightAttackIndex = 0;          // Animation frame for player attacking in fight (0 = not attacking)
int monsterMoveIndex = 1;          // Animation frame for monster movement/attack
int damageFlashTimer = 0;          // Timer for red flash effect when player takes damage
int attackCooldown = 0;            // Cooldown timer after player attacks

bool isWin = false;                // True when player defeats the monster
int winTimer = 0;                  // Timer after winning (to delay exit)
int gameOverPage = 0;              // Flag to show game over screen (1 = active)

enum MusicState                    // Enumeration for different music/sound states
{
	MUSIC_NONE,                    // No music/sound playing
	MUSIC_MAZE_BG,                 // Maze background music
	MUSIC_WALK,                    // Walking sound effect
	MUSIC_FIGHT,                   // Fight background music
	MUSIC_HEARTBEAT,               // Heartbeat sound when low health
	MUSIC_GAMEOVER,                // Game over sound
	SFX_MONSTER_ATTACK,            // Monster attack sound effect
	SFX_MONSTER_RUN,               // Monster running sound effect
	SFX_SWORD                      // Sword attack sound effect
};
MusicState currentMusic = MUSIC_NONE; // Current active music/sound state

char animNames[4][20] = {          // Array of base names for player animation sprites in different directions
	"playerTopFront",              // Down direction
	"playerTopBack",               // Up direction
	"playerTopLeft",               // Left direction
	"playerTopRight"               // Right direction
};

int maze[ROWS][COLS] = {           // 2D array representing the maze layout
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, // Row 0 (walls)
	{ 1, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 0, 2, 2, 4, 1 }, // Row 1 (0=walkable, 1=wall, 2=exit area, 3=trap, 4=exit, 5=story trigger)
	{ 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 2, 2, 2, 1 }, // Row 2
	{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 }, // Row 3
	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 }, // Row 4
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1 }, // Row 5
	{ 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 }, // Row 6
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 }, // Row 7
	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1 }, // Row 8
	{ 1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1 }, // Row 9
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 }, // Row 10
	{ 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 1 }, // Row 11
	{ 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 }, // Row 12
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }  // Row 13 (walls)
};

int homePage = 1;                  // Flag for main menu screen (1 = active)
void drawHomePage();               // Function declaration for drawing main menu

int startPage = 0;                 // Flag for maze gameplay screen (1 = active)
void drawStartPage();              // Function declaration for drawing maze gameplay
void startButtonClickHandler();    // Function declaration for START button action

int creditsPage = 0;               // Flag for credits screen (1 = active)
void drawCreditsPage();            // Function declaration for drawing credits
void creditsButtonClickHandler();  // Function declaration for CREDITS button action

int storyPage = 0;                 // Flag for story/cutscene screen (1 = active)
void drawStoryPage();              // Function declaration for drawing story
int storyStep = 0;                 // Current step in the story sequence
int storyTimer = 0;                // Timer to advance story steps
bool storyFinished = false;        // True after story sequence completes

int fightPage = 0;                 // Flag for fight scene (1 = active)
void drawFightPage();              // Function declaration for drawing fight scene
void drawGameOverPage();           // Function declaration for drawing game over screen

void playLoop(const char* file)    // Function to play a sound file in loop asynchronously
{
	PlaySound(file, NULL, SND_LOOP | SND_ASYNC); // Plays sound in loop
}

void playOnce(const char* file)    // Function to play a sound file once asynchronously
{
	PlaySound(file, NULL, SND_ASYNC); // Plays sound once
}

void stopSound()                   // Function to stop any currently playing sound
{
	PlaySound(NULL, 0, 0);         // Stops sound playback
}

void setMusic(MusicState next)     // Function to switch to a new music/sound state
{
	if (currentMusic == next)      // If already playing the requested sound, do nothing
	{
		return;
	}
	stopSound();                   // Stop current sound
	if (next == MUSIC_MAZE_BG)     // Play maze background music if requested
	{
		playLoop("Sounds\\mazeBg.wav");
	}
	else if (next == MUSIC_WALK)   // Play walking sound if requested
	{
		playLoop("Sounds\\walk.wav");
	}
	else if (next == MUSIC_FIGHT)  // Play fight background music if requested
	{
		playLoop("Sounds\\fight.wav");
	}
	else if (next == MUSIC_HEARTBEAT) // Play heartbeat sound if requested
	{
		playLoop("Sounds\\heartBeat.wav");
	}
	else if (next == MUSIC_GAMEOVER) // Play game over sound if requested
	{
		playOnce("Sounds\\gameOver.wav");
	}
	else if (next == SFX_MONSTER_ATTACK) // Play monster attack sound if requested
	{
		playOnce("Sounds\\monsterAttack.wav");
	}
	else if (next == SFX_MONSTER_RUN) // Play monster run sound if requested
	{
		playLoop("Sounds\\monsterRun.wav");
	}
	else if (next == SFX_SWORD)    // Play sword attack sound if requested
	{
		playOnce("Sounds\\sword.wav");
	}
	currentMusic = next;           // Update current music state
}

bool isWalkable(int x, int y)      // Function to check if a position is walkable (not wall)
{
	const int c = x / TILE_SIZE;   // Calculate column index from x coordinate
	const int r = y / TILE_SIZE;   // Calculate row index from y coordinate
	if (r < 0 || r >= ROWS || c < 0 || c >= COLS) // Out of bounds check
	{
		return false;
	}
	if (maze[r][c] == 1)           // Wall tile check
	{
		return false;
	}
	return true;                   // Position is walkable
}

void iDraw()                       // Main drawing function called every frame
{
	iClear();                      // Clear the screen
	if (gameOverPage == 1)         // If game over screen active
	{
		drawGameOverPage();        // Draw game over screen
	}
	else if (homePage == 1)        // If main menu active
	{
		drawHomePage();            // Draw main menu
	}
	else if (startPage == 1)       // If maze gameplay active
	{
		drawStartPage();           // Draw maze gameplay
	}
	else if (creditsPage == 1)     // If credits screen active
	{
		drawCreditsPage();         // Draw credits
	}
	else if (storyPage == 1)       // If story screen active
	{
		drawStoryPage();           // Draw story
	}
	else if (fightPage == 1)       // If fight scene active
	{
		drawFightPage();           // Draw fight scene
	}
}

void iMouseMove(int mx, int my)    // Mouse move callback (not used)
{
}

void iPassiveMouseMove(int mx, int my) // Passive mouse move callback (for hover detection)
{
	if (homePage == 1)             // On main menu
	{
		if (mx >= 213 && mx <= 435 && my >= 157 && my <= 204) // Hover over START button
		{
			hoverStart = true;
		}
		else
		{
			hoverStart = false;
		}
		if (mx >= 488 && mx <= 710 && my >= 157 && my <= 204) // Hover over CREDITS button
		{
			hoverCredits = true;
		}
		else
		{
			hoverCredits = false;
		}
		if (mx >= 765 && mx <= 990 && my >= 157 && my <= 204) // Hover over QUIT button
		{
			hoverQuit = true;
		}
		else
		{
			hoverQuit = false;
		}
	}
	else if (creditsPage == 1)     // On credits screen
	{
		if (mx >= 540 && mx <= 660 && my >= 35 && my <= 75) // Hover over BACK button
		{
			hoverBack = true;
		}
		else
		{
			hoverBack = false;
		}
	}
}

void iMouse(int button, int state, int mx, int my) // Mouse click callback
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // Left click down
	{
		if (homePage == 1 && hoverStart) // Click on START button
		{
			startButtonClickHandler();
		}
		else if (homePage == 1 && hoverCredits) // Click on CREDITS button
		{
			creditsButtonClickHandler();
		}
		else if (homePage == 1 && hoverQuit) // Click on QUIT button
		{
			exit(0);
		}
		else if (creditsPage == 1 && mx >= 540 && mx <= 660 && my >= 35 && my <= 75) // Click on BACK in credits
		{
			homePage = 1;
			creditsPage = 0;
		}
		else if (gameOverPage == 1) // Click anywhere on game over screen
		{
			gameOverPage = 0;
			homePage = 1;
			gameTime = INITIAL_TIME;
			gameOverSoundPlaying = false;
		}
	}
}

void countDown()                   // Timer function called every second
{
	if (startPage == 1 || fightPage == 1) // Countdown active during maze and fight
	{
		if (gameTime > 0)
		{
			gameTime--;            // Decrease remaining time
		}
		else
		{
			startPage = 0;         // End maze phase
			fightPage = 0;         // End fight phase
			gameOverPage = 1;      // Show game over
		}
	}
	if (isWin)                     // If player won the fight
	{
		winTimer++;
		if (winTimer >= 3)         // Delay before exit
		{
			exit(0);
		}
	}
}

void fixedUpdate()                 // Timer function called every 20ms (game logic update)
{
	setMusic(MUSIC_MAZE_BG);       // Attempt to play maze background (may be overridden)
	if (startPage == 1 && !mazeMusicPlaying) // Start maze creepy music when entering maze
	{
		stopSound();
		playLoop("Sounds\\mazeCreepy.wav");
		mazeMusicPlaying = true;
		fightMusicPlaying = false;
	}
	if (storyPage == 1 && !storyMusicPlaying) // Start story music when entering story
	{
		stopSound();
		playLoop("Sounds\\storyBg.wav");
		storyMusicPlaying = true;
		mazeMusicPlaying = false;
		fightMusicPlaying = false;
	}
	if (startPage == 1)            // Maze gameplay logic
	{
		int dx = 0;                // Horizontal movement delta
		int dy = 0;                // Vertical movement delta
		isMoving = false;          // Reset moving flag
		if (isKeyPressed('w') || isSpecialKeyPressed(GLUT_KEY_UP)) // Move up
		{
			dy++;
		}
		if (isKeyPressed('s') || isSpecialKeyPressed(GLUT_KEY_DOWN)) // Move down
		{
			dy--;
		}
		if (isKeyPressed('a') || isSpecialKeyPressed(GLUT_KEY_LEFT)) // Move left
		{
			dx--;
		}
		if (isKeyPressed('d') || isSpecialKeyPressed(GLUT_KEY_RIGHT)) // Move right
		{
			dx++;
		}
		if (dx != 0 || dy != 0)    // Player is pressing movement keys
		{
			isMoving = true;
		}
		if (isMoving && !walkSoundPlaying) // Start walking sound when movement begins
		{
			playLoop("Sounds\\walk.wav");
			walkSoundPlaying = true;
		}
		static bool wasMoving = false; // Track previous movement state
		if (isMoving && !wasMoving) // Transition from not moving to moving
		{
			stopSound();
			playLoop("Sounds\\walk.wav");
			walkSoundPlaying = true;
		}
		if (!isMoving && wasMoving) // Transition from moving to not moving
		{
			stopSound();
			if (startPage == 1)
			{
				playLoop("Sounds\\mazeCreepy.wav");
			}
			else if (fightPage == 1)
			{
				playLoop("Sounds\\fightBg.wav");
			}
			walkSoundPlaying = false;
		}
		wasMoving = isMoving;      // Update previous state
		int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1); // Direction step X
		int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1); // Direction step Y
		int paddingX = 12;         // Collision padding from left/right edges
		int paddingY = 5;          // Collision padding from top/bottom edges
		for (int i = 0; i < playerSpeed; i++) // Move pixel by pixel for accurate collision
		{
			int nextX = playerX + stepX;
			int nextY = playerY + stepY;
			if (isWalkable(nextX + paddingX, nextY + paddingY) && // Check four corners for collision
				isWalkable(nextX + charWidth - paddingX, nextY + paddingY) &&
				isWalkable(nextX + paddingX, nextY + charHeight - paddingY) &&
				isWalkable(nextX + charWidth - paddingX, nextY + charHeight - paddingY))
			{
				playerX = nextX;   // Update position if all corners are walkable
				playerY = nextY;
			}
			else
			{
				break;             // Stop moving in this direction if collision
			}
		}
		if (dx != 0 || dy != 0)    // Determine facing direction based on input
		{
			if (abs(dx) > abs(dy))
			{
				if (dx > 0)
				{
					playerDirection = 3; // Right
				}
				else
				{
					playerDirection = 2; // Left
				}
			}
			else
			{
				if (dy > 0)
				{
					playerDirection = 1; // Up
				}
				else
				{
					playerDirection = 0; // Down
				}
			}
		}
		if (isMoving)              // Advance walking animation
		{
			animTimer++;
			if (animTimer >= 5)
			{
				animTimer = 0;
				animIndex++;
				if (animIndex > 5)
				{
					animIndex = 1;
				}
			}
		}
		else
		{
			animIndex = 1;         // Reset to idle frame when not moving
		}
		int checkLeft = playerX + 12;   // Collision check points
		int checkRight = playerX + charWidth - 12;
		int checkBottom = playerY + 5;
		int checkTop = playerY + charHeight - 5;
		int tileL = checkLeft / TILE_SIZE;   // Tile indices for collision points
		int tileR = checkRight / TILE_SIZE;
		int tileB = checkBottom / TILE_SIZE;
		int tileT = checkTop / TILE_SIZE;
		if (maze[tileB][tileL] == 3 || maze[tileB][tileR] == 3 || maze[tileT][tileL] == 3 || maze[tileT][tileR] == 3) // Trap tile (3)
		{
			playerX = 60;          // Reset player to start position
			playerY = 60;
		}
		if (!storyFinished && storyPage == 0 && (maze[tileB][tileL] == 5 || maze[tileB][tileR] == 5 || maze[tileT][tileL] == 5 || maze[tileT][tileR] == 5)) // Story trigger tile (5)
		{
			startPage = 0;
			storyPage = 1;
			storyStep = 0;
			storyTimer = 0;
		}
		if (maze[tileB][tileL] == 4 || maze[tileB][tileR] == 4 || maze[tileT][tileL] == 4 || maze[tileT][tileR] == 4) // Exit tile (4)
		{
			startPage = 0;
			homePage = 1;
			playerX = 60;
			playerY = 60;
		}
	}
	if (fightPage == 1 && !isWin)  // Fight scene logic
	{
		if (!fightMusicPlaying)    // Start fight music
		{
			stopSound();
			playLoop("Sounds\\fightBg.wav");
			fightMusicPlaying = true;
			mazeMusicPlaying = false;
		}
		static int monsterAttackCooldown = 0; // Cooldown for monster attack
		static int monsterMoveTimer = 0;      // Timer for monster movement animation
		if (monsterAttackCooldown > 0)
		{
			monsterAttackCooldown--;
		}
		int dist = abs(fightPlayerX - monsterX); // Distance between player and monster
		if (dist > 120 && !monsterRunSoundPlaying) // Play run sound when far
		{
			stopSound();
			playLoop("Sounds\\monsterRun.wav");
			monsterRunSoundPlaying = true;
		}
		else if (dist <= 120 && monsterRunSoundPlaying) // Switch to fight music when close
		{
			stopSound();
			playLoop("Sounds\\fightBg.wav");
			monsterRunSoundPlaying = false;
		}
		monsterMoveTimer++;
		if (monsterMoveTimer >= 2) // Slow down monster movement
		{
			monsterMoveTimer = 0;
			if (dist > 40)         // Monster approaches player
			{
				if (monsterX > fightPlayerX + 80)
				{
					monsterX -= 1;
				}
				else if (monsterX < fightPlayerX - 80)
				{
					monsterX += 1;
				}
			}
		}
		if (abs(monsterX - fightPlayerX) < 120) // Monster in attack range
		{
			if (monsterAttackCooldown == 0 && monsterMoveIndex == 3) // Attack at specific animation frame
			{
				playOnce("Sounds\\monsterAttack.wav");
				playerHealth -= 8; // Deal damage to player
				if (playerHealth < 0)
				{
					playerHealth = 0;
				}
				damageFlashTimer = 10; // Trigger damage flash
				monsterAttackCooldown = 40; // Set attack cooldown
			}
		}
		static int fightAnimCounter = 0; // General animation counter
		fightAnimCounter++;
		if (fightAnimCounter >= 8)
		{
			fightAnimCounter = 0;
			if (fightAttackIndex > 0) // Advance attack animation
			{
				fightAttackIndex++;
				if (fightAttackIndex > 5)
				{
					fightAttackIndex = 0;
				}
			}
			monsterMoveIndex++;    // Advance monster animation
			if (monsterMoveIndex > 5)
			{
				monsterMoveIndex = 1;
			}
		}
		if ((isKeyPressed('d') || isKeyPressed('D')) && fightPlayerX < screen_width - 160) // Move right
		{
			if (dist > 120)
			{
				fightPlayerX += playerSpeed;
			}
			else
			{
				fightPlayerX += 1;
			}
			static int walkTimer = 0;
			walkTimer++;
			if (walkTimer >= playerSpeed)
			{
				fightWalkIndex++;
				if (fightWalkIndex > playerSpeed) fightWalkIndex = 1;
				walkTimer = 0;
			}
		}
		else if ((isKeyPressed('a') || isKeyPressed('A')) && fightPlayerX > 50) // Move left
		{
			fightPlayerX -= playerSpeed;
			static int walkTimer = 0;
			walkTimer++;
			if (walkTimer >= playerSpeed)
			{
				fightWalkIndex++;
				if (fightWalkIndex > 5) fightWalkIndex = 1;
				walkTimer = 0;
			}
		}
		if (attackCooldown > 0)    // Decrease attack cooldown
		{
			attackCooldown--;
		}
		if ((isKeyPressed('f') || isKeyPressed('F')) && fightAttackIndex == 0 && attackCooldown == 0) // Attack with F key
		{
			if (abs(fightPlayerX - monsterX) < 250) // In attack range
			{
				fightAttackIndex = 1;
				attackCooldown = 20;
				monsterHealth -= 15; // Deal damage to monster
				if (monsterHealth <= 0)
				{
					monsterHealth = 0;
					isWin = true;  // Player wins
				}
				if (playerHealth <= 0) // Check if player died during attack
				{
					playerHealth = 0;
					fightPage = 0;
					gameOverPage = 1;
					return;
				}
			}
		}
	}
	if (fightPage == 1 && playerHealth <= 30 && playerHealth > 0) // Low health heartbeat
	{
		if (!heartBeatPlaying)
		{
			stopSound();
			playLoop("Sounds\\heartBeat.wav");
			heartBeatPlaying = true;
		}
	}
	else
	{
		if (heartBeatPlaying)
		{
			stopSound();
			if (fightPage == 1)
			{
				playLoop("Sounds\\fightBg.wav");
			}
			heartBeatPlaying = false;
		}
	}
	if (fightPage == 1)            // Smooth health bar updates
	{
		if (displayPlayerHealth > playerHealth)
		{
			displayPlayerHealth -= 2;
			if (displayPlayerHealth < playerHealth)
			{
				displayPlayerHealth = playerHealth;
			}
		}
		if (displayMonsterHealth > monsterHealth)
		{
			displayMonsterHealth -= 1;
			if (displayMonsterHealth < monsterHealth)
			{
				displayMonsterHealth = monsterHealth;
			}
		}
	}
	if (damageFlashTimer > 0)      // Decrease damage flash timer
	{
		damageFlashTimer--;
	}
	if (storyPage == 1 && fightPage == 0 && startPage == 0) // Story sequence timer
	{
		storyTimer++;
		int timeLimit = 120;
		if (storyStep >= 6)
		{
			timeLimit = 200;
		}
		if (storyTimer >= timeLimit)
		{
			storyTimer = 0;
			storyStep++;
			if (storyStep > 15)
			{
				storyFinished = true;
				storyPage = 0;
				fightPage = 1;
				attackCooldown = 0;
				stopSound();
				storyMusicPlaying = false;
				fightMusicPlaying = false;
				monsterRunSoundPlaying = false;
				heartBeatPlaying = false;
				fightPlayerX = 157;
				monsterX = 765;
				playerHealth = 100;
				monsterHealth = 100;
				displayPlayerHealth = 100;
				displayMonsterHealth = 100;
			}
		}
	}
	if (gameOverPage == 1 && !gameOverSoundPlaying) // Play game over sound once
	{
		stopSound();
		playOnce("Sounds\\gameOver.wav");
		gameOverSoundPlaying = true;
		mazeMusicPlaying = false;
		fightMusicPlaying = false;
		storyMusicPlaying = false;
		monsterRunSoundPlaying = false;
		walkSoundPlaying = false;
		heartBeatPlaying = false;
	}
}

void drawGameOverPage()            // Draw game over screen
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	iSetColor(255, 0, 0);
	iText(screen_width / 2 - 100, screen_height / 2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(screen_width / 2 - 100, screen_height / 2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(screen_width / 2 - 100, screen_height / 2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(screen_width / 2 - 100, screen_height / 2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(255, 255, 255);
	iText(screen_width / 2 - 150, screen_height / 2 - 50, "Click to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
	iText(screen_width / 2 - 150, screen_height / 2 - 50, "Click to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
}

void drawHomePage()                // Draw main menu screen
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	iShowBMP(0, 0, "Images\\interface.bmp");
	if (hoverStart)
	{
		iSetColor(255, 0, 0);
		iText(287, 173, "START", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(288, 173, "START", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(287, 173, "START", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(288, 173, "START", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(186, 94, "Images\\button1.bmp", 0);
		iSetColor(235, 205, 150);
		iText(287, 173, "START", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(288, 173, "START", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (hoverCredits)
	{
		iSetColor(255, 0, 0);
		iText(550, 173, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(551, 173, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(550, 173, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(551, 173, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(471, 89, "Images\\button2.bmp", 0);
		iSetColor(220, 215, 200);
		iText(550, 173, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(551, 173, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (hoverQuit)
	{
		iSetColor(255, 0, 0);
		iText(850, 173, "QUIT", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(851, 173, "QUIT", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(850, 173, "QUIT", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(851, 173, "QUIT", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(753, 83, "Images\\button3.bmp", 0);
		iSetColor(230, 225, 210);
		iText(850, 173, "QUIT", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(851, 173, "QUIT", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void drawTileDetail(int r, int c, int x, int y, int type) // Draw detailed appearance of a single tile
{
	if (type == 1)                 // Wall tile
	{
		iSetColor(0, 0, 0);
		iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
		iSetColor(150, 0, 0);
		if (c % 3 == 0)
		{
			iLine(x + 10, y + 50, x + 10, y + 20);
			iFilledCircle(x + 10, y + 20, 2);
		}
		if (r % 2 == 0)
		{
			iLine(x + 35, y + 50, x + 35, y + 35);
			iSetColor(20, 0, 0);
			iLine(x, y + 10, x + 50, y + 15);
			iLine(x + 20, y, x + 30, y + 50);
		}
	}
	else if (type == 3)            // Trap tile
	{
		iSetColor(25, 20, 15);
		iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
		iSetColor(40, 40, 40);
		iFilledRectangle(x + 5, y + 5, 40, 10);
		iSetColor(120, 120, 120);
		double b1x[] = { (double)x + 8, (double)x + 15, (double)x + 22 };
		double b1y[] = { (double)y + 15, (double)y + 45, (double)y + 15 };
		iFilledPolygon(b1x, b1y, 3);
		iSetColor(180, 0, 0);
		iFilledCircle(x + 15, y + 45, 2);
	}
	else if (type == 0 || type == 5) // Floor or story trigger
	{
		iSetColor(25, 20, 15);
		iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
		if (type == 5)
		{
			iShowBMP2(x, y, "Images\\monster1.bmp", 0);
		}
		if ((r + c) % 7 == 0)
		{
			iSetColor(100, 0, 0);
			iFilledEllipse(x + 25, y + 25, 15, 8);
		}
		if ((r * c) % 11 == 2 && type == 0)
		{
			iSetColor(150, 0, 0);
			iFilledCircle(x + 10, y + 10, 4);
			iSetColor(210, 170, 150);
			iFilledRectangle(x + 12, y + 10, 15, 6);
		}
	}
	else if (type == 2 || type == 4) // Exit area or exit
	{
		iSetColor(0, 0, 0);
		iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
		if (type == 2)
		{
			iSetColor(80, 40, 20);
			iFilledRectangle(x + 10, y, 5, 50);
		}
		else if (type == 4)
		{
			iSetColor(50, 30, 20);
			iFilledCircle(x + 25, y + 28, 6);
			iSetColor(70, 70, 70);
			for (int i = 8; i < 50; i += 12)
			{
				iFilledRectangle(x + i, y, 3, 50);
			}
		}
	}
}

void drawStartPage()               // Draw maze gameplay screen
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	int pCX = playerX + (charWidth / 2); // Player center X
	int pCY = playerY + (charHeight / 2); // Player center Y
	for (int r = 0; r < ROWS; r++) // Draw only tiles within light radius
	{
		for (int c = 0; c < COLS; c++)
		{
			int x = c * TILE_SIZE;
			int y = r * TILE_SIZE;
			if (sqrt(pow((double)x + 25 - pCX, 2) + pow((double)y + 25 - pCY, 2)) < LIGHT_RADIUS)
			{
				drawTileDetail(r, c, x, y, maze[r][c]);
			}
		}
	}
	char path[100];
	sprintf(path, "Images\\%s%d.bmp", animNames[playerDirection], animIndex); // Build player sprite path
	iShowBMP2(playerX, playerY, path, 0); // Draw player sprite
	iSetColor(255, 255, 255);
	sprintf(timeStr, "Time: %d", gameTime); // Format time string
	iText(1050, 650, timeStr, GLUT_BITMAP_TIMES_ROMAN_24); // Display time
}

void drawCreditsPage()             // Draw credits screen
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	iShowBMP(0, 0, "Images\\credits.bmp");
	if (hoverBack)
	{
		iSetColor(255, 0, 0);
		iText(569, 47, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(569, 47, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(569, 47, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(570, 47, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		iShowBMP2(526, 10, "Images\\button4.bmp", 0);
		iSetColor(235, 205, 150);
		iText(569, 47, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(569, 47, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void drawStoryPage()               // Draw story/cutscene screen
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, 1200, 700);
	if (storyStep < 6)             // Text-based story steps
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
		iText(startX, startY, text, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(startX + 1, startY, text, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(startX, startY + 1, text, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(startX + 1, startY + 1, text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (storyStep >= 6 && storyStep <= 15) // Image-based story steps
	{
		char file[50];
		sprintf(file, "Images\\story%d.bmp", storyStep - 5);
		iShowBMP(0, 0, file);
	}
}

void drawFightPage()               // Draw fight scene
{
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screen_width, screen_height);
	iShowBMP(0, 0, "Images\\fight1.bmp");
	if (damageFlashTimer > 0 && damageFlashTimer % 2 == 0) // Red flash on damage
	{
		iSetColor(255, 0, 0);
		iFilledRectangle(0, 0, screen_width, screen_height);
	}
	if (isWin)                     // Victory message
	{
		iSetColor(0, 255, 0);
		iText(400, 350, "I SAVED YOU, LAMIS!", GLUT_BITMAP_TIMES_ROMAN_24);
		return;
	}
	iSetColor(50, 0, 0);           // Health bar backgrounds
	iFilledRectangle(50, 600, 200, 20);
	iFilledRectangle(950, 600, 200, 20);
	iSetColor(255, 0, 0);          // Health bar fills
	int hp = displayPlayerHealth;
	if (hp < 0)
	{
		hp = 0;
	}
	if (hp > 100)
	{
		hp = 100;
	}
	iFilledRectangle(50, 600, hp * 2, 20);
	int mhp = displayMonsterHealth;
	if (mhp < 0)
	{
		mhp = 0;
	}
	if (mhp > 100)
	{
		mhp = 100;
	}
	iFilledRectangle(950, 600, mhp * 2, 20);
	iSetColor(255, 255, 255);      // Health bar labels
	iText(50, 630, "PLAYER HEALTH", GLUT_BITMAP_HELVETICA_18);
	iText(950, 630, "MONSTER HEALTH", GLUT_BITMAP_HELVETICA_18);
	char pFile[100];
	if (fightAttackIndex > 0)      // Choose attack or walk sprite
	{
		sprintf(pFile, "Images\\fightMove%d.bmp", fightAttackIndex);
	}
	else
	{
		sprintf(pFile, "Images\\fightWalk%d.bmp", fightWalkIndex);
	}
	iShowBMP2(fightPlayerX, fightPlayerY, pFile, 0);
	char mFile[100];
	sprintf(mFile, "Images\\monster1move%d.bmp", monsterMoveIndex);
	iShowBMP2(monsterX, monsterY, mFile, 0);
}

void startButtonClickHandler()     // Handler for START button
{
	homePage = 0;
	startPage = 1;
	playerX = 60;
	playerY = 60;
	playerDirection = 0;
	gameTime = INITIAL_TIME;
	playerHealth = 100;
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

void creditsButtonClickHandler()   // Handler for CREDITS button
{
	homePage = 0;
	creditsPage = 1;
}

int main()                         // Program entry point
{
	srand((unsigned int)time(NULL)); // Seed random number generator
	iInitialize(screen_width, screen_height, "Last Light of hope"); // Initialize iGraphics window
	iSetTimer(1000, countDown);    // Set 1-second timer for countdown
	iSetTimer(20, fixedUpdate);    // Set 20ms timer for game logic
	iStart();                      // Start iGraphics main loop
	return 0;
}
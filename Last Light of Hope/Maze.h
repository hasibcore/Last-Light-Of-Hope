#ifndef MAZE_H
#define MAZE_H

#include "Variables.h"

// Maze navigation and drawing functions
inline bool isWalkable(int x, int y) {
  int c = x / TILE_SIZE;
  int r = y / TILE_SIZE;
  if (r < 0 || r >= ROWS || c < 0 || c >= COLS)
    return false;

  if (maze[r][c] == 1)
    return false;      // Wall
  if (maze[r][c] == 4) // Finish Gate
    return true;
  if (maze[r][c] == 5) // Boss Gate
  {
    if (currentLevel == 1 && (lv1P1Solved && lv1P2Solved))
      return true;
    if (currentLevel == 2 &&
        (lv2P1Solved && lv2P2Solved && lv2P3Solved && lv2P4Solved))
      return true;
    if (currentLevel == 3 && (lv3P1Solved && lv3P2Solved && lv3P3Solved &&
                              lv3P4Solved && lv3P5Solved && lv3P6Solved))
      return true;
    return false;
  }
  return true;
}

inline void drawTileDetail(int r, int c, int x, int y, int type) {
  if (type == 1) // Wall tile
  {
    iSetColor(0, 0, 0);
    iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
    iSetColor(150, 0, 0);
    if (c % 3 == 0) {
      iLine(x + 10, y + 50, x + 10, y + 20);
      iFilledCircle(x + 10, y + 20, 2);
    }
    if (r % 2 == 0) {
      iLine(x + 35, y + 50, x + 35, y + 35);
      iSetColor(20, 0, 0);
      iLine(x, y + 10, x + 50, y + 15);
      iLine(x + 20, y, x + 30, y + 50);
    }
  } else if (type == 3) // Trap tile
  {
    iSetColor(25, 20, 15);
    iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
    iSetColor(40, 40, 40);
    iFilledRectangle(x + 5, y + 5, 40, 10);
    iSetColor(120, 120, 120);
    double b1x[] = {(double)x + 8, (double)x + 15, (double)x + 22};
    double b1y[] = {(double)y + 15, (double)y + 45, (double)y + 15};
    iFilledPolygon(b1x, b1y, 3);
    iSetColor(180, 0, 0);
    iFilledCircle(x + 15, y + 45, 2);
  } else if (type == 0 || type == 5) // Floor or story trigger
  {
    iSetColor(25, 20, 15);
    iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
    if (type == 5) {
      bool alreadyCompleted = (currentLevel == 1 && level1Finished) ||
                              (currentLevel == 2 && level2Finished) ||
                              (currentLevel == 3 && level3Finished);
      bool puzzlesSolved = false;
      if (currentLevel == 1)
        puzzlesSolved = (lv1P1Solved && lv1P2Solved);
      else if (currentLevel == 2)
        puzzlesSolved =
            (lv2P1Solved && lv2P2Solved && lv2P3Solved && lv2P4Solved);
      else if (currentLevel == 3)
        puzzlesSolved = (lv3P1Solved && lv3P2Solved && lv3P3Solved &&
                         lv3P4Solved && lv3P5Solved && lv3P6Solved);

      if (puzzlesSolved && !alreadyCompleted) {
        char mPath[100];
        sprintf(mPath, "Images\\monster%d.bmp", currentLevel);
        iShowBMP2(x, y, mPath, 0);
      }
    }
    if ((r + c) % 7 == 0) {
      iSetColor(100, 0, 0);
      iFilledEllipse(x + 25, y + 25, 15, 8);
    }
    if ((r * c) % 11 == 2 && type == 0) {
      iSetColor(150, 0, 0);
      iFilledCircle(x + 10, y + 10, 4);
      iSetColor(210, 170, 150);
      iFilledRectangle(x + 12, y + 10, 15, 6);
    }
  } else if (type == 2 || type == 4) // Exit area or exit
  {
    iSetColor(0, 0, 0);
    iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
    if (type == 2) {
      iSetColor(80, 40, 20);
      iFilledRectangle(x + 10, y, 5, 50);
    } else if (type == 4) {
      iSetColor(50, 30, 20);
      iFilledCircle(x + 25, y + 28, 6);
      iSetColor(70, 70, 70);
      for (int i = 8; i < 50; i += 12) {
        iFilledRectangle(x + i, y, 3, 50);
      }
    }
  } else if (type >= 6 && type <= 11) // All puzzle types
  {
    iSetColor(25, 20, 15);
    iFilledRectangle(x, y, TILE_SIZE, TILE_SIZE);
    bool solved = false;
    int pIdx = 0;
    if (currentLevel == 1) {
      if (type == 6) {
        solved = lv1P1Solved;
        pIdx = 1;
      } else if (type == 7) {
        solved = lv1P2Solved;
        pIdx = 2;
      }
    } else if (currentLevel == 2) {
      if (type == 6) {
        solved = lv2P1Solved;
        pIdx = 1;
      } else if (type == 7) {
        solved = lv2P2Solved;
        pIdx = 2;
      } else if (type == 8) {
        solved = lv2P3Solved;
        pIdx = 3;
      } else if (type == 9) {
        solved = lv2P4Solved;
        pIdx = 4;
      }
    } else if (currentLevel == 3) {
      if (type == 6) {
        solved = lv3P1Solved;
        pIdx = 1;
      } else if (type == 7) {
        solved = lv3P2Solved;
        pIdx = 2;
      } else if (type == 8) {
        solved = lv3P3Solved;
        pIdx = 3;
      } else if (type == 9) {
        solved = lv3P4Solved;
        pIdx = 4;
      } else if (type == 10) {
        solved = lv3P5Solved;
        pIdx = 5;
      } else if (type == 11) {
        solved = lv3P6Solved;
        pIdx = 6;
      }
    }

    if (!solved && activePuzzle != pIdx) {
      if (type == 6)
        iSetColor(0, 200, 255);
      else if (type == 7)
        iSetColor(255, 200, 0);
      else if (type == 8)
        iSetColor(255, 100, 0);
      else if (type == 9)
        iSetColor(200, 0, 255);
      else if (type == 10)
        iSetColor(0, 255, 100);
      else if (type == 11)
        iSetColor(255, 0, 100);
      iRectangle(x + 5, y + 5, TILE_SIZE - 10, TILE_SIZE - 10);
      iSetColor(255, 255, 255);

      const char *sym = "?";
      if (type == 7)
        sym = "!";
      else if (type == 8)
        sym = "*";
      else if (type == 9)
        sym = "#";
      else if (type == 10)
        sym = "$";
      else if (type == 11)
        sym = "@";
      iText(x + 18, y + 15, (char *)sym, GLUT_BITMAP_TIMES_ROMAN_24);
    }
  }
}

// Helper to draw clues without lambda for compatibility
// Helper to draw clues without lambda for compatibility
inline void drawClueInternal(int uiBoxX, int uiBoxY, bool solved, int yOff,
                             const char *name) {
  iSetColor(255, 255, 255);
  char buf[64];
  if (!solved) {
    sprintf(buf, "- %s clue: [LOCKED]", name);
  } else {
    sprintf(buf, "- %s clue: [DONE]", name);
  }
  iText(uiBoxX + 15, uiBoxY + yOff, buf, GLUT_BITMAP_HELVETICA_12);
}

inline void drawMazeUI() {
  iSetColor(50, 50, 50);
  iFilledRectangle(20, 620, 200, 25);
  iSetColor(255, 0, 0);
  iFilledRectangle(20, 620, (double)playerHealth * 2, 25);
  iSetColor(255, 255, 255);
  iRectangle(20, 620, 200, 25);
  iText(20, 650, (char *)"HEALTH", GLUT_BITMAP_HELVETICA_12);

  if ((currentLevel == 1 && level1Finished) ||
      (currentLevel == 2 && level2Finished) ||
      (currentLevel == 3 && level3Finished))
    return;

  int uiBoxX = (playerX > 600) ? 20 : 950;
  int uiBoxY = 20;
  int boxHeight = (currentLevel == 3) ? 200 : 150;

  iSetColor(20, 20, 20);
  iFilledRectangle(uiBoxX, uiBoxY, 230, boxHeight);
  iSetColor(255, 255, 255);
  iRectangle(uiBoxX, uiBoxY, 230, boxHeight);
  iText(uiBoxX + 15, uiBoxY + (boxHeight - 30),
        (char *)"MISSIONS:", GLUT_BITMAP_HELVETICA_18);

  if (currentLevel == 1) {
    drawClueInternal(uiBoxX, uiBoxY, lv1P1Solved, 90, "first");
    drawClueInternal(uiBoxX, uiBoxY, lv1P2Solved, 70, "second");
  } else if (currentLevel == 2) {
    drawClueInternal(uiBoxX, uiBoxY, lv2P1Solved, 90, "first");
    drawClueInternal(uiBoxX, uiBoxY, lv2P2Solved, 70, "second");
    drawClueInternal(uiBoxX, uiBoxY, lv2P3Solved, 50, "third");
    drawClueInternal(uiBoxX, uiBoxY, lv2P4Solved, 30, "final");
  } else if (currentLevel == 3) {
    drawClueInternal(uiBoxX, uiBoxY, lv3P1Solved, 140, "first");
    drawClueInternal(uiBoxX, uiBoxY, lv3P2Solved, 120, "second");
    drawClueInternal(uiBoxX, uiBoxY, lv3P3Solved, 100, "third");
    drawClueInternal(uiBoxX, uiBoxY, lv3P4Solved, 80, "fourth");
    drawClueInternal(uiBoxX, uiBoxY, lv3P5Solved, 60, "fifth");
    drawClueInternal(uiBoxX, uiBoxY, lv3P6Solved, 40, "final");
  }

  iSetColor(255, 255, 255);
  bool allDone = false;
  if (currentLevel == 1)
    allDone = (lv1P1Solved && lv1P2Solved);
  else if (currentLevel == 2)
    allDone = (lv2P1Solved && lv2P2Solved && lv2P3Solved && lv2P4Solved);
  else if (currentLevel == 3)
    allDone = (lv3P1Solved && lv3P2Solved && lv3P3Solved && lv3P4Solved &&
               lv3P5Solved && lv3P6Solved);

  if (allDone)
    iText(uiBoxX + 15, uiBoxY + 10, (char *)"- Confront the Darkness",
          GLUT_BITMAP_HELVETICA_12);
  else
    iText(uiBoxX + 15, uiBoxY + 10, (char *)"- ??? LOCKED ???",
          GLUT_BITMAP_HELVETICA_12);

  // Draw Timer
  bool currentLevelDone = (currentLevel == 1 && level1Finished) ||
                          (currentLevel == 2 && level2Finished) ||
                          (currentLevel == 3 && level3Finished);
  if (!currentLevelDone) {
    iSetColor(255, 255, 255);
    char tbuf[64];
    sprintf(tbuf, "Time: %d", gameTime);
    int timeWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24,
                                     (const unsigned char *)tbuf);
    iText(1185 - timeWidth, 650, (char *)tbuf, GLUT_BITMAP_TIMES_ROMAN_24);
  }

  // Draw HP gain popup
  if (hpGainTimer > 0) {
    iSetColor(0, 255, 0);
    char hpbuf[16];
    sprintf(hpbuf, "+%d HP", hpGainValue);
    int floatY = (100 - hpGainTimer);
    iText(playerX, playerY + charHeight + 10 + floatY, (char *)hpbuf,
          GLUT_BITMAP_HELVETICA_18);
  }
}

// Helper to draw level selection boxes without lambda
inline void drawLvlBoxInternal(int x, int y, bool unlocked, bool hovered,
                               const char *name, const char *diff, int r, int g,
                               int b) {
  if (unlocked) {
    if (hovered)
      iSetColor(0, 255, 255);
    else
      iSetColor(r, g, b);
  } else
    iSetColor(50, 50, 50);
  iFilledRectangle(x, y, 300, 200);
  iSetColor(255, 255, 255);
  if (hovered && unlocked)
    iRectangle(x - 2, y - 2, 304, 204);
  iRectangle(x, y, 300, 200);
  iText(x + 50, y + 150, (char *)name, GLUT_BITMAP_HELVETICA_18);
  iText(x + 30, y + 100, (char *)diff, GLUT_BITMAP_HELVETICA_12);
  if (!unlocked)
    iText(x + 70, y + 30, (char *)"[LOCKED]", GLUT_BITMAP_HELVETICA_18);
}

inline void drawLevelSelectionPage() {
  iSetColor(0, 0, 0);
  iFilledRectangle(0, 0, 1200, 800);
  iSetColor(255, 255, 255);
  iText(450, 650, (char *)"SELECT YOUR DESTINATION",
        GLUT_BITMAP_TIMES_ROMAN_24);

  drawLvlBoxInternal(100, 250, true, hoverLvl1, "MAZE 1: THE HUT",
                     "DIFFICULTY: EASY", 50, 100, 50);
  drawLvlBoxInternal(450, 250, (unlockedLevel >= 2), hoverLvl2,
                     "MAZE 2: SCHOOL", "DIFFICULTY: MEDIUM", 100, 100, 50);
  drawLvlBoxInternal(800, 250, (unlockedLevel >= 3), hoverLvl3,
                     "MAZE 3: FACTORY", "DIFFICULTY: HARD", 100, 50, 50);
}

inline void loadLevelMaze(int level) {
  static int template1[ROWS][COLS] = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 0, 2, 2, 4, 1},
      {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 2, 2, 2, 1},
      {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
      {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0, 0, 0, 0, 1, 3, 1},
      {1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
      {1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
      {1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
      {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 1},
      {1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

  static int template2[ROWS][COLS] = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 2, 2, 4, 1},
      {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 2, 2, 2, 1},
      {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
      {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 5, 1},
      {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 6, 0, 0, 0, 0, 1, 0, 1},
      {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 9, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
      {1, 0, 0, 0, 0, 7, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
      {1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
      {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 8, 0, 1},
      {1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

  static int template3[ROWS][COLS] = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 8, 0, 0, 1, 0, 1, 0, 1, 0, 2, 2, 4, 1},
      {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2, 2, 2, 1},
      {1, 6, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
      {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 5, 1},
      {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1, 0, 1},
      {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
      {1, 0, 0, 0, 0, 9, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
      {1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
      {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 11, 0, 1},
      {1, 1, 1, 1, 0, 10, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int val = 0;
      if (level == 1)
        val = template1[r][c];
      else if (level == 2)
        val = template2[r][c];
      else if (level == 3)
        val = template3[r][c];
      maze[r][c] = val;

      bool cleared = (level == 1 && level1Finished) ||
                     (level == 2 && level2Finished) ||
                     (level == 3 && level3Finished);
      if (cleared && (maze[r][c] >= 5 && maze[r][c] <= 11))
        maze[r][c] = 0;

      if (maze[r][c] == 6) {
        puzzle1R = r;
        puzzle1C = c;
      }
      if (maze[r][c] == 7) {
        puzzle2R = r;
        puzzle2C = c;
      }
      if (maze[r][c] == 8) {
        puzzle3R = r;
        puzzle3C = c;
      }
      if (maze[r][c] == 9) {
        puzzle4R = r;
        puzzle4C = c;
      }
      if (maze[r][c] == 10) {
        puzzle5R = r;
        puzzle5C = c;
      }
      if (maze[r][c] == 11) {
        puzzle6R = r;
        puzzle6C = c;
      }
    }
  }
}

inline void drawStartPage() {
  iSetColor(0, 0, 0);
  iFilledRectangle(0, 0, screen_width, screen_height);
  int pCX = playerX + (charWidth / 2);
  int pCY = playerY + (charHeight / 2);
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int x = c * TILE_SIZE;
      int y = r * TILE_SIZE;
      if (sqrt(pow((double)x + 25 - pCX, 2.0) +
               pow((double)y + 25 - pCY, 2.0)) < LIGHT_RADIUS) {
        drawTileDetail(r, c, x, y, maze[r][c]);
      }
    }
  }
  char path[100];
  sprintf(path, "Images\\%s%d.bmp", animNames[playerDirection], animIndex);
  iShowBMP2(playerX, playerY, path, 0);

  // Timer and HP gain are drawn here, Missions are in drawMazeUI()
  drawMazeUI();
}

inline void drawPuzzleOverlay() {
  if (activePuzzle == 0)
    return;
  // Box size and centering
  int bw = 800;
  int bh = 250;
  int bx = (screen_width - bw) / 2;
  int by = (screen_height - bh) / 2;

  iSetColor(0, 0, 0);
  iFilledRectangle(bx, by, bw, bh);
  iSetColor(255, 255, 255);
  iRectangle(bx, by, bw, bh);

  int textX = bx + 40;
  int titleY = by + bh - 40;
  int riddleY = by + bh - 80;

  if (activePuzzle == 1) {
    iText(textX + 20, titleY, (char *)"PUZZLE 1: THE RIDDLE",
          GLUT_BITMAP_HELVETICA_18);
    if (currentLevel == 1) {
      iText(textX, riddleY, (char *)"I follow you in the light,",
            GLUT_BITMAP_TIMES_ROMAN_24);
      iText(textX, riddleY - 30, (char *)"But hide when it's dark.",
            GLUT_BITMAP_TIMES_ROMAN_24);
    } else if (currentLevel == 2) {
      iText(textX, riddleY, (char *)"What word is always",
            GLUT_BITMAP_TIMES_ROMAN_24);
      iText(textX, riddleY - 30, (char *)"spelled 'wrong'?",
            GLUT_BITMAP_TIMES_ROMAN_24);
    } else { // Level 3
      iText(textX, riddleY,
            (char *)"I have cities, but no houses; forests, but no trees;",
            GLUT_BITMAP_TIMES_ROMAN_24);
      iText(textX, riddleY - 30, (char *)"and water, but no fish. What am I?",
            GLUT_BITMAP_TIMES_ROMAN_24);
    }
  } else if (activePuzzle == 2) {
    iText(textX + 20, titleY, (char *)"PUZZLE 2: THE ANAGRAM/LOGIC",
          GLUT_BITMAP_HELVETICA_18);
    if (currentLevel == 1) {
      iText(textX, riddleY, (char *)"Unscramble: ETH ILTGH",
            GLUT_BITMAP_TIMES_ROMAN_24);
    } else if (currentLevel == 2) {
      iText(textX, riddleY, (char *)"I am the line between",
            GLUT_BITMAP_TIMES_ROMAN_24);
      iText(textX, riddleY - 30, (char *)"earth and sky.",
            GLUT_BITMAP_TIMES_ROMAN_24);
    } else { // Level 3
      iText(textX, riddleY, (char *)"(5 * 5) - (9 * 2) = ?",
            GLUT_BITMAP_TIMES_ROMAN_24);
    }
  } else if (activePuzzle == 3) {
    iText(textX + 20, titleY, (char *)"PUZZLE 3: THE CIPHER",
          GLUT_BITMAP_HELVETICA_18);
    if (currentLevel == 2)
      iText(textX, riddleY, (char *)"Reverse: SYAWLA",
            GLUT_BITMAP_TIMES_ROMAN_24);
    else
      iText(textX, riddleY, (char *)"Voltage check: IX + X = ?",
            GLUT_BITMAP_TIMES_ROMAN_24);
  } else if (activePuzzle == 4) {
    iText(textX + 20, titleY, (char *)"PUZZLE 4: LOGIC",
          GLUT_BITMAP_HELVETICA_18);
    if (currentLevel == 2) {
      iText(textX, riddleY, (char *)"If 1=5, 2=25, 3=125,",
            GLUT_BITMAP_TIMES_ROMAN_24);
      iText(textX, riddleY - 30, (char *)"then 5 = ?",
            GLUT_BITMAP_TIMES_ROMAN_24);
    } else { // Level 3
      iText(
          textX, riddleY,
          (char *)"Who's the legendary wizard teaching in the Game Dev course?",
          GLUT_BITMAP_TIMES_ROMAN_24);
      iText(textX, riddleY - 30,
            (char *)"Choose wisely, your GPA (and life) depends on it :)!",
            GLUT_BITMAP_TIMES_ROMAN_24);
    }
  } else if (activePuzzle == 5) {
    iText(textX + 20, titleY, (char *)"PUZZLE 5: SEQUENCE",
          GLUT_BITMAP_HELVETICA_18);
    iText(textX, riddleY, (char *)"2, 4, 8, 16, ?", GLUT_BITMAP_TIMES_ROMAN_24);
  } else if (activePuzzle == 6) {
    iText(textX + 20, titleY, (char *)"PUZZLE 6: RIDDLE",
          GLUT_BITMAP_HELVETICA_18);
    iText(textX, riddleY, (char *)"Has a head and a tail",
          GLUT_BITMAP_TIMES_ROMAN_24);
    iText(textX, riddleY - 30, (char *)"but no body?",
          GLUT_BITMAP_TIMES_ROMAN_24);
  }

  // Draw input box
  iSetColor(100, 100, 100);
  iFilledRectangle(bx + 100, by + 40, bw - 200, 40);
  iSetColor(255, 255, 255);
  iText(bx + 110, by + 55, puzzleInput, GLUT_BITMAP_HELVETICA_18);
  iText(bx + 100, by + 10, (char *)"Press ENTER to solve",
        GLUT_BITMAP_HELVETICA_12);
}

inline void drawPauseOverlay() {
  extern bool hoverPauseLeave;
  extern double pauseScale;

  // Background dimming
  iSetColor(0, 0, 0);
  iFilledRectangle(0, 0, screen_width, screen_height);

  // Animated dimensions
  int baseW = 450;
  int baseH = 350;
  double bw = baseW * pauseScale;
  double bh = baseH * pauseScale;
  double bx = (double)(screen_width - bw) / 2.0;
  double by = (double)(screen_height - bh) / 2.0;

  // Only draw content if it's large enough
  if (pauseScale > 0.1) {
    iSetColor(30, 30, 30);
    iFilledRectangle(bx, by, bw, bh);
    iSetColor(255, 255, 255);
    iRectangle(bx, by, bw, bh);
    iRectangle(bx + 5 * pauseScale, by + 5 * pauseScale, bw - 10 * pauseScale,
               bh - 10 * pauseScale);

    if (pauseScale > 0.8) {
      // Pause Title
      iSetColor(255, 255, 255);
      char *pauseText = (char *)"G A M E   P A U S E D";
      int tw = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24,
                                (const unsigned char *)pauseText);
      iText(bx + (double)(bw - tw) / 2.0, by + (double)bh - 70.0,
            (char *)pauseText, GLUT_BITMAP_TIMES_ROMAN_24);

      // Leave Game Button
      double btnX = bx + 50.0 * pauseScale;
      double btnY = by + 80.0 * pauseScale;
      double btnW = bw - 100.0 * pauseScale;
      double btnH = 90.0 * pauseScale;

      if (hoverPauseLeave) {
        iSetColor(200, 0, 0); // Animated red glow
        iFilledRectangle(btnX, btnY, btnW, btnH);
        iSetColor(255, 255, 255);
        iRectangle(btnX, btnY, btnW, btnH);
      } else {
        iSetColor(60, 60, 60);
        iFilledRectangle(btnX, btnY, btnW, btnH);
        iSetColor(150, 150, 150);
        iRectangle(btnX, btnY, btnW, btnH);
      }

      iSetColor(255, 255, 255);
      char *leaveText = (char *)"LEAVE GAME";
      int lw = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                (const unsigned char *)leaveText);
      iText(bx + (double)(bw - lw) / 2.0,
            (double)btnY + (double)btnH / 2.0 - 8.0, (char *)leaveText,
            GLUT_BITMAP_HELVETICA_18);

      // Footer hint
      iSetColor(180, 180, 180);
      char *hintText = (char *)"Press ESC to resume your journey";
      int hw = glutBitmapLength(GLUT_BITMAP_HELVETICA_12,
                                (const unsigned char *)hintText);
      iText(bx + (double)(bw - hw) / 2.0, by + 40.0 * pauseScale,
            (char *)hintText, GLUT_BITMAP_HELVETICA_12);
    }
  }
}

#endif // MAZE_H

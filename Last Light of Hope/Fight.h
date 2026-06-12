#ifndef FIGHT_H
#define FIGHT_H

#include "Variables.h"

// Fight scene rendering logic
inline void drawFightPage() {
  iSetColor(0, 0, 0);
  iFilledRectangle(0, 0, screen_width, screen_height);
  char bgFile[100];
  sprintf(bgFile, "Images\\fight%d.bmp", currentLevel);
  iShowBMP(0, 0, bgFile);
  if (damageFlashTimer > 0 && damageFlashTimer % 2 == 0) {
    iSetColor(255, 0, 0);
    iFilledRectangle(0, 0, screen_width, screen_height);
  }
  if (isWin) {
    // SPECIAL FINALE: Game Completed Screen
    if (currentLevel == 3) {
      // celebratory backdrop
      iSetColor(0, 0, 0);
      iFilledRectangle(screen_width / 2 - 350, screen_height / 2 - 180, 700,
                       360);
      iSetColor(255, 215, 0); // Gold border
      iRectangle(screen_width / 2 - 355, screen_height / 2 - 185, 710, 370);

      // Finale Text
      iSetColor(255, 255, 255);
      char gcText[] = "GAME COMPLETED";
      int gcw = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24,
                                 (const unsigned char *)gcText);
      iText(screen_width / 2 - gcw / 2, screen_height / 2 + 100, gcText,
            GLUT_BITMAP_TIMES_ROMAN_24);

      iSetColor(200, 200, 200);
      char thanksText[] = "Thanks for playing!";
      int tw = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                (const unsigned char *)thanksText);
      iText(screen_width / 2 - tw / 2, screen_height / 2 + 50, thanksText,
            GLUT_BITMAP_HELVETICA_18);

      // START NEW GAME Button
      int bx = screen_width / 2 - 200;
      int by = screen_height / 2 - 70;
      int bw = 400;
      int bh = 80;

      if (hoverNewGame) {
        int pulse = 30 + 30 * sin(winTimer * 0.1);
        iSetColor(100 + pulse, 80 + pulse, 0);
        iFilledRectangle(bx, by, bw, bh);
        iSetColor(255, 255, 255);
      } else {
        iSetColor(50, 40, 0);
        iFilledRectangle(bx, by, bw, bh);
        iSetColor(218, 165, 32); // Gold
      }
      iRectangle(bx, by, bw, bh);

      char btText[] = "START NEW GAME";
      int btw = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24,
                                 (const unsigned char *)btText);
      iText(screen_width / 2 - btw / 2, by + 30, btText,
            GLUT_BITMAP_TIMES_ROMAN_24);

      return;
    }

    // STANDARD POPUP: Maze 1 and 2
    // Dark overlay
    iSetColor(0, 0, 0);
    iFilledRectangle(screen_width / 2.0 - 300, screen_height / 2.0 - 130, 600,
                     260);

    // Decorative Golden Border
    iSetColor(218, 165, 32); // Gold
    iRectangle(screen_width / 2 - 305, screen_height / 2 - 135, 610, 270);
    iRectangle(screen_width / 2 - 300, screen_height / 2 - 130, 600, 260);

    // Floating Animation Offset (stops after 100 frames)
    int yFloat = (winTimer < 100) ? (int)(winTimer * 0.4) : 40;

    // Character Name Selection
    char savedName[50];
    if (currentLevel == 1)
      strcpy(savedName, "LAMIS");
    else if (currentLevel == 2)
      strcpy(savedName, "HAGGESH");
    else
      strcpy(savedName, "NIGGESH");

    // Saved Text
    char revealText[100];
    sprintf(revealText, "YOU SAVED %s!", savedName);
    int rw = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24,
                              (const unsigned char *)revealText);
    iSetColor(255, 215, 0); // Bright Gold
    iText(screen_width / 2 - rw / 2, screen_height / 2 + 50 + yFloat,
          revealText, GLUT_BITMAP_TIMES_ROMAN_24);

    // Reward Text (Only for Maze 2)
    if (currentLevel == 2) {
      char pwrText[] = "ATTACK POWER INCREASED +5!";
      int pw = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                (const unsigned char *)pwrText);
      iSetColor(0, 255, 127); // Spring Green
      iText(screen_width / 2 - pw / 2, screen_height / 2 + 10 + yFloat, pwrText,
            GLUT_BITMAP_HELVETICA_18);
    }

    // Pulse for sub-text
    int alpha = 127 + 128 * sin(winTimer * 0.05);
    iSetColor(alpha, alpha, alpha);
    char contText[] = "Click to Continue";
    int contWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                     (const unsigned char *)contText);
    iText(screen_width / 2.0 - contWidth / 2.0, screen_height / 2.0 - 80,
          contText, GLUT_BITMAP_HELVETICA_18);

    return;
  }
  iSetColor(50, 0, 0);
  iFilledRectangle(50, 600, 200, 20);
  int enemyHpLimit =
      (currentLevel == 3 ? 150
                         : (currentLevel == 2 ? 120 : 100)); // Dynamic HP max
  iFilledRectangle(950, 600, enemyHpLimit * 2, 20); // Scale the background too
  iSetColor(255, 0, 0);
  double hp = displayPlayerHealth;
  if (hp < 0)
    hp = 0;
  if (hp > 100)
    hp = 100;
  iFilledRectangle(50, 600, (int)(hp * 2.0), 20);
  double mhp = displayMonsterHealth;
  if (mhp < 0)
    mhp = 0;
  if (mhp > (double)enemyHpLimit)
    mhp = (double)enemyHpLimit;
  iFilledRectangle(950, 600, (int)(mhp * 2.0), 20);
  iSetColor(255, 255, 255);

  char pHealthText[] = "PLAYER HEALTH";
  int pHealthWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                      (const unsigned char *)pHealthText);
  iText(50 + (200 - pHealthWidth) / 2, 630, pHealthText,
        GLUT_BITMAP_HELVETICA_18);

  char mHealthText[] = "BOSS HEALTH";
  int mHealthWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                      (const unsigned char *)mHealthText);
  iText(950 + (enemyHpLimit * 2 - mHealthWidth) / 2, 630, mHealthText,
        GLUT_BITMAP_HELVETICA_18);
  char pFile[100];
  if (fightAttackIndex > 0) {
    sprintf(pFile, "Images\\fightMove%d.bmp", fightAttackIndex);
  } else {
    sprintf(pFile, "Images\\fightWalk%d.bmp", fightWalkIndex);
  }
  iShowBMP2((int)fightPlayerX, (int)fightPlayerY, pFile, 0);
  char mFile[100];
  if (monsterState == 1) // Attacking
    sprintf(mFile, "Images\\monster%dfight%d.bmp", currentLevel,
            monsterAttackIndex);
  else // Walking
    sprintf(mFile, "Images\\monster%dmove%d.bmp", currentLevel,
            monsterMoveIndex);

  iShowBMP2((int)monsterX, (int)monsterY, mFile, 0);
}

#endif // FIGHT_H

#ifndef LOGIC_H
#define LOGIC_H

#include "Maze.h"
#include "Sound.h"
#include "Variables.h"

// Resets the current level completely
inline void resetLevel(int level) {
  // Reset Core Game State
  playerX = 60;
  playerY = 60;
  playerHealth = startLevelHealth;
  gameTime = INITIAL_TIME;
  activePuzzle = 0;
  puzzleInputIndex = 0;
  puzzleInput[0] = '\0';
  isPaused = false;
  isWin = false;
  winTimer = 0;

  // Reset Boss Fight State
  monsterHealth = 100;
  if (level == 2)
    monsterHealth = 120;
  if (level == 3)
    monsterHealth = 150;
  displayMonsterHealth = (double)monsterHealth;
  displayPlayerHealth = 100.0;
  monsterX = 765;
  monsterY = 176;
  fightPlayerX = 157;
  fightPlayerY = 176;
  fightAttackIndex = 0;
  attackCooldown = 0;
  damageFlashTimer = 0;
  hpGainTimer = 0;

  // Reset Level Puzzles (don't start from progress I did)
  if (level == 1) {
    lv1P1Solved = lv1P2Solved = false;
  } else if (level == 2) {
    lv2P1Solved = lv2P2Solved = lv2P3Solved = lv2P4Solved = false;
  } else if (level == 3) {
    lv3P1Solved = lv3P2Solved = lv3P3Solved = lv3P4Solved = lv3P5Solved =
        lv3P6Solved = false;
  }

  // Reload Maze Layout
  loadLevelMaze(level);

  // Stop all sounds and prepare for fresh start
  stopSound();
  menuMusicPlaying = false;
  mazeMusicPlaying = false;
  fightMusicPlaying = false;
  storyMusicPlaying = false;
  heartBeatPlaying = false;
  walkSoundPlaying = false;
  monsterRunSoundPlaying = false;
  gameOverSoundPlaying = false;
}

// Timer function for countdown
inline void countDown() {
  if (isPaused)
    return;
  if (startPage == 1 || fightPage == 1) {
    if (gameTime > 0) {
      gameTime--;
    } else {
      startPage = 0;
      fightPage = 0;
      gameOverPage = 1;
    }
  }
}

// Skips the instruction page and starts the maze
inline void skipInstruction() {
  instructionPage = 0;
  levelPage = 1;
  instructionTimer = 0;
}

// Skips the current story step or advances to the next phase
inline void skipStoryStep() {
  storyTimer = 0;
  storyStep++;
  if (storyStep > 15) {
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
    monsterHealth = 100;
    displayPlayerHealth = playerHealth;
    displayMonsterHealth = 100;
  }
}

// Timer function for game logic updates
inline void fixedUpdate() {
  if (isPaused) {
    if (pauseScale < 1.0)
      pauseScale += 0.05;
    return;
  } else {
    pauseScale = 0.0;
  }
  // Home page and credits page background music
  if ((homePage == 1 || creditsPage == 1) && !menuMusicPlaying) {
    stopSound();
    playLoop("Sounds\\mazeBg.wav");
    menuMusicPlaying = true;
    mazeMusicPlaying = false;
    fightMusicPlaying = false;
  } else if (homePage == 0 && creditsPage == 0 && menuMusicPlaying) {
    stopSound();
    menuMusicPlaying = false;
  }

  if (instructionPage == 1) {
    instructionTimer++;
    if (instructionTimer >= 250) {
      skipInstruction();
    }
    return;
  }
  if (startPage == 1 && !mazeMusicPlaying) {
    stopSound();
    playLoop("Sounds\\mazeCreepy.wav");
    mazeMusicPlaying = true;
    fightMusicPlaying = false;
  }
  if (storyPage == 1 && !storyMusicPlaying) {
    stopSound();
    playLoop("Sounds\\storyBg.wav");
    storyMusicPlaying = true;
    mazeMusicPlaying = false;
    fightMusicPlaying = false;
  }
  if (startPage == 1) {
    int dx = 0;
    int dy = 0;
    isMoving = false;
    if (activePuzzle == 0) {
      if (isKeyPressed('w') || isKeyPressed('W') ||
          isSpecialKeyPressed(GLUT_KEY_UP))
        dy++;
      if (isKeyPressed('s') || isKeyPressed('S') ||
          isSpecialKeyPressed(GLUT_KEY_DOWN))
        dy--;
      if (isKeyPressed('a') || isKeyPressed('A') ||
          isSpecialKeyPressed(GLUT_KEY_LEFT))
        dx--;
      if (isKeyPressed('d') || isKeyPressed('D') ||
          isSpecialKeyPressed(GLUT_KEY_RIGHT))
        dx++;
    }

    if (playerHealth <= 0) {
      startPage = 0;
      mazeMusicPlaying = false;
      gameOverPage = 1;
    }

    if (puzzleTimer > 0)
      puzzleTimer--;

    if (dx != 0 || dy != 0)
      isMoving = true;

    if (isMoving && !walkSoundPlaying) {
      playLoop("Sounds\\walk.wav");
      walkSoundPlaying = true;
    }
    static bool wasMoving = false;
    if (isMoving && !wasMoving) {
      stopSound();
      playLoop("Sounds\\walk.wav");
      walkSoundPlaying = true;
    }
    if (!isMoving && wasMoving) {
      stopSound();
      if (startPage == 1)
        playLoop("Sounds\\mazeCreepy.wav");
      else if (fightPage == 1)
        playLoop("Sounds\\fightBg.wav");
      walkSoundPlaying = false;
    }
    wasMoving = isMoving;

    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
    int paddingX = 12, paddingY = 5;

    for (int i = 0; i < playerSpeed; i++) {
      int nextX = playerX + stepX;
      int nextY = playerY + stepY;
      if (isWalkable(nextX + paddingX, nextY + paddingY) &&
          isWalkable(nextX + charWidth - paddingX, nextY + paddingY) &&
          isWalkable(nextX + paddingX, nextY + charHeight - paddingY) &&
          isWalkable(nextX + charWidth - paddingX,
                     nextY + charHeight - paddingY)) {
        playerX = nextX;
        playerY = nextY;
      } else
        break;
    }

    if (dx != 0 || dy != 0) {
      if (abs(dx) > abs(dy))
        playerDirection = (dx > 0) ? 3 : 2;
      else
        playerDirection = (dy > 0) ? 1 : 0;
    }

    if (isMoving) {
      animTimer++;
      if (animTimer >= 5) {
        animTimer = 0;
        animIndex++;
        if (animIndex > 5)
          animIndex = 1;
      }
    } else
      animIndex = 1;

    int tileL = (playerX + 12) / TILE_SIZE;
    int tileR = (playerX + charWidth - 12) / TILE_SIZE;
    int tileB = (playerY + 5) / TILE_SIZE;
    int tileT = (playerY + charHeight - 5) / TILE_SIZE;

    if (maze[tileB][tileL] == 3 || maze[tileB][tileR] == 3 ||
        maze[tileT][tileL] == 3 || maze[tileT][tileR] == 3) {
      playerX = 60;
      playerY = 60;
    }
    if (maze[tileB][tileL] == 6 || maze[tileB][tileR] == 6 ||
        maze[tileT][tileL] == 6 || maze[tileT][tileR] == 6) {
      bool alreadySolved =
          (currentLevel == 1) ? lv1P1Solved
                              : (currentLevel == 2 ? lv2P1Solved : lv3P1Solved);
      if (!alreadySolved && activePuzzle == 0) {
        activePuzzle = 1;
        puzzleInputIndex = 0;
        puzzleInput[0] = '\0';
      }
    }
    if (maze[tileB][tileL] == 7 || maze[tileB][tileR] == 7 ||
        maze[tileT][tileL] == 7 || maze[tileT][tileR] == 7) {
      bool alreadySolved =
          (currentLevel == 1) ? lv1P2Solved
                              : (currentLevel == 2 ? lv2P2Solved : lv3P2Solved);
      if (!alreadySolved && activePuzzle == 0) {
        activePuzzle = 2;
        puzzleInputIndex = 0;
        puzzleInput[0] = '\0';
      }
    }
    if (maze[tileB][tileL] == 8 || maze[tileB][tileR] == 8 ||
        maze[tileT][tileL] == 8 || maze[tileT][tileR] == 8) {
      bool alreadySolved = (currentLevel == 2) ? lv2P3Solved : lv3P3Solved;
      if (!alreadySolved && activePuzzle == 0) {
        activePuzzle = 3;
        puzzleInputIndex = 0;
        puzzleInput[0] = '\0';
      }
    }
    if (maze[tileB][tileL] == 9 || maze[tileB][tileR] == 9 ||
        maze[tileT][tileL] == 9 || maze[tileT][tileR] == 9) {
      bool alreadySolved = (currentLevel == 2) ? lv2P4Solved : lv3P4Solved;
      if (!alreadySolved && activePuzzle == 0) {
        activePuzzle = 4;
        puzzleInputIndex = 0;
        puzzleInput[0] = '\0';
      }
    }
    if (maze[tileB][tileL] == 10 || maze[tileB][tileR] == 10 ||
        maze[tileT][tileL] == 10 || maze[tileT][tileR] == 10) {
      if (currentLevel == 3 && !lv3P5Solved && activePuzzle == 0) {
        activePuzzle = 5;
        puzzleInputIndex = 0;
        puzzleInput[0] = '\0';
      }
    }
    if (maze[tileB][tileL] == 11 || maze[tileB][tileR] == 11 ||
        maze[tileT][tileL] == 11 || maze[tileT][tileR] == 11) {
      if (currentLevel == 3 && !lv3P6Solved && activePuzzle == 0) {
        activePuzzle = 6;
        puzzleInputIndex = 0;
        puzzleInput[0] = '\0';
      }
    }

    bool onBossTile = (maze[tileB][tileL] == 5 || maze[tileB][tileR] == 5 ||
                       maze[tileT][tileL] == 5 || maze[tileT][tileR] == 5);
    if (onBossTile) {
      if (currentLevel == 1 && !storyFinished && storyPage == 0) {
        if (lv1P1Solved && lv1P2Solved) {
          startPage = 0;
          storyPage = 1;
          storyStep = 0;
          storyTimer = 0;
        }
      } else if (currentLevel == 2 && fightPage == 0) {
        if (lv2P1Solved && lv2P2Solved && lv2P3Solved && lv2P4Solved) {
          startPage = 0;
          fightPage = 1;
          monsterHealth = 120;
          displayMonsterHealth = 120;
          monsterX = 765;
          monsterY = 176;
        }
      } else if (currentLevel == 3 && fightPage == 0) {
        if (lv3P1Solved && lv3P2Solved && lv3P3Solved && lv3P4Solved &&
            lv3P5Solved && lv3P6Solved) {
          startPage = 0;
          fightPage = 1;
          monsterHealth = 150;
          displayMonsterHealth = 150;
          monsterX = 765;
          monsterY = 176;
        }
      }
    }
    if (maze[tileB][tileL] == 4 || maze[tileB][tileR] == 4 ||
        maze[tileT][tileL] == 4 || maze[tileT][tileR] == 4) {
      obj3Solved = true;
      if (currentLevel == 1)
        level1Finished = true;
      else if (currentLevel == 2)
        level2Finished = true;
      else if (currentLevel == 3)
        level3Finished = true;

      playerHealth += 10;
      if (playerHealth > 100)
        playerHealth = 100;
      if (currentLevel == unlockedLevel && unlockedLevel < 3)
        unlockedLevel++;

      startPage = 0;
      levelPage = 1;
      playerX = 60;
      playerY = 60;
      obj1Solved = obj2Solved = obj3Solved = false;
    }
  }
  if (fightPage == 1) {
    if (!isWin) {
      if (!fightMusicPlaying) {
        stopSound();
        playLoop("Sounds\\fightBg.wav");
        fightMusicPlaying = true;
        mazeMusicPlaying = false;
      }

      // Difficulty Scaling Parameters (Lowered damage as requested)
      int bossDamage = (currentLevel == 3 ? 12 : (currentLevel == 2 ? 10 : 8));
      int bossSpeed = (currentLevel == 3 ? 3 : (currentLevel == 2 ? 2 : 1));
      int bossHpMax =
          (currentLevel == 3 ? 150 : (currentLevel == 2 ? 120 : 100));

      static int monsterAttackCooldown = 0;
      static int monsterAnimCounter = 0;
      if (monsterAttackCooldown > 0)
        monsterAttackCooldown--;

      int dist = abs((int)fightPlayerX - (int)monsterX);
      if (dist > 150 && !monsterRunSoundPlaying) {
        stopSound();
        playLoop("Sounds\\monsterRun.wav");
        monsterRunSoundPlaying = true;
      } else if (dist <= 150 && monsterRunSoundPlaying) {
        stopSound();
        playLoop("Sounds\\fightBg.wav");
        monsterRunSoundPlaying = false;
      }

      // Monster State and Movement
      if (dist > 80) {
        monsterState = 0; // Walking
        if (monsterX > fightPlayerX + 80)
          monsterX -= (double)bossSpeed;
        else if (monsterX < fightPlayerX - 80)
          monsterX += (double)bossSpeed;
      } else {
        monsterState = 1; // Attacking
      }

      // Monster Animations - Scaled with Speed for smoothness
      monsterAnimCounter++;
      int animLimit = (currentLevel == 3 ? 3 : (currentLevel == 2 ? 4 : 6));
      if (monsterAnimCounter >= animLimit) {
        monsterAnimCounter = 0;
        if (monsterState == 0) {
          monsterMoveIndex++;
          if (monsterMoveIndex > 5)
            monsterMoveIndex = 1;
          monsterAttackIndex = 1; // Reset attack frame
        } else {
          monsterAttackIndex++;
          if (monsterAttackIndex > 3)
            monsterAttackIndex = 1;

          // Apply Damage on specific frame
          if (monsterAttackIndex == 2 && monsterAttackCooldown == 0) {
            playOnce("Sounds\\monsterAttack.wav");
            playerHealth -= bossDamage;
            if (playerHealth < 0)
              playerHealth = 0;
            damageFlashTimer = 10;
            monsterAttackCooldown =
                (currentLevel == 3 ? 15 : 30); // Boss 3 attacks even faster
          }
        }
      }

      // Player Animation Updates
      static int fightAnimCounter = 0;
      fightAnimCounter++;
      if (fightAnimCounter >= 6) {
        fightAnimCounter = 0;
        if (fightAttackIndex > 0) {
          fightAttackIndex++;
          if (fightAttackIndex > 5)
            fightAttackIndex = 0;
        }
      }

      bool moving = false;
      if (isKeyPressed('d') || isKeyPressed('D')) {
        fightPlayerVelX += 1.0f;
        moving = true;
      }
      if (isKeyPressed('a') || isKeyPressed('A')) {
        fightPlayerVelX -= 1.0f;
        moving = true;
      }

      fightPlayerVelX *= 0.75f;
      if (fightPlayerVelX > 4.5f)
        fightPlayerVelX = 4.5f;
      if (fightPlayerVelX < -4.5f)
        fightPlayerVelX = -4.5f;
      fightPlayerX += (double)fightPlayerVelX;

      if (fightPlayerX < 50) {
        fightPlayerX = 50;
        fightPlayerVelX = 0;
      }
      if (fightPlayerX > screen_width - 160) {
        fightPlayerX = screen_width - 160;
        fightPlayerVelX = 0;
      }

      if (moving || fabsf(fightPlayerVelX) > 0.5f) {
        static float walkAnimTimer = 0;
        walkAnimTimer += fabsf(fightPlayerVelX) * 0.08f;
        if (walkAnimTimer >= 1.0f) {
          fightWalkIndex++;
          if (fightWalkIndex > 5)
            fightWalkIndex = 1;
          walkAnimTimer = 0;
        }
      } else {
        fightWalkIndex = 1;
      }

      if (attackCooldown > 0)
        attackCooldown--;
      if ((isKeyPressed('f') || isKeyPressed('F')) && fightAttackIndex == 0 &&
          attackCooldown == 0) {
        if (abs((int)fightPlayerX - (int)monsterX) < 250) {
          fightAttackIndex = 1;
          attackCooldown = 15;
          monsterHealth -= playerAttackPower;
          if (monsterHealth <= 0) {
            monsterHealth = 0;
            isWin = true;
            winTimer = 0;
            if (currentLevel == 2)
              playerAttackPower += 5;
            else if (currentLevel == 3)
              playerAttackPower += 5;
          }
        }
      }

      if (playerHealth <= 0) {
        playerHealth = 0;
        fightPage = 0;
        gameOverPage = 1;
        return;
      }
    }

    if (playerHealth <= 30 && playerHealth > 0) {
      if (!heartBeatPlaying) {
        stopSound();
        playLoop("Sounds\\heartBeat.wav");
        heartBeatPlaying = true;
      }
    } else if (heartBeatPlaying) {
      stopSound();
      if (fightPage == 1)
        playLoop("Sounds\\fightBg.wav");
      heartBeatPlaying = false;
    }

    if (displayPlayerHealth != (double)playerHealth) {
      displayPlayerHealth +=
          ((double)playerHealth - displayPlayerHealth) * 0.15;
    }
    if (displayMonsterHealth != (double)monsterHealth) {
      displayMonsterHealth +=
          ((double)monsterHealth - displayMonsterHealth) * 0.10;
    }

    if (isWin) {
      winTimer++;
    }
  }

  if (damageFlashTimer > 0)
    damageFlashTimer--;
  if (hpGainTimer > 0)
    hpGainTimer--;
  if (storyPage == 1 && fightPage == 0 && startPage == 0) {
    storyTimer++;
    int timeLimit = (storyStep >= 6) ? 200 : 120;
    if (storyTimer >= timeLimit) {
      skipStoryStep();
    }
  }
  if (gameOverPage == 1 && !gameOverSoundPlaying) {
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

inline void handlePuzzleKeyboard(unsigned char key) {
  if (key == '\r' || key == '\n') {
    bool solved = false;
    if (activePuzzle == 1) {
      if (currentLevel == 1 && _stricmp(puzzleInput, "SHADOW") == 0) {
        solved = true;
        lv1P1Solved = true;
      } else if (currentLevel == 2 && _stricmp(puzzleInput, "WRONG") == 0) {
        solved = true;
        lv2P1Solved = true;
      } else if (currentLevel == 3 && _stricmp(puzzleInput, "MAP") == 0) {
        solved = true;
        lv3P1Solved = true;
      }
    } else if (activePuzzle == 2) {
      if (currentLevel == 1 && _stricmp(puzzleInput, "THE LIGHT") == 0) {
        solved = true;
        lv1P2Solved = true;
      } else if (currentLevel == 2 && _stricmp(puzzleInput, "HORIZON") == 0) {
        solved = true;
        lv2P2Solved = true;
      } else if (currentLevel == 3 && (strcmp(puzzleInput, "7") == 0)) {
        solved = true;
        lv3P2Solved = true;
      }
    } else if (activePuzzle == 3) {
      if (currentLevel == 2 && _stricmp(puzzleInput, "ALWAYS") == 0) {
        solved = true;
        lv2P3Solved = true;
      } else if (currentLevel == 3 && _stricmp(puzzleInput, "19") == 0) {
        solved = true;
        lv3P3Solved = true;
      }
    } else if (activePuzzle == 4) {
      if (currentLevel == 2 && (strcmp(puzzleInput, "1") == 0)) {
        solved = true;
        lv2P4Solved = true;
        playerHealth += 40;
        if (playerHealth > 100)
          playerHealth = 100;
        hpGainValue = 40;
        hpGainTimer = 100;
      } else if (currentLevel == 3 &&
                 (_stricmp(puzzleInput, "reno") == 0 ||
                  _stricmp(puzzleInput, "zahid") == 0 ||
                  _stricmp(puzzleInput, "reno,zahid") == 0)) {
        solved = true;
        lv3P4Solved = true;
        playerHealth = 100;
        hpGainValue = 70;
        hpGainTimer = 100;
      }
    } else if (activePuzzle == 5) {
      if (currentLevel == 3 && (strcmp(puzzleInput, "32") == 0)) {
        solved = true;
        lv3P5Solved = true;
      }
    } else if (activePuzzle == 6) {
      if (currentLevel == 3 && _stricmp(puzzleInput, "COIN") == 0) {
        solved = true;
        lv3P6Solved = true;
      }
    }

    if (solved) {
      activePuzzle = 0;
      for (int ir = 0; ir < ROWS; ir++)
        for (int ic = 0; ic < COLS; ic++)
          if (maze[ir][ic] >= 6 && maze[ir][ic] <= 11) {
            bool s = false;
            int type = maze[ir][ic];
            if (currentLevel == 1) {
              if (type == 6 && lv1P1Solved)
                s = true;
              if (type == 7 && lv1P2Solved)
                s = true;
            } else if (currentLevel == 2) {
              if (type == 6 && lv2P1Solved)
                s = true;
              if (type == 7 && lv2P2Solved)
                s = true;
              if (type == 8 && lv2P3Solved)
                s = true;
              if (type == 9 && lv2P4Solved)
                s = true;
            } else if (currentLevel == 3) {
              if (type == 6 && lv3P1Solved)
                s = true;
              if (type == 7 && lv3P2Solved)
                s = true;
              if (type == 8 && lv3P3Solved)
                s = true;
              if (type == 9 && lv3P4Solved)
                s = true;
              if (type == 10 && lv3P5Solved)
                s = true;
              if (type == 11 && lv3P6Solved)
                s = true;
            }
            if (s)
              maze[ir][ic] = 0;
          }
    } else {
      puzzleInputIndex = 0;
      puzzleInput[0] = '\0';
      playerHealth -= 5;
      if (playerHealth < 0)
        playerHealth = 0;
    }
  } else if (key == '\b') {
    if (puzzleInputIndex > 0) {
      puzzleInputIndex--;
      puzzleInput[puzzleInputIndex] = '\0';
    }
  } else if (puzzleInputIndex < 18) {
    puzzleInput[puzzleInputIndex] = key;
    puzzleInputIndex++;
    puzzleInput[puzzleInputIndex] = '\0';
  }
}

#endif // LOGIC_H

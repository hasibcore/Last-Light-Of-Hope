#ifndef SOUND_H
#define SOUND_H

#include "Variables.h"

// Sound related functions
void playLoop(const char* file)
{
	PlaySound(file, NULL, SND_LOOP | SND_ASYNC | SND_NODEFAULT | SND_FILENAME);
}

void playOnce(const char* file)
{
	PlaySound(file, NULL, SND_ASYNC | SND_NODEFAULT | SND_FILENAME);
}

void stopSound()
{
	PlaySound(NULL, 0, 0);
}

void setMusic(MusicState next)
{
	if (currentMusic == next)
	{
		return;
	}
	stopSound();
	if (next == MUSIC_MAZE_BG)
	{
		playLoop("Sounds\\mazeBg.wav");
	}
	else if (next == MUSIC_WALK)
	{
		playLoop("Sounds\\walk.wav");
	}
	else if (next == MUSIC_FIGHT)
	{
		playLoop("Sounds\\fight.wav");
	}
	else if (next == MUSIC_HEARTBEAT)
	{
		playLoop("Sounds\\heartBeat.wav");
	}
	else if (next == MUSIC_GAMEOVER)
	{
		playOnce("Sounds\\gameOver.wav");
	}
	else if (next == SFX_MONSTER_ATTACK)
	{
		playOnce("Sounds\\monsterAttack.wav");
	}
	else if (next == SFX_MONSTER_RUN)
	{
		playLoop("Sounds\\monsterRun.wav");
	}
	else if (next == SFX_SWORD)
	{
		playOnce("Sounds\\sword.wav");
	}
	currentMusic = next;
}

#endif // SOUND_H

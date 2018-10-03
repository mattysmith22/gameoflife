
#ifndef GOL_H_INCLUDED
#define GOL_H_INCLUDED

const char cellLive;
const char cellDead;

struct GameOfLife {
	char **grid;
	unsigned int height, width;
	unsigned int numIteration;
};

void golInit(struct GameOfLife *gol, unsigned int height, unsigned int width);

void golDisp(struct GameOfLife *gol);

char golReadSafe(struct GameOfLife *gol, unsigned int x, unsigned int y);

void golRun(struct GameOfLife *gol);

void golToggle(struct GameOfLife *gol, unsigned int x, unsigned int y);

void golResize(struct GameOfLife *gol, unsigned int newHeight, unsigned int newWidth);

#endif
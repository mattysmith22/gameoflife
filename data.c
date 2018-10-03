const char cellLive = '#';
const char cellDead = '-';

struct GameOfLife {
	char **grid;
	unsigned int height, width;
	unsigned int numIteration;
};
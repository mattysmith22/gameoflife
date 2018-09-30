#include <stdio.h>
#include <curses.h>
#include <string.h>

#define GRID_WIDTH 20
#define GRID_HEIGHT 20

const char cellLive = '#';
const char cellDead = '-';

struct GameOfLife {
	char grid[GRID_HEIGHT][GRID_WIDTH + 1];
	unsigned int numIteration;
};

void golInit(struct GameOfLife *gol)
{
    int x, y;
    for(y = 0; y < GRID_HEIGHT; y++)
    {
	for(x = 0; x < GRID_WIDTH; x++)
	{
	    gol->grid[y][x] = cellDead;
	}
	gol->grid[y][GRID_WIDTH] = '\0';
    }

    gol->numIteration = 0;
}

char golReadSafe(struct GameOfLife *gol, int x, int y)
{
    if(x < 0 || x >= GRID_WIDTH)
    {
	return cellDead;
    }
    else if(y < 0 || y >= GRID_HEIGHT)
    {
	return cellDead;
    }
    else
    {
	return gol->grid[y][x];
    }
}

void golRun(struct GameOfLife *gol)
{
    char nextGrid[GRID_HEIGHT][GRID_WIDTH];

    
    int x, y;
    char numAdjacent;
    for(x = 0; x < GRID_WIDTH; x++)
    {
	for(y = 0; y < GRID_HEIGHT; y++)
	{
	    numAdjacent = (golReadSafe(gol, x - 1, y) == cellLive ? 1 : 0);
	    numAdjacent += (golReadSafe(gol, x + 1, y) == cellLive ? 1 : 0);
	    numAdjacent += (golReadSafe(gol, x, y - 1) == cellLive ? 1 : 0);
	    numAdjacent += (golReadSafe(gol, x, y + 1) == cellLive ? 1 : 0);
	    numAdjacent += (golReadSafe(gol, x - 1, y + 1) == cellLive ? 1 : 0);
	    numAdjacent += (golReadSafe(gol, x - 1, y - 1) == cellLive ? 1 : 0);
	    numAdjacent += (golReadSafe(gol, x + 1, y + 1) == cellLive ? 1 : 0);
	    numAdjacent += (golReadSafe(gol, x + 1, y - 1) == cellLive ? 1 : 0);

	    if(golReadSafe(gol, x, y) == cellLive && (numAdjacent < 2 || numAdjacent > 3))
	    {
		nextGrid[y][x] = cellDead;
	    }
	    else if(golReadSafe(gol, x, y) == cellDead && numAdjacent == 3)
	    {
		nextGrid[y][x] = cellLive;
	    }
	    else
	    {
		nextGrid[y][x] = gol->grid[y][x];
	    }
	}
    }

    for(x = 0; x < GRID_WIDTH; x++)
    {
	for(y = 0; y < GRID_HEIGHT; y++)
	{
	    gol->grid[y][x] = nextGrid[y][x];
	}
    }

    gol->numIteration += 1;
}

void golToggle(struct GameOfLife *gol, int x, int y)
{
    if(x >= 0 && y >= 0 && x < GRID_WIDTH && y < GRID_HEIGHT)
    {
	gol->grid[y][x] = (gol->grid[y][x] == cellLive ? cellDead : cellLive);
    }
}

void winDraw(struct GameOfLife *gol)
{
    char iterBuffer[GRID_WIDTH + 1];
    sprintf(iterBuffer, "Iteration: %du", gol->numIteration);
    move(0, 0);
    addstr(iterBuffer);
    
    int y;
    for(y = 0; y < GRID_HEIGHT; y++)
    {
	move(y + 1, 0);
	printw("%s", gol->grid[y]);
    }
    refresh();
}

int main()
{
    struct GameOfLife gol;
    //initalise the curses screen
    initscr();
    cbreak(); //Disable buffering of typed characters
    noecho(); //suppress echo of character
    
    mousemask(ALL_MOUSE_EVENTS, NULL);


    keypad(stdscr, TRUE); //capture special characters
    
    golInit(&gol);

    int ch;
    int running = 1;
    MEVENT mouseEvent;
    clear();
    winDraw(&gol);

    while(running)
    {
	ch = wgetch(stdscr);
	switch(ch) {
	    case 'q':
		running = 0;
		break;
	    case 'n':
	        golRun(&gol);
		winDraw(&gol);
	    case KEY_MOUSE:
		if(getmouse(&mouseEvent) == OK && (mouseEvent.bstate & BUTTON1_PRESSED))
		{
	            golToggle(&gol, mouseEvent.x, mouseEvent.y - 1);
		    winDraw(&gol);
		}
	}
    }

    //run before ending
    endwin();
    return 0;
}

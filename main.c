#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

const char cellLive = '#';
const char cellDead = '-';

struct GameOfLife {
	char **grid;
	unsigned int height, width;
	unsigned int numIteration;
};

void golInit(struct GameOfLife *gol, unsigned int height, unsigned int width)
{
    gol->height = height;
    gol->width = width;

    gol->grid = malloc(sizeof(char*) * height);

    unsigned int x, y;
    for(y = 0; y < height; y++)
    {
        gol->grid[y] = malloc(sizeof(char) * (width + 1));

        for(x = 0; x < width; x++)
        {
            gol->grid[y][x] = cellDead;
        }
        gol->grid[y][width] = '\0';
    }

    gol->numIteration = 0;
}

void golDisp(struct GameOfLife *gol)
{
    unsigned int y;
    for(y = 0; y < gol->height; y++)
    {
        free(gol->grid[y]);
    }

    free(gol->grid);
}

char golReadSafe(struct GameOfLife *gol, unsigned int x, unsigned int y)
{
    if(x < 0 || x >= gol->width)
    {
	    return cellDead;
    }
    else if(y < 0 || y >= gol->height)
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
    char nextGrid[gol->height][gol->width];

    
    unsigned int x, y;
    char numAdjacent;
    for(x = 0; x < gol->width; x++)
    {
        for(y = 0; y < gol->height; y++)
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

    for(x = 0; x < gol->width; x++)
    {
        for(y = 0; y < gol->height; y++)
        {
            gol->grid[y][x] = nextGrid[y][x];
        }
    }

    gol->numIteration += 1;
}

void golToggle(struct GameOfLife *gol, unsigned int x, unsigned int y)
{
    if(x >= 0 && y >= 0 && x < gol->width && y < gol->height)
    {
        gol->grid[y][x] = (gol->grid[y][x] == cellLive ? cellDead : cellLive);
    }
}

void winDraw(struct GameOfLife *gol)
{
    clear();
    char iterBuffer[gol->width + 1];
    sprintf(iterBuffer, "Iteration: %d", gol->numIteration);
    move(0, 0);
    addstr(iterBuffer);
    
    unsigned int y;
    for(y = 0; y < gol->height; y++)
    {
        move(y + 1, 0);
        printw("%s", gol->grid[y]);
    }
    refresh();
}

void clearGrid(struct GameOfLife* gol){
	unsigned int i,j;
	for(i = 0; i < gol->height; i++)
		for(j = 0; j < gol->width; j++)
			gol->grid[i][j] = cellDead;
	gol->numIteration = 0;
}

int main(int argc, char **argv)
{
    struct GameOfLife gol;
    //initalise the curses screen
    initscr();
    cbreak(); //Disable buffering of typed characters
    noecho(); //suppress echo of character
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE); //capture special characters
    unsigned int width, height;
    if(argc == 1)
    {
        getmaxyx(stdscr, height, width);
        height--;
	    golInit(&gol, height, width);
    }
    else if(argc == 3)
    {
	    height = strtol(argv[1], NULL, 0);
	    width = strtol(argv[2], NULL, 0);
	    golInit(&gol, height, width);
    }
    else
    {
	    return 1;
    }

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
                break;
            case 'c':
                clearGrid(&gol);
                winDraw(&gol);
                break;
            case KEY_MOUSE:
                if(getmouse(&mouseEvent) == OK && (mouseEvent.bstate & BUTTON1_PRESSED))
                {
                    golToggle(&gol, mouseEvent.x, mouseEvent.y - 1);
                    winDraw(&gol);
                }
                break;
            case KEY_RESIZE:
                if (argc == 1) //if no arguments were given, and it should therefore be on autoresize
                {
                    getmaxyx(stdscr, height, width);
                    height--;
                    //run command to resize
                    winDraw(&gol);
                }
        }
    }

    //run before ending
    endwin();
    golDisp(&gol);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "gol.h"

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
    //initialise the curses screen
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
					endwin();
					refresh();
                    getmaxyx(stdscr, height, width);
                    height--;
                    golResize(&gol, height, width);
                    winDraw(&gol);
                }
        }
    }

    //run before ending
    endwin();
    golDisp(&gol);
    return 0;
}
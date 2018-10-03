#include <stdlib.h>
#include "data.h"
#include "gol.h"

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

void golResize(struct GameOfLife *gol, unsigned int newHeight, unsigned int newWidth)
{
    unsigned int x, y;

    for(y = 0; y < newHeight && y < gol->height; y++) //For any rows that are not added / removed, change width if necessary
    {
        if(gol->width != newWidth)
        {
            gol->grid[y] = realloc(gol->grid[y], (newWidth + 1) * sizeof(char));
            for(x = gol->width; x < newWidth; x++) //For any created cell (between width and newWidth)
            {
                gol->grid[y][x] = cellDead;
            }
            gol->grid[y][newWidth] = '\0';
        }
    }

    for(y = newHeight; y < gol->height; y++) //For any rows that need to be removed
    {
        free(gol->grid[y]);
    }

    gol->grid = realloc(gol->grid, newHeight * sizeof(char*)); //For any row that need to be added

    for(y = gol->height; y < newHeight; y++)
	{
		gol->grid[y] = malloc((newWidth + 1) * sizeof(char));
		for(x = 0; x < newWidth; x++)
			gol->grid[y][x] = cellDead;
		gol->grid[y][newWidth];
	}
	
	gol->width = newWidth;
	gol->height = newHeight;
}
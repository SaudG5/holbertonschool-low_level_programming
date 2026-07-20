#include "main.h"

/**
 * free_grid - frees a 2 dimensional grid previously allocated
 * by alloc_grid
 * @grid: pointer to the grid to free
 * @height: height of the grid (number of rows)
 *
 * Return: Nothing.
 */
void free_grid(int **grid, int height)
{
	int h;

	for (h = 0; h < height; h++)
		free(grid[h]);

	free(grid);
}

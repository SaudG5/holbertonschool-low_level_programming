#include "main.h"

/**
 * alloc_grid - allocates a 2 dimensional array of integers
 * @width: width of the grid (number of columns)
 * @height: height of the grid (number of rows)
 *
 * Return: pointer to the newly allocated grid, with each element
 * initialized to 0, or NULL on failure or if width or height
 * is 0 or negative
 */
int **alloc_grid(int width, int height)
{
	int **grid;
	int h;
	int w;

	if (width <= 0 || height <= 0)
		return (NULL);

	grid = malloc(sizeof(int *) * height);
	if (grid == NULL)
		return (NULL);

	for (h = 0; h < height; h++)
	{
		grid[h] = malloc(sizeof(int) * width);
		if (grid[h] == NULL)
		{
			for (w = 0; w < h; w++)
				free(grid[w]);
			free(grid);
			return (NULL);
		}

		for (w = 0; w < width; w++)
			grid[h][w] = 0;
	}

	return (grid);
}

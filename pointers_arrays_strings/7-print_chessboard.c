#include "main.h"

/**
 * print_chessboard - prints an 8x8 chessboard
 * @a: the 2D array representing the chessboard
 *
 * Return: Nothing.
 */
void print_chessboard(char (*a)[8])
{
	int row;
	int col;

	for (row = 0; row < 8; row++)
	{
		for (col = 0; col < 8; col++)
		{
			_putchar(a[row][col]);
		}
		_putchar('\n');
	}
}

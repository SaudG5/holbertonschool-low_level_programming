#include "main.h"

/**
 * sqrt_helper - checks candidate values to find the natural square root
 * @n: the number whose square root is sought
 * @guess: the current candidate value being tested
 *
 * Return: the natural square root of n if found, or -1 if none exists
 */
int sqrt_helper(int n, int guess)
{
	if (guess * guess == n)
	{
		return (guess);
	}

	if (guess * guess > n)
	{
		return (-1);
	}

	return (sqrt_helper(n, guess + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of a number,
 * using recursion
 * @n: the number whose square root is sought
 *
 * Return: the natural square root of n, or -1 if it has none
 */
int _sqrt_recursion(int n)
{
	if (n < 0)
	{
		return (-1);
	}

	return (sqrt_helper(n, 0));
}

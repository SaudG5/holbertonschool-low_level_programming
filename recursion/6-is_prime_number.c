#include "main.h"

/**
 * prime_helper - checks whether n is divisible by any value from
 * divisor up to n - 1
 * @n: the number being tested
 * @divisor: the current divisor to test
 *
 * Return: 1 if n is prime, 0 otherwise
 */
int prime_helper(int n, int divisor)
{
	if (divisor * divisor > n)
	{
		return (1);
	}

	if (n % divisor == 0)
	{
		return (0);
	}

	return (prime_helper(n, divisor + 1));
}

/**
 * is_prime_number - returns 1 if the input integer is a prime number,
 * otherwise 0, using recursion
 * @n: the number to test
 *
 * Return: 1 if n is prime, 0 otherwise
 */
int is_prime_number(int n)
{
	if (n < 2)
	{
		return (0);
	}

	return (prime_helper(n, 2));
}

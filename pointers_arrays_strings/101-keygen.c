#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * main - Generates a valid password for 101-crackme
 * Return: 0 on success
 */
int main(void)
{
    int sum = 0, i = 0;
    int target_sum = 2772; /* Replace this with the value found in your binary */
    char password[100];
    int r;

    srand(time(NULL));

    while (sum < target_sum - 127)
    {
        r = (rand() % 94) + 33; /* Generate printable ASCII */
        password[i++] = r;
        sum += r;
    }
    
    password[i] = target_sum - sum; /* Last char to hit target exactly */
    password[i + 1] = '\0';
    
    printf("%s\n", password);
    return (0);
}

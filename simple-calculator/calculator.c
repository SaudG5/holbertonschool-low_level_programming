#include <stdio.h>
int main(void)
{
	while (True)
	{
		int choice;

		printf("Welcome to the simple calcutar, which operation do you want?\n");
		printf("1 Add\n");
		printf("2 Subtract\n");
		printf("3 Multiply\n");
		printf("4 Divide\n");
		printf("0 Quit\n");
		printf("choose the number of operatoin you want: \n");
		scanf("%d", &choice);
		if (choice == 0)
		{
			printf("Bye!\n");
			return (0);
		}
		else if (choice > 4 && choice < 0)
			printf("Invalid choice");
	}
}

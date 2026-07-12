#include <stdio.h>
int main(void)
{
	while (1)
	{
		int choice, A, B;

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
		else if (choice > 4 || choice <= 0)
			printf("Invalid choice");
		else if (choice == 1)
		{
			printf("A: \n");
			scanf("%d", &A);
			printf("B: \n");
			scanf("%d", &B);
			printf("Result: %d\n", A + B);
		}
		else if (choice == 2)
		{
			printf("A: \n");
			scanf("%d", &A);
			printf("B: \n");
			scanf("%d", &B);
			printf("Result: %d\n", A - B);
		}
		else if (choice == 3)
		{
			printf("A: \n");
			scanf("%d", &A);
			printf("B: \n");
			scanf("%d", &B);
			printf("Result: %d\n", A * B);
		}
		else if (choice == 4)
		{
			printf("A: \n");
			scanf("%d", &A);
			printf("B: \n");
			scanf("%d", &B);
			if (B == 0)
				printf("Error: division by zero");
			printf("Result: %d\n", A / B);
		}
	}
	return (0);
}

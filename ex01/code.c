#include "gnlh.h"
#include <string.h>

int main(void)
{
	int fd = open("text.txt", O_RDONLY);

	char* str = get_next_line(fd);
	int sum = 0;
	char *digits[] = {"one","two","three","four","five","six","seven","eight","nine"};

	while(str)
	{
/* 		printf("%s\n", str); */
		int nums = 0;
		int lastdig = 0;
		int result = 0;
		for(int i = 0; str[i]; i++)
		{
			if(isdigit(str[i]))
			{
				if(nums == 0)
					result = result * 10 + str[i] - 48;
				lastdig = str[i] - 48;
				nums++;
			}

			for(int j = 0; j < 9; j++)
			{
				int size = strlen(digits[j]);
				if(strncmp(str + i, digits[j], size) == 0)
				{
					if(nums == 0)
					{
						result = result * 10 + j + 1;
/* 						printf("reached, [%i]\n", j + 1); */
					}
					lastdig = j + 1;
					nums++;
				}
			}
		}
		result = result * 10 + lastdig;
/* 		printf("[%i]\n", result); */
		sum += result;
		str = get_next_line(fd);
	}
	printf("%i\n", sum);
}
#include "gnlh.h"


int check_limits(int green, int red, int blue)
{
	if(green > 13 || red > 12 || blue > 14)
		return 0;
	return 1;
}

int find_color(char **colors, char *str)
{
	for(int i = 0; str[i]; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			int size = strlen(colors[j]);
			if(strncmp(str+i, colors[j], size) == 0)
				return j;
		}
	}
}

int main(void)
{
	int fd = open("test.txt", O_RDONLY);
	char *line;
	char *colors[] = {"red","green","blue"};
	int sum = 0;

	while(line = get_next_line(fd))
	{
		char **strs = split(line, ':');
		int game = atoi(strs[0]);
		char **revealed = split(strs[1], ';');
		int possible = 1;
		for(int i = 0; revealed[i]; i++)
		{
			int green = 0;
			int red = 0;
			int blue = 0;
			char **groups = split(revealed[i], ',');

			for(int j = 0; groups[j]; j++)
			{
				int balls = atoi(groups[j]);
				int color = find_color(colors, groups[j]);
				switch (color)
				{
					case 0:
					{
						red += balls;
						break;
					}
					case 1:
					{
						green += balls;
						break;
					}
					case 2:
					{
						blue += balls;
						break;
					}
				}
			}
			if(check_limits(green, red, blue) == 0)
				possible = 0;
			//printf("green = %i // red = %i // blue = %i\n\n", green, red, blue);
		}
		if(possible)
		{
			printf("Game %i is possible\n", game);
			sum += game;
		}
		//printf("%s\n", strs[1]);
	}
	printf("%i\n\n", sum);
}
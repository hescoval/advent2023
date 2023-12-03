#include "gnl.h"

int limit_y;
int limit_x;

int check_around(char **lines, int x, int y, int* sum)
{
	if(x < 0 || x > limit_x || y < 0 || y > limit_y)
		return 0;

	if(!isdigit(lines[y][x]))
		return 0;

	while(isdigit(lines[y][x]) && x >= 0)
		x--;
	
	*sum += atoi(lines[y] + (x + 1));
	return 1;
}

void check_numbers(char** lines, int y, int x, int* sum)
{
	check_around(lines, x + 1, y, sum);
	check_around(lines, x - 1, y, sum);
	if(!check_around(lines, x, y + 1, sum))
	{
		check_around(lines, x - 1, y + 1, sum);
		check_around(lines, x + 1, y + 1, sum);
	}
	if(!check_around(lines, x, y - 1, sum))
	{
		check_around(lines, x - 1, y - 1, sum);
		check_around(lines, x + 1, y - 1, sum);
	}
}

int symbol(char c)
{
	if(c != '.' && !isdigit(c) && !isalpha(c) && c != '\n')
		return 1;
	return 0;
}

char **parse_input(int line_number)
{
	int i = 0;
	int fd = open("test.txt", O_RDONLY);

	char **lines = malloc((sizeof(char*)) * (line_number + 1));
	char *str;

	while(str = get_next_line(fd))
	{
		lines[i] = str;
		i++;
	}
	lines[i] = NULL;
	close(fd);
	return lines;
}

int count_lines()
{
	int fd = open("test.txt", O_RDONLY);
	int lines = 0;
	char *str;

	while(str = get_next_line(fd))
		lines++;
	close(fd);
	return lines;
}

int main()
{
	int line_number = count_lines();
	char **lines = parse_input(line_number);
	int sum = 0;
	limit_x = strlen(lines[0]) - 2;
	limit_y = line_number - 1;

	printf("X limit is %i and Y limit is %i\n", limit_x, limit_y);

	for(int i = 0; lines[i]; i++)
	{
		for(int j = 0; j < limit_x; j++)
		{
			if(symbol(lines[i][j]))
			{
				printf("We're at a %c\n", lines[i][j]);
				check_numbers(lines, i, j, &sum);
			}
		}
	}
	printf("%i\n", sum);
}
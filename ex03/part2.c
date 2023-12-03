#include "gnl.h"

int limit_y;
int limit_x;

int check_around(char **lines, int x, int y, int* numbers)
{
	if(x < 0 || x > limit_x || y < 0 || y > limit_y)
		return 0;

	if(!isdigit(lines[y][x]))
		return 0;

	while(isdigit(lines[y][x]) && x >= 0)
		x--;
	
	for(int i = 0; i < 8; i++)
	{
		if(numbers[i] == -1)
		{
			numbers[i] = atoi(lines[y] + (x + 1));
			break;
		}
	}
	return 1;
}

void check_numbers(char** lines, int y, int x, int *sum)
{
	int ints[8];
	for(int i = 0; i < 8; i++)
		ints[i] = -1;
	int adjacent = 0;

	check_around(lines, x + 1, y, ints);
	check_around(lines, x - 1, y, ints);
	if(!check_around(lines, x, y + 1, ints))
	{
		check_around(lines, x - 1, y + 1, ints);
		check_around(lines, x + 1, y + 1, ints);
	}
	if(!check_around(lines, x, y - 1, ints))
	{
		check_around(lines, x - 1, y - 1, ints);
		check_around(lines, x + 1, y - 1, ints);
	}

	for(int i = 0; i < 8; i++)
	{
		if(ints[i] == -1)
			break;
		adjacent++;
	}

	if(adjacent == 2)
		*sum += ints[0] * ints[1];
}

int symbol(char c)
{
	if(c == '*')
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
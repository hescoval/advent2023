#include "gnl.h"

int start_x;
int start_y;
int line_count;
int line_size;


int valid_position(char current, char from)
{
	if(current == '.')
		return 0;
	if(current == '-' && (from == 'N' || from == 'S'))
		return 0;
	if(current == '|' && (from == 'E' || from == 'W'))
		return 0;
	if(current == 'L' && (from == 'W' || from == 'S'))
		return 0;
	if(current == 'J' && (from == 'E' || from == 'S'))
		return 0;
	if(current == 'F' && (from == 'N' || from == 'W'))
		return 0;
	if(current == '7' && (from == 'N' || from == 'E'))
		return 0;
	return 1;
}

double is_loop(char **input, double steps, int x, int y, char from)
{
	steps++;
	if(x < 0 || x >= line_size || y >= line_count || y < 0)
		return 0;
	char current = input[y][x];
	if(!valid_position(current, from))
		return 0;
	if(current == 'S')
		return steps;

	if(current == '|')
	{
		if(from == 'N')
			return is_loop(input, steps, x, y + 1, 'N');
		else
			return is_loop(input, steps, x, y - 1, 'S');
	}
	if(current == '-')
	{
		if(from == 'W')
			return is_loop(input, steps, x + 1, y, 'W');
		else
			return is_loop(input, steps, x - 1, y, 'E');
	}
	if(current == 'L')
	{
		if(from == 'N')
			return is_loop(input, steps, x + 1, y, 'W');
		else
			return is_loop(input, steps, x, y - 1, 'S');
	}
	if(current == 'J')
	{
		if(from == 'N')
			return is_loop(input, steps, x - 1, y, 'E');
		else
			return is_loop(input, steps, x, y - 1, 'S');
	}
	if(current == '7')
	{
		if(from == 'S')
			return is_loop(input, steps, x - 1, y, 'E');
		else
			return is_loop(input, steps, x, y + 1, 'N');
	}
	if(current == 'F')
	{
		if(from == 'S')
			return is_loop(input, steps, x + 1, y, 'W');
		else
			return is_loop(input, steps, x, y + 1, 'N');
	}
}

double check_distance(char **input)
{
	double furthest = 0;
	double steps_taken = 0;


	steps_taken = is_loop(input, steps_taken, start_x + 1, start_y, 'W');
	if(steps_taken > furthest)
		furthest = steps_taken;
	steps_taken = 0;

 	steps_taken = is_loop(input, steps_taken, start_x - 1, start_y, 'E');
	if(steps_taken > furthest)
		furthest = steps_taken;
	steps_taken = 0;

	steps_taken = is_loop(input, steps_taken, start_x, start_y + 1, 'N');
	if(steps_taken > furthest)
		furthest = steps_taken;
	steps_taken = 0;

	steps_taken = is_loop(input, steps_taken, start_x, start_y - 1, 'S');
	if(steps_taken > furthest)
		furthest = steps_taken; 

	return(furthest);
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
	{
		free(str);
		lines++;
	}
	close(fd);
	return lines;
}

int main()
{
	line_count = count_lines();
	char **input = parse_input(line_count);
	line_size = strlen(input[0]) - 1;
	int found = 0;
	while(!found)
	{
		for(int i = 0; i < line_count; i++)
		{
			for(int j = 0; j < line_size; j++)
			{
				if(input[i][j] == 'S')
				{
					found = 1;
					start_x = j;
					start_y = i;
					break;
				}
			}
			if(found)
				break;
		}
	}
 	double highest = check_distance(input) / 2;
	printf("%.0f", highest);
}
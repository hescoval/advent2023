#include "gnl.h"

int raw_len;
int real_len = 0;
int raw_lines;
int real_lines = 0;

typedef struct g_coord
{
	double x;
	double y;
	int index;
	struct g_coord *next;
}	g_coord;

g_coord* make_node(double x, double y, int index)
{
	g_coord *new = malloc(sizeof(g_coord));

	new->x = x;
	new->y = y;
	new->index = index;
	new->next = NULL;

	return new;
}

char **parse_input(int line_number, char *string)
{
	int i = 0;
	int fd = open(string, O_RDONLY);

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

int no_galaxy(char *str)
{
	for(int i = 0; str[i]; i++)
		if(str[i] == '#')
			return 0;
	return 1;
}

int empty_column(char **raw, int check)
{
	for(int i = 0; raw[i]; i++)
	{
		if(raw[i][check] == '#')
			return 0;
	}
	return 1;
}

int count_lines(char *string)
{
	int fd = open(string, O_RDONLY);
	int lines = 0;
	char *str;

	while(str = get_next_line(fd))
	{
		if(no_galaxy(str))
			real_lines++;
		free(str);
		lines++;
	}
	close(fd);
	return lines;
}

int expanded_len(char **input)
{
	int extra = 0;
	for(int i = 0; i < raw_len; i++)
	{
		int found = 0;
		for(int j = 0; j < raw_lines; j++)
			if(input[j][i] == '#')
				found = 1;
		if(!found)
			extra++;
	}

	return extra;
}

char *make_empty()
{
	char *line = malloc(real_len + 1);

	int i = 0;
	while(i < real_len)
		line[i++] = '.';
	line[i] = '\0';

	return line;
}

void debug(char **raw, char **real)
{
	for(int i = 0; raw[i]; i++)
		printf("%s", raw[i]);
	printf("\n///////////////////////////");
	for(int i = 0; real[i]; i++)
		printf("\n%s", real[i]);
}

void do_math(g_coord *head, int max_i)
{
	g_coord *helper = head;
	int start_i = 1;
	double sum = 0;
	while(start_i < max_i)
	{
		helper = head;
		while(helper->index != start_i && helper)
			helper = helper->next;
		
		double start_x = helper->x;
		double start_y = helper->y;

		helper = helper->next;
		while(helper)
		{
			sum += abs(start_x - helper->x) + abs(start_y - helper->y);
			helper = helper->next;
		}
		start_i++;
	}
	printf("%.0f", sum);
}

void expand_galaxy(char **raw)
{
	int current_real = 0;
	double y_padding = 0;
	double padding = 999999;
	g_coord *head = NULL;
	g_coord *tail = NULL;
	int index = 1;

	for(int i = 0; raw[i]; i++)
	{
		double x_padding = 0;
		if(no_galaxy(raw[i]))
		{
			y_padding += padding;
			continue;
		}
		for(int j = 0; j < raw_len; j++)
		{
			if(empty_column(raw, j))
				x_padding += padding;
			if(raw[i][j] == '#')
			{
				g_coord* current = make_node((double)j + x_padding, (double)i + y_padding, index);
				index++;
				if(head == NULL)
				{
					head = current;
					tail = head;
				}
				else
				{
					tail->next = current;
					tail = tail->next;
				}
			}
		}
	}
	do_math(head, tail->index);
}

int main(int ac, char **av)
{
	raw_lines = count_lines(av[1]);
	char **raw = parse_input(raw_lines, av[1]);
	raw_len = strlen(raw[0]) - 1;
/* 	
	for(int i = 0; get_raw[i]; i++)
		printf("%s", get_raw[i]);
*/
	expand_galaxy(raw);

	printf("\nraw lines is %i", raw_lines);
}
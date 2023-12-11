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

g_coord* make_node(int x, int y, int index)
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

char **expand_galaxy(char **raw)
{
	char **real = malloc(sizeof(char *) * (real_lines + 1));
	int current_real = 0;
	double x_padding = 0;
	double y_padding = 0;
	for(int i = 0; raw[i]; i++)
	{
		if(no_galaxy(raw[i]))
		{
			real[current_real] = make_empty();
			real[current_real + 1] = make_empty();
			current_real += 2;
			continue;
		}

		real[current_real] = malloc(real_len + 1);
		int real_j = 0;
		int j = 0;
		for( j ; j < raw_len; j++)
		{
			if(empty_column(raw, j))
			{
				real[current_real][real_j++] = '.';
				real[current_real][real_j++] = '.';
				continue;
			}
			real[current_real][real_j++] = raw[i][j];
		}
		real[current_real][real_j] = '\0';
		current_real++;
	}

	real[current_real] = NULL;
	return(real);
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
		
		int start_x = helper->x;
		int start_y = helper->y;

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

void find_coords(char **real)
{
	g_coord* head = NULL;
	g_coord* tail = NULL;

	int g_i = 1;
	for(int i = 0; real[i]; i++)
	{
		for(int j = 0; j < real_len; j++)
		{
			if(real[i][j] == '#')
			{
				g_coord *current = make_node(j, i, g_i);
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
				g_i++;
			}
		}
	}
/* 	while(head)
	{
		printf("Index is %i, positions X/Y are %i %i\n", head->index, head->x, head->y);
		head = head->next;
	} 
*/
	do_math(head, tail->index);
}

int main(int ac, char **av)
{
	raw_lines = count_lines(av[1]);
	real_lines += raw_lines;
	char **raw = parse_input(raw_lines, av[1]);
	raw_len = strlen(raw[0]) - 1;
/* 	
	for(int i = 0; get_raw[i]; i++)
		printf("%s", get_raw[i]);
*/
	real_len = expanded_len(raw) + raw_len;
	char **real = expand_galaxy(raw);

	find_coords(real);
	printf("\nreal lines is %i and raw lines is %i", raw_lines, real_lines);
}
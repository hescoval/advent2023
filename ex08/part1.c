#include "gnl.h"

int customlen(char *str)
{
	int i = 0;
	while(str[i] && str[i] != '\n')
		i++;
	return i;
}

typedef struct	directions
{
	char *name;
	char *left;
	char *right;
	struct directions *next;
}	directions;

directions *make_node(char *str, char *left, char *right)
{
	directions *new_node = (directions*)malloc(sizeof(directions));

	new_node->name = strdup(str);
	new_node->left = strdup(left);
	new_node->right = strdup(right);
	new_node->next = NULL;

	return new_node;
}

int get_lines()
{
	int fd = open("test.txt", O_RDONLY);
	int total = 0;
	char *str;

	while(str = get_next_line(fd))
	{
		free(str);
		total++;
	}
	close(fd);

	return total - 1;
}

long long find_steps(directions *head, char *orders)
{
	long long steps = 0;
	int found = 0;
	int len = customlen(orders);
	printf("%i\n", len);
	char *current = "AAA";
	char *end = "ZZZ";
	printf("Starting at %s\n", current);
	while(!found)
	{
		for(int i = 0; i < len; i++)
		{
			directions *helper = head;
			if(strncmp(current, end, 3) == 0)
			{
				found = 1;
				break;
			}
			if(orders[i] == 'L')
			{
				while(helper != NULL)
				{
					if(strncmp(current, helper->name, 3) == 0)
					{
						current = helper->left;
						break;
					}
					helper = helper->next;
				}
			}
			else
			{
				while(helper != NULL)
				{
					if(strncmp(current, helper->name, 3) == 0)
					{
						current = helper->right;
						break;
					}
					helper = helper->next;
				}
			}
			steps++;
		}
	}
	return steps;
}

int main(void)
{
	int count_lines = get_lines();
	int fd = open("test.txt", O_RDONLY);

	directions *head = NULL;
	directions *tail = NULL;

	char *str = get_next_line(fd);
	char *orders = str;

	while(str = get_next_line(fd))
	{
		char **commands = split(str, ',');
		directions *current_node = make_node(commands[0], commands[1], commands[2]);
		directions *helper;
		if(head == NULL)
		{
			head = current_node;
			tail = current_node;
		}
		else
		{
			tail->next = current_node;
			tail = tail->next;
		}
		for(int i = 0; i < 3; i++)
			free(commands[i]);
		free(commands);
		free(str);
	}
	printf("%lli", find_steps(head, orders));
	directions *help;
	while(head)
	{
		help = head->next;
		free(head);
		head = help;
	}
}
#include "gnl.h"

int customlen(char *str)
{
	int i = 0;
	while(str[i] && str[i] != '\n')
		i++;
	return i;
}

long long gcd(long long a, long long b)
{
	if(b == 0)
		return a;
	return(gcd(b, a % b));
}

long long lcm(long long a, long long b)
{
	return llabs(a * b) / gcd(a, b);
}

typedef struct	directions
{
	char *name;
	char *left;
	char *right;
	int is_start;
	int checked;
	int steps;
	struct directions *next;
}	directions;

directions *make_node(char *str, char *left, char *right)
{
	directions *new_node = (directions*)malloc(sizeof(directions));

	new_node->name = strdup(str);
	new_node->left = strdup(left);
	new_node->right = strdup(right);
	new_node->is_start = 0;
	new_node->checked = 0;
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

int find_starts(directions* head)
{
	int starts = 0;

	while(head)
	{
		if(head->name[2] == 'A')
		{
			head->is_start = 1;
			head->checked = 0;
			starts++;
		}
		head = head->next;
	}
	return starts;
}

long long till_z(directions *head, char* current, char *orders)
{
	long long steps = 0;
	int found = 0;
	int len = customlen(orders);
	while(!found)
	{
		for(int i = 0; i < len; i++)
		{
			directions *helper = head;
			if(current[2] == 'Z')
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

long long find_steps(directions *head, char *orders)
{
	long long steps = 0;
	int found = 0;
	int len = customlen(orders);
	int starts = find_starts(head);
	long long *start_steps = malloc(sizeof(long long) * starts);
	while(found < starts)
	{
		directions *helper = head;
		while(helper)
		{
			if(helper->is_start == 1 && helper->checked == 0)
			{
				helper->steps = till_z(head, helper->name, orders);
				helper->checked = 1;
				start_steps[found] = helper->steps;
				found++;
			}
			helper = helper->next;
		}
	}

	long long result = start_steps[0];
	for(int i = 1; i < starts; i++)
		result = lcm(result, start_steps[i]);

	printf("%lld\n", result);
	free(start_steps);
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

	find_steps(head, orders);

	directions *help;

	while(head)
	{
		help = head;
		head = head->next;
		free(head);
	}
	close(fd);
}
#include "gnl.h"

int find_value(int matches)
{
	if (matches == 0)
		return 0;

	int value = 1;
	matches--;

	while(matches)
	{
		value *= 2;
		matches--;
	}
	return value;
}

int *find_nums(char *str, int *nums)
{
	int total = 0;
	char **numbers = split(str, ' ');
	while(numbers[total])
		total++;

	*nums = total;

	int *ret = malloc(sizeof(int) * total);

	for(int i = 0; i < total; i++)
		ret[i] = atoi(numbers[i]);
	
	return ret;
}

int main()
{
	char *str;
	int fd = open("real.txt", O_RDONLY);
	int sum = 0;
	while(str = get_next_line(fd))
	{
		char *helper = split(str, ':')[1];
		char *helper_drawn = split(helper, '|')[1];
		char *helper_winning = split(helper, '|')[0];

		int total_winning = 0;
		int total_drawn = 0;
		int match = 0;

		int *win = find_nums(helper_winning, &total_winning);
		int *draw = find_nums(helper_drawn, &total_drawn);

		for(int i = 0; i < total_drawn; i++)
		{
			for(int j = 0; j < total_winning; j++)
			{
				if(draw[i] == win[j])
				{
					match++;
					break;
				}
			}
		}

		//printf("Number of matches is %i\n", match);
		sum += find_value(match);
	}
	
	printf("%i\n", sum);
}
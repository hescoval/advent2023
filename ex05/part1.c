#include "gnl.h"

int count_nums(char *input)
{
	int total = 0;

	char **numbers = split(input, ' ');
	while(numbers[total])
		total++;

	return total;
}

void	fill_array(long long *nums, char *input)
{
	int total = 0;
	char **num = split(input, ' ');
	while(num[total])
	{
		nums[total] = atoll(num[total]);
		total++;
	}
}

int main(void)
{
	int fd = open("test.txt", O_RDONLY);
	char *str;

	str = get_next_line(fd);
	printf("%s\n", str);
	char *input = split(str, ':')[1];
	int total_seeds = count_nums(input);
	long long seeds[total_seeds];
	fill_array(seeds, input);

	int converted[total_seeds];
	while(str = get_next_line(fd))
	{
		if (!isdigit(str[0]))
		{
			for(int i = 0; i < total_seeds; i++)
				converted[i] = 0;
			continue;
		}

		long long current_nums[3];

		char **filter = split(str, ' ');
		for(int i = 0; filter[i]; i++)
			current_nums[i] = atoll(filter[i]);

		for(int i = 0; i < total_seeds; i++)
		{
			long long subtraction = seeds[i] - current_nums[1];
			long long range = current_nums[2];

			if(subtraction < range && subtraction >= 0 && converted[i] == 0)
			{
				seeds[i] = current_nums[0] + subtraction;
				converted[i] = 1;
			}
		}
	}
	
	long long lowest = seeds[0];
	for(int i = 0; i < total_seeds; i++)
	{
		if(seeds[i] < lowest)
			lowest = seeds[i];
	}

	close(fd);
	printf("%lld\n", lowest);
}
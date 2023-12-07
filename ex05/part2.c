#include "gnl.h"

long long allnums = 0;
long long nums_tested = 0;

int count_nums(char *input)
{
	int total = 0;

	char **numbers = split(input, ' ');
	while(numbers[total])
	{
		free(numbers[total]);
		total++;
	}

	free(numbers);

	return total;
}

void	find_numbers(long long *array, char *input)
{
	int total = 0;
	char **nums = split(input, ' ');
	while(nums[total])
	{
		array[total] = atoll(nums[total]);
		free(nums[total]);
		total++;
	}
	free(nums);
}

void	find_starts(long long *array, long long *input, int size)
{
	int current_start = 0;

	for(int i = 0; i < size; i += 2)
		array[current_start++] = input[i];
}

void	find_ranges(long long	*array, long long *input, int size)
{
	int current_range = 0;

	for(int i = 1; i < size; i += 2)
		array[current_range++] = input[i];
}

int	isseed(long long *starts, long long *ranges, int total_ranges, long long seed)
{
	for(int i = 0; i < total_ranges; i++)
	{
		if(seed >= starts[i] && seed < starts[i] + ranges[i])
			return 1;
	}
	return 0;
}

int isdest(long long src, long long range)
{
	if(nums_tested >= src && nums_tested < src + range)
		return 1;
	return 0;
}

void	find_lowest(long long *starts, long long *ranges, int total_ranges)
{
	char *line;
	int converted = 0;
	int valid_seed = 0;
	long long curr = 0;
		for(int j = 0; valid_seed == 0; j++)
		{
			int total_conversions = 0;
			int valid = 0;
			int fd = open("test.txt", O_RDONLY);
			long long current_num = nums_tested;
			while(line = get_next_line(fd))
			{
				if(!isdigit(line[0]))
				{
					converted = 0;
					total_conversions++;
					free(line);
					continue;
				}
				long long line_numbers[3];
				find_numbers(line_numbers, line);

				long long base_dest = line_numbers[1];
				long long base_src = line_numbers[0];
				long long base_range = line_numbers[2];
				if(isdest(base_src, base_range))
					valid = 1;
				long long subtr = current_num - base_src;
				if(subtr < base_range && subtr >= 0 && converted == 0)
				{
					current_num = base_dest + subtr;
					converted = 1;
				}
				free(line);
			}
			if(isseed(starts, ranges, total_ranges, current_num) && valid)
			{
				printf("seed found is %lld\n", current_num);
				printf("location found is %lli\n", nums_tested);
				valid_seed = 1;
				break;
			}
			printf("Testing location %lld\n", nums_tested);
			nums_tested++;
			close(fd);
		}
}

int main(void)
{
	int fd = open("seedstest.txt", O_RDONLY);
	char *seed_file;
	seed_file = get_next_line(fd);
	char **input = split(seed_file, ':');
	char	*values = input[1];

//
	free(seed_file);
	free(input[0]);
	free(input);
	close(fd);
//

	int total_nums = count_nums(values);
	long long	numbers[total_nums];
	find_numbers(numbers, values);

	long long start_values[total_nums / 2];
	long long ranges[total_nums / 2];

	find_starts(start_values, numbers, total_nums);
	find_ranges(ranges, numbers, total_nums);
	
	for(int i = 0; i < total_nums / 2; i++)
		allnums += ranges[i];

	find_lowest(start_values, ranges, total_nums / 2);
	printf("%lli", nums_tested);
	free(values);
}
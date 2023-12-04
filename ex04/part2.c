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

int count_lines()
{
	int fd = open("real.txt", O_RDONLY);
	int lines = 0;
	char *str;

	while(str = get_next_line(fd))
		lines++;
	close(fd);
	return lines;
}

int main()
{
	char *str;
	int total = count_lines();

	int card_value[total];
	int card_amounts[total];

	for(int i = 0; i < total; i++)
		card_amounts[i] = 1;

	int card = 0;
	int fd = open("real.txt", O_RDONLY);
//	int sum = 0;

	int total_cards = 0;

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

		card_value[card] = find_value(match);
		
		for(int i = 1; match; i++)
		{
			int copies = card_amounts[card];
			if(card + i < total)
			{
				while(copies)
				{
					card_amounts[card + i]++;
					copies--;
				}
			}
			match--;
		}
		card ++;
	}

/* 	
	#########		Bit of a blunder while reading the subject		########

	for(int i = 0; i < total; i++)
	{
		printf("Card %i is worth %i points\n", i + 1, card_value[i]);
		sum += card_value[i] * card_amounts[i];
		printf("Sum is %i\n", sum);
	} 
*/

	for(int i = 0; i < total; i++)
		printf("Card %i has %i copies(including original)\n", i + 1, card_amounts[i]);

	for(int i = 0; i < total; i++)
		total_cards += card_amounts[i];

	printf("%i\n", total_cards);
}
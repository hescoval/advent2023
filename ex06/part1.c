#include "gnl.h"

int main(void)
{
	int fd = open("test.txt", O_RDONLY);

	int race_times[4];
	int race_distances[4];

	char *line = get_next_line(fd);
	char **all_times = split(line, ' ');
	for(int i = 0; all_times[i]; i++)
		race_times[i] = atoi(all_times[i]);
	line = get_next_line(fd);
	char **all_distances = split(line, ' ');
	for(int i = 0; all_distances[i]; i++)
		race_distances[i] = atoi(all_distances[i]);

	int ways[4];

	for(int i = 0; i < 4; i++)
	{
		int possible = 0;
		int total_time = race_times[i];
		int to_beat = race_distances[i];
		int current_speed = 0;

		while(total_time > 0)
		{
			if(current_speed * total_time > to_beat)
				possible++;
			current_speed++;
			total_time--;
		}
		ways[i] = possible;
	}

	int total = 1;
	for(int i = 0; i < 4; i++)
	{
		printf("Total ways for %i is %i\n", i, ways[i]);
		total *= ways[i];
	}
	
	printf("%i", total);
}
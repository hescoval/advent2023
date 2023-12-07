#include "gnl.h"

int main(void)
{
	long long ways = 0;
	long long possible = 0;

	long long total_time = censored;
	long long to_beat = censored;
	long long current_speed = 0;
	int copy = total_time;
	long long lower_boundary = 0;
	long long lower_time = 0;
	long long upper_boundary = 0;
	long long to_decrement = 1000;

	while(1)
	{
		if(total_time < 0)
		{
			current_speed = 0;
			total_time = copy;
			to_decrement = 1;
		}
		printf("%lli\n", total_time);
		if(current_speed * total_time > to_beat)
		{
			while(!lower_boundary)
			{
				if(current_speed * total_time <= to_beat)
				{
					current_speed++;
					total_time--;
					lower_boundary = current_speed;
					lower_time = total_time;
					break;
				}
				current_speed--;
				total_time++;
			}
		}
		if(lower_boundary)
			break;

		current_speed += to_decrement;
		total_time -= to_decrement;
	}

	printf("%lld\n", lower_boundary);
	upper_boundary = copy - lower_boundary;
	printf("%lli\n", upper_boundary);
	printf("Total ways is %lli\n", upper_boundary - lower_boundary + 1);
}
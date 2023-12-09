#include "gnl.h"
#include <time.h>

typedef struct node
{
	double *array;
	int size;
	struct node* next;
	struct node* prev;
}	node;

node*	make_head(double *numbers, int size)
{
	node *new = (node*)malloc(sizeof(node));
	double *array = numbers;

/* 	for(int i = 0; i < size - 1; i++)
		printf("[%.0f] ", array[i]);
	printf("\n"); */

	new->array = array;
	new->size = size;
	new->next = NULL;
	new->prev = NULL;
}

node*	make_array(double *numbers, int size)
{
	node *new = (node*)malloc(sizeof(node));
	double *array = malloc(sizeof(double) * (size));

	for(int i = 0; i < size - 1; i++)
		array[i] = numbers[i+1] - numbers[i];

/* 	for(int i = 0; i < size - 1; i++)
		printf("[%.0f] ", array[i]);
	printf("\n"); */

	new->array = array;
	new->size = size - 1;
}

int all_zero(double *numbers, int size)
{
	for(int i = 0; i < size; i++)
		if(numbers[i] != 0)
			return 0;
	return 1;
}

double find_last(node *last, int o_size)
{
	//Last element of previous array - x == 0;
	// x = 0 + last;
	// 0 = x;
	int result = 0;
	int current_size = last->size;
	while(current_size <= o_size)
	{
		result = last->array[0] - result;
		last = last->prev;
		current_size++;
	}
	return(result);
}

double deconstruct(double *numbers, int size)
{
	node *head = make_head(numbers, size);
	node *tail = head;
	int original = size;
	int copies_made = 0;
	while(!all_zero(numbers, size))
	{
		node *next = make_array(numbers, size);
		numbers = next->array;
		next->prev = tail;
		tail->next = next;
		tail = tail->next;
		size--;
	}
	return(find_last(tail, original));
}

int count_n(char **split)
{
	int i = 0;

	while(split[i])
		i++;
	return i;
}

int main(int ac, char **av)
{
	clock_t start, end;
	start = clock();
	int fd = open(av[1], O_RDONLY);
	char *str;
	double sum = 0;
	while(str = get_next_line(fd))
	{
		char **split_numbers = split(str, ' ');
		int size = count_n(split_numbers);
		double *numbers = malloc(sizeof(double) * size);

		for(int i = 0; split_numbers[i]; i++)
			numbers[i] = atoll(split_numbers[i]);

		double result = deconstruct(numbers, size);
		free(numbers);
		for(int i = 0; split_numbers[i]; i++)
			free(split_numbers[i]);
		sum += result;
		free(str);
	}


	printf("%.0f\n", sum);
	end = clock();
	double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
	printf("Time taken was %f", time_taken);
}
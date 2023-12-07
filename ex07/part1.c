#include "gnl.h"

char	*cards = "23456789TJQKA";
char	*types[] = {"High Card", "One Pair", "Two Pair", "Three of a Kind", "Full House", "Four of a kind", "Five of a Kind"};

typedef struct hand_info
{
	char *hand;
	int bid;
	int type;
	int changed;
	int rank;
	struct hand_info* next;
	struct hand_info* prev;
}	hand_info;

hand_info *make_node(char *str, int type, int bid)
{
	hand_info *new_node = (hand_info*)malloc(sizeof(hand_info));

	new_node-> hand = strdup(str);
	new_node->type = type;
	new_node->bid = bid;
	new_node->rank = 0;
	new_node->next = NULL;
	new_node->prev = NULL;

	return new_node;
}

int	line_amount()
{
	int fd = open("test.txt", O_RDONLY);

	char *str;
	int total = 0;

	while(str = get_next_line(fd))
	{
		free(str);
		total++;
	}
	close(fd);
	return total;
}

char** get_lines(int line_total)
{
	int fd = open("test.txt", O_RDONLY);

	char *str;
	char **strings = malloc(sizeof(char *) * (line_total));

	int i = 0;

	while(str = get_next_line(fd))
		strings[i++] = str;

	strings[i] = NULL;
	close(fd);
	return strings;
}

void	count_cards(char *hand, int *array)
{
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; cards[j]; j++)
		{
			if(hand[i] == cards[j])
			{
				array[j]++;
				break;
			}
		}
	}
}

int check_type(int *cards)
{
	int highest = 0;
	for(int i = 0; i < 13; i++)
	{
		if(highest == 2 && cards[i] == 2)
			return (2);
		if((highest == 3 && cards[i] == 2) || (highest == 2 && cards[i] == 3))	
			return (4);
		if(cards[i] == 4)
			return (5);
		if(cards[i] == 5)
			return (6);
		if(cards[i] > highest)
			highest = cards[i];
	}
	if(highest == 3)
		return 3;
	if(highest)
		return (highest - 1);
	return 0;
}

void swap_nodes(hand_info **head, hand_info *node1, hand_info *node2)
{
    if (node1->prev != NULL)
        node1->prev->next = node2;
    else
        *head = node2;

    if (node2->next != NULL)
        node2->next->prev = node1;

    if (node1->next == node2)
    {
        node1->next = node2->next;
        node2->prev = node1->prev;
        node2->next = node1;
        node1->prev = node2;
    }
    else
    {
        hand_info *temp = node1->prev;
        node1->prev = node2->prev;
        node2->prev = temp;

        temp = node1->next;
        node1->next = node2->next;
        node2->next = temp;
    }
}

int c_i(char hand)
{
	for(int j = 0; cards[j]; j++)
	{
		if(hand == cards[j])
			return j;
	}
}

void	sort_by_type(hand_info** head)
{
	int swapped = 1;
	int howmany = 0;
	while(swapped)
	{
		swapped = 0;
		hand_info *helper = *head;

		while(helper->next != NULL)
		{
			if(helper->type > helper->next->type)
			{
				swap_nodes(head, helper, helper->next);
				swapped = 1;
				helper = helper->prev;
			}
			helper = helper->next;
		}
	}
}

int	compare_hands(char *hand1, char *hand2)
{
	for(int i = 0; i < 5; i++)
	{
		if(c_i(hand1[i]) < c_i(hand2[i]))
			return 0;
		if(c_i(hand1[i]) > c_i(hand2[i]))
			return 1;
	}
	return 0;
}

void	sort_by_cards(hand_info **head)
{
	int swapped = 1;

	while(swapped)
	{
		swapped = 0;
		hand_info *helper = *head;

		while(helper->next != NULL)
		{
			if(helper->type == helper->next->type && compare_hands(helper->hand, helper->next->hand))
			{
				swap_nodes(head, helper, helper->next);
				swapped = 1;
				helper=helper->prev;
			}
			helper = helper->next;
		}
	}
}

int main(void)
{
	char	*str;
	int line_total = line_amount();
	char **lines = get_lines(line_total);
	hand_info *head = NULL;

	double sum = 0;
	for(int i = 0; i < line_total; i++)
	{
		hand_info *current_hand;

		int card_amount[13];
		for(int j = 0 ; j < 13 ; j++)
			card_amount[j] = 0;
		char **current = split(lines[i], ' ');
		char *hand = current[0];
		int bid = atoi(current[1]);
		count_cards(hand, card_amount);

		current_hand = make_node(hand, check_type(card_amount), bid);
		if(head == NULL)
			head = current_hand;
		else
		{
			hand_info *helper = head;
			while(helper->next != NULL)
				helper = helper->next;
			current_hand->prev = helper;
			helper->next = current_hand;
		}

		free(hand);
		free(current[1]);
		free(current);
	}
	
	sort_by_type(&head);
	sort_by_cards(&head);
	int rank = 1;

	while(head != NULL)
	{
		head->rank = rank;
		printf("Hand is %s, type is %s, bid is %i and it's final rank is %i\n", head->hand, types[head->type], head->bid, rank);
		sum += head->bid * rank;
		rank++;
		head = head->next;
	}

	printf("Total winning are %f", sum);
}
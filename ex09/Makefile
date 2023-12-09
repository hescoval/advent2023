PART_ONE = part1.c
PART_TWO = part2.c
OBJS = *.o
CC = cc -g

part1: $(PART_ONE)
	$(CC) -o part1 $(PART_ONE) gnl.c

part2:	$(PART_TWO)
	$(CC) -o part2 $(PART_TWO) gnl.c

clean:
	rm -f $(OBJS) part1 part2

make:
	make part1
	make part2
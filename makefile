all:
	g++ -Wall -Wextra -g -ggdb3 -D_DEBUG -O3  -fsanitize=address my_cache.cpp -o my_cache
# -D_DEBUG
# 5 40 1 1 8 8 3 3 3 9 9 2 10 8 3 6 8 8 10 5 10 9 4 5 1 1 7 3 6 2 8 9 6 2 1 6 4 9 3 2 5 4

all:
	g++ -Wall -Wextra -g -ggdb3 -fsanitize=address my_cache.cpp -o my_cache

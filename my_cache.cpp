#include <cstring>
#include <iostream>
#include <fstream>

#include <cassert>
#include <iostream>
#include <vector>

#include "my_cache.hpp"

//***C++11 Style:***
#include <chrono>

// slow get page imitation
int slow_get_page_int(int key)
{
    return key;
}

int main()
{
    int lru_hits = 0;
    int ora_hits = 0;

    int number_of_queries;
    size_t cacheline_size;

    std::cin >> cacheline_size >> number_of_queries;
    assert(std::cin.good());

    caches::CacheLRU<int> cache{cacheline_size};


    int queries[number_of_queries];
    for (int cnt = 0; cnt != number_of_queries; ++cnt)
    {
        std::cin >> queries[cnt];
        assert(std::cin.good());
    }

    std::vector<int> queries_vector;
    // memcpy(queries_vector.data(), queries, number_of_queries * sizeof (int));
    queries_vector.insert(queries_vector.end(), &queries[0], &queries[number_of_queries]);

    caches::CacheORA<int> better_cache{cacheline_size, queries_vector};


    std::chrono::steady_clock::time_point lru_clock_begin = std::chrono::steady_clock::now();
    for (int cnt = 0; cnt != number_of_queries; ++cnt)
    {
        if (cache.lookup_update(queries[cnt], slow_get_page_int))
        {
            ++lru_hits;
        }
    }
    std::chrono::steady_clock::time_point lru_clock_end = std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point ora_clock_begin = std::chrono::steady_clock::now();
    for (int cnt = 0; cnt != number_of_queries; ++cnt)
    {
        if (better_cache.lookup_update(queries[cnt], slow_get_page_int))
        {
            ++ora_hits;
        }
    }
    std::chrono::steady_clock::time_point ora_clock_end = std::chrono::steady_clock::now();


    std::cout << std::endl  << "LRU hits: " << lru_hits << "/" << number_of_queries << std::endl
                            << "ORA hits: " << ora_hits << "/" << number_of_queries << std::endl;

    std::cout << "LRU time: " << std::chrono::duration_cast<std::chrono::microseconds>(lru_clock_end - lru_clock_begin).count() << "[µs]" << std::endl;
    std::cout << "ORA time: " << std::chrono::duration_cast<std::chrono::microseconds>(ora_clock_end - ora_clock_begin).count() << "[µs]" << std::endl;

    #ifdef _DEBUG
    std::cout << "MIND THAT DEBUG FLAG IS ACTIVE (ORA results are not fair)" << std::endl;
    #endif
}

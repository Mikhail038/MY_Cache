//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example for LRU cache in C++
//
//----------------------------------------------------------------------------

#pragma once

#include <iostream>

#include <array>
#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename ValueT, typename KeyT = int> struct CacheLRU
{
    size_t size_;
    std::list<std::pair<KeyT, ValueT>> cachelist_;

    using list_it = typename std::list<std::pair<KeyT, ValueT>>::iterator;
    std::unordered_map<KeyT, list_it> hash_;

    //================================================================================================================================================================================

    CacheLRU(size_t size) :
    size_(size)
    {}

    //================================================================================================================================================================================

    bool full() const
    {
        return (cachelist_.size() == size_);
    }

    void free_spot_in_cacheline ()
    {
        hash_.erase(cachelist_.back().first);
        cachelist_.pop_back();
    }

    //================================================================================================================================================================================

    template <typename FunctionT>
    bool lookup_update(KeyT key, FunctionT slow_get_page)
    {
        auto hit = hash_.find(key);
        if (hit == hash_.end())
        {
            if (full())
            {
                free_spot_in_cacheline ();
            }

            cachelist_.emplace_front(key, slow_get_page(key));
            hash_.emplace(key, cachelist_.begin());

            return false;
        }

        auto hited_value = hit->second;
        if (hited_value != cachelist_.begin())
        {
            cachelist_.splice(cachelist_.begin(), cachelist_, hited_value, std::next(hited_value));
        }

        return true;
    }
};

//================================================================================================================================================================================

template <typename ValueT, typename KeyT = int> struct CacheORA //ORA for ORAcle
{
    size_t size_;
    std::vector<ValueT> request_array_;
    int current_request_num_;

    std::list<std::pair<KeyT, ValueT>> cachelist_;

    using list_it = typename std::list<std::pair<KeyT, ValueT>>::iterator;
    std::unordered_map<KeyT, list_it> hash_;

    //================================================================================================================================================================================

    CacheORA(size_t size, std::vector<ValueT> request_array) :
    size_(size),
    request_array_(request_array),
    current_request_num_(0)
    {}
    //================================================================================================================================================================================

    void print_cacheline ()
    {
        if (cachelist_.begin() == cachelist_.end())
        {
            std::cout << "[ ]";
        }

        for (list_it cnt = cachelist_.begin(); cnt != cachelist_.end(); ++cnt)
        {
            std::cout << "[" << cnt->first << "]";
        }

        std::cout << std::endl;
    }

    //================================================================================================================================================================================

    int find_next_request (list_it current_spot)
    {
        for (int cnt = current_request_num_; cnt != request_array_.size(); ++cnt)
        {
            if (request_array_[cnt] == current_spot->first)
            {
                //std::cout << "[" << cnt << "]" << std::endl;
                return cnt;
            }
        }

        return request_array_.size() + 1;
    }

    //================================================================================================================================================================================

    bool full() const
    {
        return (cachelist_.size() == size_);
    }

    void free_spot_in_cacheline ()
    {
        auto useless_spot = cachelist_.begin();
        int useless_spot_time = find_next_request(cachelist_.begin());
        int this_spot_time = 0;

        for (list_it cnt = cachelist_.begin(); cnt != cachelist_.end() ; ++cnt)
        {
            this_spot_time = find_next_request (cnt);

            #ifdef _DEBUG
            std::cout << "[" << (*cnt).first << "] time " << this_spot_time << " largest time " << useless_spot_time << std::endl;
            #endif

            if (this_spot_time > useless_spot_time)
            {
                useless_spot = cnt;
                useless_spot_time = this_spot_time;
            }
        }

        hash_.erase((*useless_spot).first);
        cachelist_.erase(useless_spot);
    }

    template <typename FunctionT>
    bool lookup_update(KeyT key, FunctionT slow_get_page)
    {
        #ifdef _DEBUG
        print_cacheline();

        std::cout << "request " << key << std::endl;
        #endif

        auto hit = hash_.find(key);
        if (hit == hash_.end())
        {
            if (full())
            {
                free_spot_in_cacheline ();
            }

            cachelist_.emplace_front(key, slow_get_page(key));
            hash_.emplace(key, cachelist_.begin());

            #ifdef _DEBUG
            std::cout << "no hit " << std::endl;
            #endif

            ++current_request_num_;
            return false;
        }

        auto hited_value = hit->second;
        // if (hited_value != cachelist_.begin())
        // {
        //     cachelist_.splice(cachelist_.begin(), cachelist_, hited_value, std::next(hited_value));
        // }

        #ifdef _DEBUG
        std::cout << "hit " <<  (*hited_value).second << std::endl;
        #endif


        ++current_request_num_;
        return true;
    }
};

} // namespace caches

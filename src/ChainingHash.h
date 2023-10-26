/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

// Standard library includes
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

// Custom project includes
#include "Hash.h"

//
// Separate chaining based hash table - derived from Hash
//

template <typename K, typename V>
class ChainingHash : public Hash<K, V>
{
private:
    std::vector<std::list<std::pair<K, V>>> m_table;
    int m_count = 0;

public:
    ChainingHash(int n = 11)
    {
        n = findNextPrime(n);
        m_table.resize(n);
    }

    ~ChainingHash()
    {
        this->clear();
    }

    int size() override
    {
        return m_count;
    }

    V &operator[](const K &key) override
    {
        int index = hash(key);
        for (auto &item : m_table[index])
        {
            if (item.first == key)
            {
                return item.second;
            }
        }
        m_table[index].emplace_back(key, V{});
        m_count++;
        return m_table[index].back().second;
    }

    bool insert(const std::pair<K, V> &pair) override
    {
        auto &bucket = m_table[hash(pair.first)];
        bucket.emplace_back(pair.first, V{});
        V &value = bucket.back().second;
        if (value != V{})
        {
            return false;
        }
        value = pair.second;
        m_count++;

        if (load_factor() > 0.75) {
            rehash();
        }

        return true;
    }

    void erase(const K &key) override
    {
        int index = hash(key);
        for (auto it = m_table[index].begin(); it != m_table[index].end(); ++it)
        {
            if (it->first == key)
            {
                m_table[index].erase(it);
                m_count--;
                return;
            }
        }
    }

    void clear() override
    {
        m_table.clear();
        m_count = 0;
    }

    int bucket_count() override
    {
        return m_table.size();
    }

    float load_factor() override
    {
        return static_cast<float>(m_count) / m_table.size();
    }

private:

    void rehash() {
        std::vector<std::list<std::pair<K, V>>> oldTable = m_table;
        m_table.clear();
        m_table.resize(findNextPrime(oldTable.size() * 2));
        for (auto &bucket : oldTable) {
            for (auto &item : bucket) {
                insert(item);
            }
        }
    }

    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    bool isPrime(int n)
    {
        if (n <= 1)
        {
            return false;
        }
        for (int i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }
        return true;
    }

    int hash(const K &key) override
    {
        return std::hash<K>()(key) % m_table.size();
    }
};

#endif //__CHAINING_HASH_H
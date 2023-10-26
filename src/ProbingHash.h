#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>
#include <cmath>
#include <optional>

#include "Hash.h"

template<typename K, typename V>
class ProbingHash : public Hash<K,V> {
private:
    std::vector<std::pair<std::optional<K>, V>> m_table;
    int m_count = 0;

public:
    ProbingHash(int n = 11) {
        n = findNextPrime(n);
        m_table.resize(n);
    }

    ~ProbingHash() {
        this->clear();
    }

    int size() override {
        return m_count;
    }

    V& operator[](const K& key) override {
        int index = hash(key);
        int count = 0;
        while (count < m_table.size()) {
            if (!m_table[index].first) {
                m_table[index].first = key;
                m_count++;
                return m_table[index].second;
            }
            if (m_table[index].first == key) {
                return m_table[index].second;
            }
            index = (index + 1) % m_table.size();
            count++;
        }
        return operator[](key);
    }

    bool insert(const std::pair<K, V>& pair) override {
        V& value = operator[](pair.first);
        if (value != V{}) {
            return false;
        }
        value = pair.second;
        m_count++;

        if (load_factor() > 0.75) {
            rehash();
        }

        return true;
    }

    void erase(const K& key) override {
        int index = find(key);
        if (index != -1) {
            m_table[index].first = std::nullopt;
            m_count--;
        }
    }

    void clear() override {
        m_table.clear();
        m_count = 0;
    }

    int bucket_count() override {
        return m_table.size();
    }

    float load_factor() override {
        return static_cast<float>(m_count) / m_table.size();
    }

private:

    void rehash() {
        std::vector<std::pair<std::optional<K>, V>> oldTable = m_table;
        m_table.clear();
        m_table.resize(findNextPrime(oldTable.size() * 2));
        m_count = 0;
        for (auto& item : oldTable) {
            if (item.first) {
                insert(std::make_pair(*item.first, item.second));
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
        if (n <= 1) {
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

    int hash(const K& key) override {
        return std::hash<K>()(key) % m_table.size();
    }

    int find(const K& key) {
        int index = hash(key);
        int count = 0;
        while (count < m_table.size()) {
            if (m_table[index].first == key) {
                return index;
            }
            if (!m_table[index].first) {
                return -1;
            }
            index = (index + 1) % m_table.size();
            count++;
        }
        return -1;
    }
};

#endif //__PROBING_HASH_H
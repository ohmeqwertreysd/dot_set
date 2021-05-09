// Заголовочный файл hashtable_struct.h, который содержит объявление и определение шаблонного класса Хеш-таблицы HashTable
#pragma once
#ifndef HASHTABLE_STRUCT_H
#define HASHTABLE_STRUCT_H
#include "list_struct.h"
#include <stdexcept>
namespace ds
{
    constexpr double epsilon_hash = 1E5; // Точность для перевода вещественного числа в натуральное для хеш-функции

    template<class T, class U>
    struct Pair
    {
        T first;
        U second;
    };

    template<class T, class U>
    class HashTable
    {
    public:
        HashTable(const size_t& max_size = 200U);
        ~HashTable();
        void insert(const Pair<T, U>&);
        int find(const T&);
        List<U>* equal_range(const T&);
    protected:
        long hash(const long&) const;
        long index(const long&) const;
    private:
        List<T>* _key;
        List<List<U>*>* _value;

        size_t _max_size;
    };

    template<class T, class U>
    HashTable<T, U>::HashTable(const size_t& max_size) : _key(nullptr), _max_size(max_size)
    {
        _key = new List<T>[_max_size];
        _value = new List<List<U>*>[_max_size];
    }

    template<class T, class U>
    HashTable<T, U>::~HashTable()
    {
        delete[] _key;
        delete[] _value;
    }

    template<class T, class U>
    void HashTable<T, U>::insert(const Pair<T, U>& pair)
    {
        long x = static_cast<long>(pair.first * epsilon_hash);
        long i = index(hash(x));
        int pos = _key[i].find(pair.first);
        if (pos != -1)
        {
            _value[i][pos]->push_back(pair.second);
        }
        else
        {
            _key[i].push_back(pair.first);
            List<U>* value = new List<U>;
            value->push_back(pair.second);
            _value[i].push_back(value);
        }
    }

    template<class T, class U>
    long HashTable<T, U>::HashTable::hash(const long& x) const
    {
        return (x >> 15) ^ x;
    }
    template<class T, class U>
    long HashTable<T, U>::index(const long& hash) const
    {
        return abs(hash) % this->_max_size;
    }
    template<class T, class U>
    int HashTable<T, U>::find(const T& key)
    {
        long x = static_cast<long>(key * epsilon_hash);
        long i = index(hash(x));
        return _key[i].find(key);
    }
    template<class T, class U>
    List<U>* HashTable<T, U>::equal_range(const T& key)
    {
        long x = static_cast<long>(key * epsilon_hash);
        long i = index(hash(x));
        int pos = _key[i].find(key);
        return _value[i][pos];
    }

}

#endif // !HASHTABLE_STRUCT_H
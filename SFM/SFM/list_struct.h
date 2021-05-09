// Заголовочный файл list_struct.h, который содержит объявление и определение шаблонного класса однонаправленного связного списка List
#pragma once
#ifndef LIST_STRUCT_H
#define LIST_STRUCT_H
#include <stdexcept>
namespace ds
{
    template <class T>
    class Node
    {
    public:
        Node(const T& data = T(), Node* pNext = nullptr);
        T getData();
    public:
        T _data;
        Node* _ptrNext;
    };

    template <class T>
    class List
    {
    public:
        List();
        ~List();
        size_t size() const;
        bool isEmpty() const;
        void push_back(const T&);
        void push_front(const T&);
        int find(const T&) const;
        T& operator[](const size_t&);
    private:
        Node<T>* _begin;
        size_t _size;
    };

    template<class T>
    Node<T>::Node(const T& data, Node* pNext) : _data(data), _ptrNext(pNext) {}

    template<class T>
    T Node<T>::getData()
    {
        return this->_data;
    }

    template<class T>
    List<T>::List() : _begin(nullptr), _size(0U) {}

    template<class T>
    List<T>::~List()
    {
        Node<T>* current = this->_begin;
        while (current != nullptr)
        {
            Node<T>* tmp = current;
            current = current->_ptrNext;
            delete tmp;
        }
    }

    template<class T>
    size_t List<T>::size() const
    {
        return this->_size;
    }

    template<class T>
    bool List<T>::isEmpty() const
    {
        return _begin == nullptr;
    }

    template<class T>
    void List<T>::push_back(const T& data)
    {
        if (_begin == nullptr)
            _begin = new Node<T>(data);
        else
        {
            Node<T>* current = this->_begin;
            while (current->_ptrNext != nullptr)
                current = current->_ptrNext;
            current->_ptrNext = new Node<T>(data);
        }
        this->_size++;
    }

    template<class T>
    void List<T>::push_front(const T& data)
    {
        this->_begin = new Node<T>(data, this->_begin);
        _size++;
    }

    template<class T>
    int List<T>::find(const T& data) const
    {
        int count = 0;
        Node<T>* current = this->_begin;
        while (current != nullptr)
        {
            if (current->_data == data)
                return count;
            current = current->_ptrNext;
            count++;
        }
        return -1;
    }

    template<class T>
    T& List<T>::operator[](const size_t& index)
    {
        if (index >= this->_size)
            throw std::logic_error("Index >= size");
        size_t count = 0U;
        Node<T>* current = this->_begin;
        while (current != nullptr)
        {
            if (count == index)
                return current->_data;
            current = current->_ptrNext;
            count++;
        }
        return this->_begin->_data;
    }
}

#endif // !LIST_STRUCT_H

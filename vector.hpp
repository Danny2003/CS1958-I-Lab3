#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 * thanks to google.com
 * thanks to RandomAccessIterator at CppReference.
 * thanks to CSDN
 * thanks to Shu Ze Chen, Alan Liang, Xiao Yang Lu and other classmates.
 */
template<typename T>
class vector {
public:
    ///current size of the vector
    size_t vectorSize;
    ///current maximal capacity of the vector
    size_t capacity;
    ///a secondary pointer which points to a pointer array
    T **storage;
    class const_iterator;
    class iterator {
    private:
        ///a pointer which points to the vector which the iterator belongs to
        vector<T> *vectorStorage;
        T **pointer;
    public:
        iterator(vector<T> *vec, T **ptr):vectorStorage(vec), pointer(ptr) {}
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        iterator operator+(const int &n) const {
            iterator tmp(vectorStorage, pointer + n);
            return tmp;
        }
        iterator operator-(const int &n) const {
            iterator tmp(vectorStorage, pointer - n);
            return tmp;
        }
        /// return the distance between two iterators,
        /// if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const {
            if (vectorStorage != rhs.vectorStorage) throw invalid_iterator();
            int tmp;
            tmp = pointer - rhs.pointer;
            return tmp;
        }
        iterator& operator+=(const int &n) {
            pointer += n;
            return *this;
        }
        iterator& operator-=(const int &n) {
            pointer -= n;
            return *this;
        }
        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp(vectorStorage, pointer);
            ++pointer;
            return(tmp);
        }
        /**
         * ++iter
         */
        iterator& operator++() {
            ++pointer;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp(vectorStorage, pointer);
            --pointer;
            return(tmp);
        }
        /**
         * --iter
         */
        iterator& operator--() {
            --pointer;
            return *this;
        }
        /**
         * *it
         */
        T& operator*() const{return **pointer;}
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return (pointer == rhs.pointer);
        }
        bool operator==(const const_iterator &rhs) const {
            return (pointer == rhs.pointer);
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return (pointer != rhs.pointer);
        }
        bool operator!=(const const_iterator &rhs) const {
            return (pointer != rhs.pointer);
        }
    ///the pointer of the iterator is needed in functions in the vector.
    ///notice that the outer class is not the friend class of the inner class.
    friend class vector<T>;
    ///operater== and operator!= needs const_iterator
    friend class const_iterator;
    };
    /**
     * has same function as iterator, just for a const object.
     */
    class const_iterator {
    private:
        const vector<T> *vectorStorage;
        T **pointer;
    public:
        const_iterator(const vector<T> *vec, T **ptr):vectorStorage(vec), pointer(ptr) {}
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        const_iterator operator+(const int &n) const {
            const_iterator tmp(vectorStorage, pointer + n);
            return tmp;
        }
        const_iterator operator-(const int &n) const {
            const_iterator tmp(vectorStorage, pointer - n);
            return tmp;
        }
        /// return the distance between two iterators,
        /// if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            if (vectorStorage != rhs.vectorStorage) throw invalid_iterator();
            int tmp;
            tmp = pointer - rhs.pointer;
            return tmp;
        }
        const_iterator& operator+=(const int &n) {
            pointer += n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            pointer -= n;
            return *this;
        }
        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator tmp(vectorStorage, pointer);
            ++pointer;
            return(tmp);
        }
        /**
         * ++iter
         */
        const_iterator& operator++() {
            ++pointer;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator tmp(vectorStorage, pointer);
            --pointer;
            return(tmp);
        }
        /**
         * --iter
         */
        const_iterator& operator--() {
            --pointer;
            return *this;
        }
        /**
         * *it
         */
        ///cannot change the value of the object which the const_iterator points to
        const T& operator*() const{return **pointer;}
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return (pointer == rhs.pointer);
        }
        bool operator==(const const_iterator &rhs) const {
            return (pointer == rhs.pointer);
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return (pointer != rhs.pointer);
        }
        bool operator!=(const const_iterator &rhs) const {
            return (pointer != rhs.pointer);
        }
    ///the pointer of the iterator is needed in functions in the vector.
    ///notice that the outer class is not the friend class of the inner class.
    friend class vector<T>;
    ///operater== and operator!= needs iterator
    friend class iterator;
    };
    /**
     * Constructor
     * default constructor, copy constructor
     */
    vector():vectorSize(0), capacity(8), storage(new T* [capacity]) {}
    vector(const vector &other):vectorSize(other.vectorSize), capacity(other.capacity), storage(new T* [other.capacity]) {
        for (size_t i = 0; i < vectorSize; ++i)
        {
            //下标运算符优先级高于*运算符
            storage[i] = new T(*other.storage[i]);
        }
    }
    /**
     * Destructor
     */
    ~vector() {
        for (size_t i = 0; i < vectorSize; ++i) delete storage[i];
        if (storage) 
        {
            delete [] storage;
            storage = nullptr;
        }
    }
    /**
     * Assignment operator
     */
    vector &operator=(const vector &other) {
        if (this == &other) return *this;
        for (size_t i = 0; i < vectorSize; ++i) delete storage[i];
        if (storage) delete [] storage;
        vectorSize = other.vectorSize;
        capacity = other.capacity;
        storage = new T* [capacity];
        for (size_t i = 0; i < vectorSize; ++i)
        {
            storage[i] = new T(*other.storage[i]);
        }
        return *this;
    }
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     */
    T & at(const size_t &pos) {
        if (pos < 0 || pos >= vectorSize) throw index_out_of_bound();
        return *storage[pos];
    }
    ///used by const vector
    //返回一个常量的引用，不能通过引用改变所引用对象的值
    const T & at(const size_t &pos) const {
        if (pos < 0 || pos >= vectorSize) throw index_out_of_bound();
        return *storage[pos];
    }
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     * !!! Pay attentions
     *   In STL this operator does not check the boundary.
     */
    T & operator[](const size_t &pos) {
        if (pos < 0 || pos >= vectorSize) throw index_out_of_bound();
        return *storage[pos];
    }
    ///used by const vector
    const T & operator[](const size_t &pos) const {
        if (pos < 0 || pos >= vectorSize) throw index_out_of_bound();
        return *storage[pos];
    }
    /**
     * access the first element.
     * throw container_is_empty if size == 0
     */
    const T & front() const {
        if (vectorSize == 0) throw container_is_empty();
        return *storage[0];
    }
    /**
     * access the last element.
     * throw container_is_empty if size == 0
     */
    const T & back() const {
        if (vectorSize == 0) throw container_is_empty();
        return *storage[vectorSize - 1];
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        iterator tmp(this, storage);
        return tmp;
    }
    ///used by const vector
    //这里是sjtu::vector的常量成员函数，不能修改其数据成员
    //因此const_iterator中必须把数据成员都设为常量，这样就不可能修改了
    const_iterator cbegin() const {
        const_iterator tmp(this, storage);
        return tmp;
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        iterator tmp(this, storage + vectorSize);
        return tmp;
    }
    ///used by const vector
    const_iterator cend() const {
        const_iterator tmp(this, storage + vectorSize);
        return tmp;
    }
    /**
     * checks whether the container is empty
     */
    bool empty() const {
        if (vectorSize == 0) return true;
        else return false;
    }
    /**
     * returns the number of elements
     */
    size_t size() const {
        return vectorSize;
    }
    /**
     * clears the contents
     */
    void clear() {
        for (size_t i = 0; i < vectorSize; ++i) delete storage[i];
        vectorSize = 0;
    }
    /**
     * inserts value before pos(valid)
     * returns an iterator pointing to the inserted value.
     */
    iterator insert(iterator pos, const T &value) {
        long long ind = pos - begin();
        if (full()) expand();
        storage[vectorSize] = new T(*storage[vectorSize - 1]);
        for (long long i = vectorSize - 2; i >= ind; --i)
        {
            *storage[i + 1] = *storage[i];
        }
        *storage[ind] = value;
        ++vectorSize;
        iterator itr(this, storage + ind);
        return itr;
    }
    /**
     * inserts value at index ind.
     * after inserting, this->at(ind) == value
     * returns an iterator pointing to the inserted value.
     * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
     */
    iterator insert(const size_t &ind, const T &value) {
        if (ind > vectorSize) throw index_out_of_bound();
        if (full()) expand();
        if (vectorSize == ind) {
            storage[vectorSize] = new T(value);
            ++vectorSize;
        } else {
            storage[vectorSize] = new T(*storage[vectorSize - 1]);
            //防止size_t到long long自动类型转唤出错
            long long newind = ind;
            for (long long i = vectorSize - 2; i >= newind; --i)
            {
                *storage[i + 1] = *storage[i];
            }
            *storage[ind] = value;
            ++vectorSize;
        }
        iterator itr(this, storage + ind);
        return itr;
    }
    /**
     * removes the element at pos.
     * return an iterator pointing to the following element.
     * If the iterator pos refers the last element, the end() iterator is returned.
     */
    iterator erase(iterator pos) {
        if (pos == end() - 1)
        {
            delete storage[vectorSize - 1];
            --vectorSize;
            return end();
        }
        size_t aryPos = pos - begin();
        for (size_t i = aryPos; i < vectorSize - 1; ++i)
        {
            *storage[i] = *storage[i + 1];
        }
        delete storage[vectorSize - 1];
        --vectorSize;
        return pos;
    }
    /**
     * removes the element with index ind.
     * return an iterator pointing to the following element.
     * throw index_out_of_bound if ind >= size
     */
    iterator erase(const size_t &ind) {
        if (ind >= vectorSize) throw index_out_of_bound();
        if (ind == vectorSize - 1)
        {
            delete storage[vectorSize - 1];
            --vectorSize;
            return end();
        }
        for (size_t i = ind; i < vectorSize - 1; ++i)
        {
            *storage[i] = *storage[i + 1];
        }
        delete storage[vectorSize - 1];
        --vectorSize;
        iterator itr(this, storage + ind);
        return itr;
    }
    /**
     * adds an element to the end.
     */
    void push_back(const T &value) {
        if (full()) expand();
        storage[vectorSize] = new T(value);
        ++vectorSize;
    }
    /**
     * remove the last element from the end.
     * throw container_is_empty if size() == 0
     */
    void pop_back() {
        if (size() == 0) throw container_is_empty();
        delete storage[vectorSize - 1];
        --vectorSize;
    }
private:
    ///judge whether the currentsize is about to be full
    inline bool full() {
        return (vectorSize + 2 >= capacity);
    }
    ///doublespace when the current size is close to the maximal capacity
    void expand() {
        capacity *= 2;
        T **newStorage = new T* [capacity];
        for (size_t i = 0; i < vectorSize; ++i)
        {
            newStorage[i] = new T(*storage[i]);
            delete storage[i];
        }
        if (storage) delete [] storage;
        storage = newStorage;
    }
// the inner class is naturally the friend class of the outer class
// friend class iterator;
// friend class const_iterator;
};
///the end of namespace sjtu
}

#endif

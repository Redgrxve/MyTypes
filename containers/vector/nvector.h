#ifndef NVECTOR_H
#define NVECTOR_H

#include <cstddef>
#include <stdexcept>
#include "debuglog.h"

template<typename T>
class NVector
{
public:
    //CONSTRUCTORS
    NVector() = default;

    explicit NVector(size_t size)
        : m_size{ size }, m_capacity{ m_size * 2 }, m_vec{ new T[m_capacity] }
    {
        DEBUG_LOG("NVector(size_t size)");
    }

    NVector(const NVector& other)
        : m_size{ other.m_size }, m_capacity{ other.m_capacity }
    {
        DEBUG_LOG("NVector(const NVector& other))");

        m_vec = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i)
            m_vec[i] = other.m_vec[i];
    }

    NVector(NVector&& other) noexcept
        : m_size{ other.m_size }, m_capacity{ other.m_capacity }, m_vec{ other.m_vec }
    {
        DEBUG_LOG("NVector(NVector&& other)");

        other.m_size = 0;
        other.m_capacity = 0;
        other.m_vec = nullptr;
    }

    NVector(const std::initializer_list<T>& initList)
        : m_size{ initList.size() }, m_capacity{ m_size * 2 }
    {
        DEBUG_LOG("NVector(const std::initializer_list<T>& initList)");

        if (initList.size() == 0) {
            NVector();
            return;
        }

        m_vec = new T[m_capacity];
        size_t i{};
        for (const auto& elem : initList)
            m_vec[i++] = elem;
    }

    ~NVector() { delete[] m_vec; }

    //OPERATORS
    NVector& operator=(const NVector& other)
    {
        DEBUG_LOG("operator=(const NVector& other)");

        if (&other == this) return *this;

        if (m_capacity >= other.m_size) {
            for (size_t i = 0; i < other.m_size; ++i)
                m_vec[i] = other.m_vec[i];
            m_size = other.m_size;
            return *this;
        }

        T* newVec = new T[other.m_capacity];
        for (size_t i = 0; i < other.m_size; ++i)
            newVec[i] = other.m_vec[i];

        delete[] m_vec;

        m_vec = newVec;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        return *this;
    }

    NVector& operator=(NVector&& other) noexcept
    {
        DEBUG_LOG("operator=(NVector&& other)");

        if (&other == this) return *this;

        delete[] m_vec;

        m_vec = other.m_vec;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_vec = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

    T& operator[](size_t index)
    {
        if (index >= m_size)
            throw std::out_of_range("Index out of bounds");
        return m_vec[index];
    }

    const T& operator[](size_t index) const
    {
        if (index >= m_size)
            throw std::out_of_range("Index out of bounds");
        return m_vec[index];
    }

    friend std::ostream& operator<<(std::ostream& os, const NVector& vec)
    {
        for (const auto& elem : vec)
            os << elem << "\n";
        return os;
    }

    //METHODS
    size_t size() const { return m_size; }

    T* begin() { return m_vec; }
    T* end() { return m_vec + m_size; }

    const T* begin() const { return m_vec; }
    const T* end() const { return m_vec + m_size; }

    template<typename U>
    void pushBack(U&& item)
    {
        if (m_size >= m_capacity)
            resize();

        m_vec[m_size] = std::forward<U>(item);
        ++m_size;

#ifdef DEBUG
        bool isLVal = std::is_lvalue_reference_v<U>;
        if (isLVal)
            std::cout << "Pushed lvalue\n";
        else
            std::cout << "Pushed rvalue\n";
#endif
    }

private:
    void resize()
    {
        m_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
        T* newVec = new T[m_capacity];
        for (int i = 0; i < m_size; ++i)
            newVec[i] = std::move_if_noexcept(m_vec[i]);

        delete[] m_vec;
        m_vec = newVec;
    }

    size_t m_size{};
    size_t m_capacity{};
    T* m_vec{};
};

#endif // NVECTOR_H

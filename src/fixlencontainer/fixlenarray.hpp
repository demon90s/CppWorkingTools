#pragma once

#include <string>
#include <cassert>
#include <functional>
#include <algorithm>
#include <vector>

// 原生数组 wrapper
// 提供了一些业务层面的常用操作
// 目的是避免使用裸操作数组，make life easier

template<typename T, int N>
class FixLenArray
{
private:
    T m_data[N];
public:
    FixLenArray() :
        m_data {}
    {
        static_assert(N > 0, "N MUST > 0");
    }

    FixLenArray(const std::initializer_list<T> &il) :
        m_data {}
    {
        int count = 0;
        for (const T& item : il)
        {
            if (count >= this->Size()) break;
            m_data[count++] = item;
        }
    }
    
    void Reset()
    {
        for (int i = 0; i < N; i++)
        {
            m_data[i] = T {};
        }
    }

    constexpr int Size() const
    {
        return N;
    }

    bool InvalidIndex(int index) const
    {
        return index < 0 || index >= this->Size();
    }

    T &operator[](int index)
    {
        return const_cast<T&>(
            static_cast<const FixLenArray*>(this)->operator[](index)
            );
    }

    const T &operator[](int index) const
    {
        if (InvalidIndex(index))
        {
            throw std::out_of_range("out of range");
        }

        return m_data[index];
    }

public:
    void Travel(std::function<void(T&)> f)
    {
        for (auto &item : m_data)
        {
            f(item);
        }
    }

    void ConstTravel(std::function<void(const T&)> f)
    {
        for (const auto &item : m_data)
        {
            f(item);
        }
    }

    void Copy(T (&raw)[N]) const
    {
        for (int i = 0; i < N; i++)
        {
            raw[i] = m_data[i];
        }
    }

    const T &MaxElement(std::function<bool(const T&, const T&)> cmp) const
    {
        return *std::max_element(std::begin(m_data), std::end(m_data), cmp);
    }

    const T &MaxElement() const
    {
        return *std::max_element(std::begin(m_data), std::end(m_data));
    }

    const T &MinElement(std::function<bool(const T&, const T&)> cmp) const
    {
        return *std::min_element(std::begin(m_data), std::end(m_data), cmp);
    }

    const T &MinElement() const
    {
        return *std::min_element(std::begin(m_data), std::end(m_data));
    }

    // 拿第一个满足谓词的元素
    const T *GetElement(std::function<bool(const T&)> cmp) const
    {
        for (int i = 0; i < N; i++)
        {
            if (cmp(m_data[i]))
            {
                return &m_data[i];
            }
        }
        return nullptr;
    }

    const T *GetElement(const T &item) const
    {
        auto cmp = [&item](const T &cmp_item)
        {
            return item == cmp_item;
        };
        return this->GetElement(cmp);
    }

    // 排序，但不排序本身，而是把排序结果放入返回值，注意，若数组生命周期结束，该返回值即不可用
    std::vector<T*> Sort(std::function<bool(const T&, const T&)> cmp)
    {
        std::vector<T*> sort_vec;
        for (int i = 0; i < N; i++)
        {
            sort_vec.push_back(&m_data[i]);
        }
        std::sort(sort_vec.begin(), sort_vec.end(), [&cmp](const T *lhs, const T *rhs) { return cmp(*lhs, *rhs); });
        return sort_vec;
    }

    std::vector<T*> Sort()
    {
        auto cmp = [](const T &lhs, const T &rhs)
        {
            return lhs < rhs;
        };

        return this->Sort(cmp);
    }

public:
    // iterators
    T *begin()
    {
        return const_cast<T*>(static_cast<const FixLenArray*>(this)->begin());
    }

    const T *begin() const
    {
        return m_data;
    }

    const T *cbegin() const
    {
        return this->begin();
    }

    T *end()
    {
        return const_cast<T*>(static_cast<const FixLenArray*>(this)->end());
    }

    const T *end() const
    {
        return m_data + this->Size();
    }

    const T *cend() const
    {
        return this->end();
    }
};

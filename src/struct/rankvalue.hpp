#pragma once

class RankValue
{
public:
    RankValue(int v = 0) : m_value(v) {}
    
    explicit operator int() const { return m_value; }

    int GetValue() const
    {
        return m_value;
    }

    void SetValue(int v)
    {
        m_value = v;
    }

    bool InvalidValue() const
    {
        return m_value <= 0;
    }

    // 比rhs排名靠前
    bool IsAheadOf(const RankValue &rhs) const
    {
        return m_value < rhs.m_value;
    }

    // 比rhs排名靠后
    bool IsBehindOf(const RankValue &rhs) const
    {
        return m_value > rhs.m_value;
    }

    // 跟rhs排名一样
    bool IsEqualOf(const RankValue &rhs) const
    {
        return m_value == rhs.m_value;
    }

private:
    int m_value {};
};

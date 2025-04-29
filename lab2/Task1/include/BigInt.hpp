#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>
#include <exception>

class BigInt
{
public:
    BigInt()
    {
        isNegative = false;
        digits.push_back(0);
    }
    BigInt(long long value)
    {
        if (value == 0)
        {
            digits.push_back(0);
            isNegative = false;
            return;
        }

        if (value < 0)
        {
            isNegative = true;
            value = -value;
        }
        else
        {
            isNegative = false;
        }
        unsigned long long len = (unsigned long long)log10((long long)value) + 1;
        digits.resize(len);

        unsigned long long mod = 0;
        while (value != 0)
        {
            mod = value % 10;
            digits[--len] = mod;
            value /= 10;
        }
    }
    BigInt(const std::string &str)
    {
        int i = 0;
        isNegative = false;
        if (str[0] == '-')
        {
            isNegative = true;
            ++i;
        }
        bool hadDigits = false;
        for (; i < (int)str.length(); i++)
        {
            if (str[i] != '0')
            {
                hadDigits = true;
            }
            if (hadDigits)
            {
                if (!isdigit(str[i]))
                {
                    throw std::exception();
                }
                digits.push_back((unsigned long long)(str[i] - '0'));
            }
        }
    }
    BigInt(const BigInt &other)
    {
        isNegative = other.isNegative;
        digits.assign(other.digits.begin(), other.digits.end());
    }

    BigInt(BigInt &&other) noexcept
    {
        isNegative = other.isNegative;
        digits.assign(other.digits.begin(), other.digits.end());

        other.digits.clear();
    }

    ~BigInt() = default;

    BigInt &operator=(const BigInt &other)
    {
        isNegative = other.isNegative;
        digits.assign(other.digits.begin(), other.digits.end());
        return *this;
    }
    BigInt &operator=(BigInt &&other) noexcept
    {
        isNegative = other.isNegative;
        digits.assign(other.digits.begin(), other.digits.end());

        other.digits.clear();
        return *this;
    }

    BigInt operator+(const BigInt &other) const
    {
        if (this->isNegative != other.isNegative)
        {
            if (this->isNegative)
            {
                BigInt first = (*this);
                first.isNegative = false;
                return other - first;
            }
            else
            {
                BigInt second = other;
                second.isNegative = false;
                return (*this) - second;
            }
        }
        else
        {
            BigInt first = (*this);
            BigInt second = other;

            int div = 0, res = 0;
            std::string result;

            if (first.digits.size() < second.digits.size())
            {
                swap(first, second);
            }

            for (int i = (int)(first.digits.size() - 1), j = (int)(second.digits.size() - 1); i >= 0; i--, j--)
            {
                if (j < 0)
                {
                    res = first.digits[i] + div;
                    result += (char)('0' + res % 10);
                    div = res / 10;
                    continue;
                }
                res = first.digits[i] + second.digits[j] + div;
                result += (char)('0' + res % 10);
                div = res / 10;
            }
            if (div != 0)
            {
                result += (char)('0' + div % 10);
            }
            if (first.isNegative)
            {
                result += '-';
            }

            std::reverse(result.begin(), result.end());
            return BigInt(result);
        }
    }
    BigInt operator-(const BigInt &other) const
    {
        if (this->isNegative != other.isNegative)
        {
            if (this->isNegative)
            {
                BigInt first = (*this);
                BigInt second = other;

                second.isNegative = true;

                return first + second;
            }
            else
            {
                BigInt second = other;
                second.isNegative = false;
                return (*this) + second;
            }
        }
        else if (this->isNegative)
        {
            BigInt first = other, second = (*this);
            first.isNegative = false;
            second.isNegative = false;
            return first - second;
        }
        else
        {
            BigInt first = (*this), second = other;
            if (first == second)
            {
                return BigInt(0);
            }
            else if (first < second)
            {
                swap(first, second);
                std::string result;
                for (int i = (int)(first.digits.size() - 1), j = (int)(second.digits.size() - 1); i >= 0; i--, j--)
                {
                    if (j < 0)
                    {
                        result += (char)('0' + first.digits[i]);
                        continue;
                    }
                    int first_int = first.digits[i], second_int = second.digits[j];
                    if (first_int < second_int)
                    {
                        take_dozen_for(first.digits, (size_t)i);
                        first_int += 10;
                    }
                    result += (char)('0' + (first_int - second_int));
                }
                result += '-';
                std::reverse(result.begin(), result.end());
                return BigInt(result);
            }
            else
            {
                std::string result;
                for (int i = (int)(first.digits.size() - 1), j = (int)(second.digits.size() - 1); i >= 0; i--, j--)
                {
                    if (j < 0)
                    {
                        result += (char)('0' + first.digits[i]);
                        continue;
                    }
                    int first_int = first.digits[i], second_int = second.digits[j];
                    if (first_int < second_int)
                    {
                        take_dozen_for(first.digits, (size_t)i);
                        first_int += 10;
                    }
                    result += (char)('0' + (first_int - second_int));
                }
                std::reverse(result.begin(), result.end());
                return BigInt(result);
            }
        }
    }
    BigInt operator*(const BigInt &other) const
    {

        bool negative = true;

        if ((this->isNegative && other.isNegative) || (!this->isNegative && !other.isNegative))
        {
            negative = false;
        }

        BigInt result = 0, first = *this, second = other;

        if (first < second)
        {
            swap(first, second);
        }
        std::vector<BigInt> numbers_to_sum;
        for (int i = second.digits.size() - 1; i >= 0; i--)
        {
            std::string num;

            for (size_t j = 0; j < second.digits.size() - i - 1; j++)
            {
                num += '0';
            }

            int div = 0, res = 0;
            for (int j = first.digits.size() - 1; j >= 0; j--)
            {
                res = second.digits[i] * first.digits[j] + div;

                num += ('0' + (res % 10));

                div = res / 10;
            }
            if (div != 0)
            {
                num += ('0' + (div % 10));
            }

            std::reverse(num.begin(), num.end());
            numbers_to_sum.push_back(BigInt(num));
        }

        for (size_t i = 0; i < numbers_to_sum.size(); i++)
        {
            result = result + numbers_to_sum[i];
        }
        if (negative)
        {
            result.isNegative = true;
        }
        return result;
    }

    BigInt operator/(const BigInt &other) const
    {

        if (other.digits[0] == 0)
        {
            throw std::exception();
        }

        bool negative = true;

        if ((this->isNegative && other.isNegative) || (!this->isNegative && !other.isNegative))
        {
            negative = false;
        }
        BigInt first = *this, second = other;
        first.isNegative = false, second.isNegative = false;
        if (first < second)
        {
            return BigInt(0);
        }
        else if (first == second)
        {
            if (negative)
            {
                return BigInt("-1");
            }
            return BigInt(1);
        }
        else
        {

            std::string part, result;
            size_t index = 0;
            while (index < first.digits.size())
            {
                part += (char)('0' + first.digits[index]);
                BigInt bi_part(part);
                int cnt = 0;
                if (bi_part >= second)
                {
                    while (second <= bi_part)
                    {
                        bi_part -= second;
                        ++cnt;
                    }
                    result += (char)('0' + cnt);
                    part.clear();
                    if (bi_part.digits[0] != 0)
                    {
                        for (size_t i = 0; i < bi_part.digits.size(); i++)
                        {
                            part += (char)('0' + bi_part.digits[i]);
                        }
                    }
                }
                else if (part == "0")
                {
                    result += '0';
                    part.clear();
                }
                else{
                    result += '0';
                }
                ++index;
            }
            if (part.size() != 0)
            {
                BigInt bi_part(part);
                int cnt = 0;
                if (bi_part >= second)
                {
                    while (second <= bi_part)
                    {
                        bi_part -= second;
                        ++cnt;
                    }
                    result += (char)('0' + cnt);
                    part.clear();
                    if (bi_part.digits[0] != 0)
                    {
                        for (size_t i = 0; i < bi_part.digits.size(); i++)
                        {
                            part += (char)('0' + bi_part.digits[i]);
                        }
                    }
                }
                else if (part == "0")
                {
                    result += '0';
                    part.clear();
                }
            }
            if (negative)
            {
                result = "-" + result;
            }
            return BigInt(result);
        }
    }

    BigInt operator<<(const BigInt &other)
    {
        if (this->digits[0] == 0)
        {
            return BigInt(1);
        }
        BigInt first = *this;
        for (BigInt i = 0; i < other; i++)
        {
            first *= 2;
        }
        return first;
    }

    BigInt operator>>(const BigInt &other)
    {
        BigInt first = *this;
        for (BigInt i = 0; i < other; i++)
        {
            first /= 2;
        }
        return first;
    }

    BigInt operator%(const BigInt &other)
    {
        BigInt first = *this / other;
        BigInt second = first * other;

        return (*this) - second;
    }

    BigInt &operator>>=(const BigInt &other)
    {
        *this = *this >> other;
        return *this;
    }

    BigInt &operator<<=(const BigInt &other)
    {
        (*this) = (*this) << other;
        return (*this);
    }

    BigInt &operator%=(const BigInt &other)
    {
        *this = *this % other;
        return *this;
    }

    BigInt &operator+=(const BigInt &other)
    {
        (*this) = (*this) + other;
        return (*this);
    }
    BigInt &operator-=(const BigInt &other)
    {
        (*this) = (*this) - other;
        return (*this);
    }
    BigInt &operator*=(const BigInt &other)
    {
        (*this) = (*this) * other;
        return (*this);
    }

    BigInt &operator/=(const BigInt &other)
    {
        (*this) = (*this) / other;
        return (*this);
    }

    BigInt &operator++()
    {
        (*this) = (*this) + BigInt(1);
        return (*this);
    }

    BigInt operator++(int)
    {
        BigInt result = *this;
        (*this) = (*this) + BigInt(1);
        return result;
    }
    BigInt &operator--()
    {
        (*this) = (*this) - BigInt(1);
        return (*this);
    }

    BigInt operator--(int)
    {
        BigInt result = *this;
        (*this) = (*this) - BigInt(1);
        return result;
    }

    bool operator==(const BigInt &other) const
    {
        if ((other.digits.size() != this->digits.size()) || (other.isNegative != this->isNegative))
        {
            return false;
        }
        for (size_t i = 0; i < other.digits.size(); i++)
        {
            if (this->digits[i] != other.digits[i])
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const BigInt &other) const
    {
        return !(*this == other);
    }

    bool operator<(const BigInt &other) const
    {
        if (this->isNegative < other.isNegative)
        {
            return false;
        }
        else if (other.isNegative < this->isNegative)
        {
            return true;
        }
        else
        {
            if (this->isNegative)
            {
                if (this->digits.size() > other.digits.size())
                {
                    return true;
                }
                else if (this->digits.size() < other.digits.size())
                {
                    return false;
                }
                else
                {
                    for (size_t i = 0; i < this->digits.size(); i++)
                    {
                        if (this->digits[i] > other.digits[i])
                        {
                            return true;
                        }
                        else if (this->digits[i] < other.digits[i])
                        {
                            return false;
                        }
                    }
                    return false;
                }
            }
            else
            {
                if (this->digits.size() > other.digits.size())
                {
                    return false;
                }
                else if (this->digits.size() < other.digits.size())
                {
                    return true;
                }
                else
                {
                    for (size_t i = 0; i < this->digits.size(); i++)
                    {
                        if (this->digits[i] > other.digits[i])
                        {
                            return false;
                        }
                        else if (this->digits[i] < other.digits[i])
                        {
                            return true;
                        }
                    }
                    return false;
                }
            }
        }
    }

    bool operator>=(const BigInt &other) const
    {
        return !((*this) < other);
    }

    bool operator>(const BigInt &other) const
    {
        return !((*this) <= other);
    }
    bool operator<=(const BigInt &other) const
    {
        return ((*this) < other) || ((*this) == other);
    }

    static void swap(BigInt &first, BigInt &second)
    {
        std::swap(first.isNegative, second.isNegative);
        std::swap(first.digits, second.digits);
    }

    long long length()
    {
        return this->digits.size();
    }

    friend std::istream &operator>>(std::istream &is, BigInt &num)
    {
        std::string number;
        is >> number;
        num = BigInt(number);
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &num)
    {
        if (num.isNegative)
        {
            os << "-";
        }
        for (size_t i = 0; i < num.digits.size(); i++)
        {
            os << num.digits[i];
        }
        return os;
    }

    friend BigInt mod_exp(const BigInt &base, const BigInt &exp, const BigInt &mod);

private:
    static void take_dozen_for(std::vector<unsigned long long> &_digits, const size_t &index)
    {
        if (index == 0)
        {
            return;
        }
        int take = index - 1;
        for (int i = index - 1; i >= 0 && _digits[i] == 0; i--)
        {
            _digits[i] = 9;
            take--;
        }
        --_digits[take];
    }

    std::vector<unsigned long long> digits;
    bool isNegative;
};

BigInt mod_exp(const BigInt &base, const BigInt &exp, const BigInt &mod = BigInt())
{
    if (exp.digits.front() == 0)
    {
        return BigInt(1);
    }

    BigInt res = 1, n = exp, x = base; // res = x ^ n;

    bool negative = false;
    if (x.isNegative && (n % BigInt(2) == BigInt(1)))
    {
        negative = true;
    }

    while (n != BigInt())
    {
        if (n.digits.back() % 2 == 1)
        {
            res = (mod.digits.front() == 0) ? res * x : (res * x) % mod;
        }
        x *= x;
        n >>= 1;
    }
    return negative ? res * BigInt("-1") : res;
}

/* BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod) const;
BigInt fft_multiply(const BigInt& a) const;
BigInt karatsuba_multiply(const BigInt& a) const;
BigInt newton_divide(const BigInt& a) const; */

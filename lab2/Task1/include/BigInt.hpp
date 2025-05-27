#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <climits>
#include <algorithm>
#include <exception>
#include <cmath>

#define NUMBER_LENGTH(x) (std::to_string(x).length())

const long double PI = std::acos(-1);

class BigInt
{
public:
    BigInt()
    {
        isNegative = false;
    }
    BigInt(long long value, unsigned long long _module = 1000000000)
    {
        module = _module;
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
        unsigned long long len = (NUMBER_LENGTH(value) / NUMBER_LENGTH(module)) + 1;
        digits.resize(len);

        unsigned long long mod = 0;
        while (value != 0)
        {
            mod = value % module;
            digits[--len] = mod;
            value /= module;
        }
    }
    BigInt(const std::string &str, unsigned long long _module = 1000000000)
    {
        module = _module;
        int i = 0;
        isNegative = false;
        if (str[0] == '-')
        {
            isNegative = true;
            ++i;
        }
        bool ended = true;
        unsigned long long buffer = 0, buflen = 1;
        for (int index = (int)str.length() - 1; index >= i; index--)
        {
            ended = false;
            if (!isdigit(str[index]))
            {
                throw std::exception();
            }

            buffer = buffer + ((str[index] - '0') * buflen);
            buflen *= 10;
            if (buflen == module)
            {
                digits.push_back(buffer % module);
                buffer = buffer / module;
                buflen = (buffer == 0) ? 1 : 10;
                ended = true;
            }
        }
        if (!ended)
        {
            digits.push_back(buffer);
        }
        std::reverse(digits.begin(), digits.end());
    }
    BigInt(const BigInt &other)
    {
        this->module = other.module;
        isNegative = other.isNegative;
        digits.assign(other.digits.begin(), other.digits.end());
    }


    BigInt(BigInt &&other) noexcept
    {
        this->module = other.module;
        isNegative = other.isNegative;
        digits.assign(other.digits.begin(), other.digits.end());

        other.digits.clear();
    }

    virtual ~BigInt() = default;

    BigInt &operator=(const BigInt &other)
    {
        this->module = other.module;
        isNegative = other.isNegative;
        digits.assign(other.digits.begin(), other.digits.end());
        return *this;
    }
    BigInt &operator=(BigInt &&other) noexcept
    {
        this->module = other.module;
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

            unsigned long long div = 0, sum = 0;
            BigInt result;
            result.digits.clear();

            if (first.digits.size() < second.digits.size())
            {
                swap(first, second);
            }

            for (int i = (int)(first.digits.size() - 1), j = (int)(second.digits.size() - 1); i >= 0; i--, j--)
            {
                if (j < 0)
                {
                    sum = first.digits[i] + div;
                    result.digits.push_back(sum % module);
                    div = sum / module;
                    continue;
                }
                sum = first.digits[i] + second.digits[j] + div;
                result.digits.push_back(sum % module);
                div = sum / module;
            }
            if (div != 0)
            {
                result.digits.push_back(div % module);
            }
            if (first.isNegative)
            {
                result.isNegative = true;
            }
            take_zeroes_from_end_ans_reverse(result.digits);
            return result;
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
            else
            {
                BigInt result;
                if (first < second)
                {
                    swap(first, second);
                    result.isNegative = true;
                }

                result.digits.clear();

                for (int i = (int)(first.digits.size() - 1), j = (int)(second.digits.size() - 1); i >= 0; i--, j--)
                {
                    if (j < 0)
                    {
                        result.digits.push_back(first.digits[i]);
                        continue;
                    }
                    int first_int = first.digits[i], second_int = second.digits[j];
                    if (first_int < second_int)
                    {
                        take_dozen_for(first.digits, (size_t)i);
                        first_int += module;
                    }
                    result.digits.push_back(first_int - second_int);
                }
                take_zeroes_from_end_ans_reverse(result.digits);
                return result;
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
            BigInt num;
            num.digits.clear();
            for (size_t j = 0; j < second.digits.size() - i - 1; j++)
            {
                num.digits.push_back(0);
            }

            unsigned long long div = 0, res = 0;
            for (int j = first.digits.size() - 1; j >= 0; j--)
            {
                res = second.digits[i] * first.digits[j] + div;

                num.digits.push_back(res % module);

                div = res / module;
            }
            if (div != 0)
            {
                num.digits.push_back(div % module);
            }

            std::reverse(num.digits.begin(), num.digits.end());
            numbers_to_sum.push_back(num);
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
                return BigInt(-1);
            }
            return BigInt(1);
        }
        else
        {

            std::string bigIntNumber_str = to_string(first);

            bool action = false;
            std::string part, result;
            size_t index = 0;
            while (index < bigIntNumber_str.size())
            {
                part += bigIntNumber_str[index];
                BigInt bi_part(part);
                int cnt = 0;
                if (bi_part >= second)
                {
                    action = true;
                    while (second <= bi_part)
                    {
                        bi_part -= second;
                        ++cnt;
                    }
                    result += (char)('0' + cnt);
                    part.clear();
                    if (bi_part.digits[0] != 0)
                    {
                        part += to_string(bi_part);
                    }
                }
                else if (part == "0")
                {
                    result += '0';
                    part.clear();
                }
                else
                {
                    if (action)
                    {
                        result += '0';
                    }
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
                        part += to_string(bi_part);
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

    BigInt operator*(const /* long  */ double &other) const
    {
        if (other < __LDBL_EPSILON__)
        {
            throw std::exception();
        }
        BigInt a = *this, result;
        bool flag = false;
        if (int(a.digits[0] * other) == 0 && a.digits.size() > 1)
        {
            flag = true;
            a.digits[1] = a.digits[0] * module + a.digits[1];
        }
        for (size_t i = (flag ? 1 : 0); i < a.digits.size(); i++)
        {
            if (a.digits[i] != 0)
            {
                result.digits.push_back(a.digits[i] * other);
            }
            else
            {
                result.digits.push_back((unsigned long long)((module * 10) * other) % module);
            }
        }
        return BigInt(to_string(result));
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

    friend std::istream &operator>>(std::istream &is, BigInt &num)
    {
        std::string number;
        is >> number;
        num = BigInt(number);
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &num)
    {
        if (num.digits.front() == 0)
        {
            os << '0';
            return os;
        }
        if (num.isNegative)
        {
            os << "-";
        }
        os << num.digits[0];
        for (size_t i = 1; i < num.digits.size(); i++)
        {
            if (num.digits[i] == 0)
            {
                for (unsigned long long j = 1; j < num.module; j *= 10)
                {
                    os << '0';
                }
                continue;
            }
            else
            {
                int j = NUMBER_LENGTH(num.digits[i]);
                int mod_length = NUMBER_LENGTH(num.module) - 1;
                while (j++ < mod_length)
                {
                    os << '0';
                }
                os << num.digits[i];
            }
        }
        return os;
    }

    BigInt mod_exp(const BigInt &exp, const BigInt &mod = BigInt(0)) const
    {
        return BigInt_mod_exp(*this, exp, mod);
    }

    static BigInt BigInt_mod_exp(const BigInt &base, const BigInt &exp, const BigInt &mod = BigInt(0))
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

        while (n != BigInt(0))
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

    BigInt fft_multiply(const BigInt &a) const
    {
        BigInt _this(to_string(*this), 1000), other(to_string(a), 1000);
        
        BigInt result;
        result.module = 1000;

        std::vector<std::complex<long double>> fa(_this.digits.begin(), _this.digits.end()), fb(other.digits.begin(), other.digits.end());
        size_t n = 1;

        while (n < std::max(_this.digits.size(), other.digits.size()))
        {
            n <<= 1;
        }
        n <<= 1;

        fa.insert(fa.begin(), (int)(n - fa.size()), std::complex<long double>(0, 0));
        fb.insert(fb.begin(), (int)(n - fb.size()), std::complex<long double>(0, 0));

        const long double ang = (2.0 * PI) / (long double)(n);
        
        const std::complex<long double> w(std::cos(ang), std::sin(ang));
        std::complex<long double> wn(1, 0);

        fft(fa, w);
        fft(fb, w);

        for (size_t i = 0; i < n; ++i)
        {
            fa[i] *= fb[i];
        }

        fft(fa, wn / w);

        for (size_t i = 0; i < n; i++)
        {
            result.digits.push_back((unsigned long long)std::round(fa[i].real() / (long double)n));
        }

        while (result.digits.size() > 1 && result.digits.front() == 0)
        {
            result.digits.erase(result.digits.begin());
        }
        

        
        std::vector<unsigned long long> r_v;
        module_digits(result.digits, r_v, result.module);
        result.digits = r_v;
        result.isNegative = this->isNegative != a.isNegative;
        std::string ans = to_string(result);

        return BigInt(ans);
    }

    BigInt karatsuba_multiply(const BigInt &element) const
    {
        BigInt a = *this, b = element, result;
        result.isNegative = (a.isNegative != b.isNegative);

        if (a.digits.size() < 2 || b.digits.size() < 2)
        {
            return a * b;
        }
        else
        {
            const size_t m = std::min(a.digits.size(), b.digits.size()) / 2;

            BigInt a_part1, a_part2;
            if (a.digits.size() > m)
            {
                a_part1.digits.assign(a.digits.begin(), a.digits.begin() + (a.digits.size() - m));
                a_part2.digits.assign(a.digits.begin() + (a.digits.size() - m), a.digits.end());
            }
            else
            {
                a_part2 = a;
            }

            BigInt b_part1, b_part2;
            if (b.digits.size() > m)
            {
                b_part1.digits.assign(b.digits.begin(), b.digits.begin() + (b.digits.size() - m));
                b_part2.digits.assign(b.digits.begin() + (b.digits.size() - m), b.digits.end());
            }
            else
            {
                b_part2 = b;
            }

            BigInt prod_of_second_parts = a_part2.karatsuba_multiply(b_part2);
            BigInt sumOfAParts = a_part1 + a_part2;
            BigInt sumOfBParts = b_part1 + b_part2;
            BigInt prod_of_sums_of_parts = sumOfAParts.karatsuba_multiply(sumOfBParts);
            BigInt prod_of_first_parts = a_part1.karatsuba_multiply(b_part1);

            BigInt sum_of_middle_terms = prod_of_sums_of_parts - prod_of_first_parts - prod_of_second_parts;

            for (size_t i = 0; i < 2 * m; i++)
            {
                prod_of_first_parts.digits.push_back(0);
            }

            for (size_t i = 0; i < m; i++)
            {
                sum_of_middle_terms.digits.push_back(0);
            }

            result = prod_of_first_parts + sum_of_middle_terms + prod_of_second_parts;
        }

        return result;
    }

    BigInt newton_divide(const BigInt &a) const
    {
        if (a.digits.front() == 0)
        {
            throw std::exception();
        }
        return BigInt(newton_divide_mas(this->digits, a.digits).first);
    }

    static std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>> newton_divide_mas(const std::vector<unsigned long long> &f, const std::vector<unsigned long long> &g)
    {
        if (f.size() < g.size())
        {
            return {std::vector<unsigned long long>(1, 0), std::vector<unsigned long long>(f)};
        }
        int m = f.size() - g.size();
        std::vector<unsigned long long> h = newton_inverse(rev(g, g.size()), m + 1);
        std::vector<unsigned long long> q = (((BigInt(rev(f, f.size())) * BigInt(h)))).digits;
        if(q.size() >= (size_t)(m + 1)){
            q = {q.begin() + q.size() - m - 1, q.end()};
        }
        return {rev(q, m), (BigInt(f) - (BigInt(rev(q, m)) * BigInt(g))).digits};
    }

    static std::vector<unsigned long long> rev(const std::vector<unsigned long long> &vec, size_t n)
    {
        std::vector<unsigned long long> result = vec;

        size_t elements_to_reverse = std::min(n, result.size());

        std::reverse(result.begin(), result.begin() + elements_to_reverse);

        return result;
    }

    static std::vector<unsigned long long> newton_inverse(std::vector<unsigned long long> f, int l)
    {
        unsigned int k = std::ceil(std::log2(l))/*  + 1 */;
        std::vector<unsigned long long> g{mod_inv(f[0])};
        BigInt gi(g), fi(f);
        for (size_t i = 1; i <= k; i++)
        {
            gi = (BigInt(2) * gi - fi * gi.mod_exp(BigInt(2)));
            if(gi.digits.size() > (size_t)(1 << i)){
                gi.digits = {gi.digits.begin() + gi.digits.size() - (1 << i), gi.digits.end()};
            }
            
        }

        return gi.digits;
    }

    static unsigned long long mod_inv(unsigned long long a, unsigned long long module = 1000000000)
    {
        if (a == 0)
        {
            throw std::exception();
        }
        else if (a % 2 == 0 || a % 5 == 0)
        {
            a++;
        }

        // Используем расширенный алгоритм Евклида
        long long m = module;
        long long y = 0, x = 1;
        long long original_m = m;

        if (m == 1)
        {
            return 0;
        }

        while (a > 1)
        {
            long long q = a / m;
            long long t = m;

            m = a % m;
            a = t;
            t = y;

            y = x - q * y;
            x = t;
        }

        if (x < 0)
        {
            x += original_m;
        }

        return x;
    }

    /* static unsigned long long mod_inv(unsigned long long a, unsigned int MOD)
    {
        unsigned long long res = 1;
        unsigned long long p = MOD - 2;
        while (p)
        {
            if (p & 1)
                res = (res * a) % MOD;
            a = (a * a) % MOD;
            p >>= 1;
        }
        return res;
    } */

private:
    BigInt(std::vector<unsigned long long> vec)
    {
        bool notnull = false;
        std::vector<unsigned long long> vec_res;
        module_digits(vec, vec_res);

        for (size_t i = 0; i < vec_res.size(); i++)
        {
            if (vec_res[i] == 0 && notnull)
            {
                digits.push_back(vec_res[i]);
            }
            else if (vec_res[i] != 0)
            {
                notnull = true;
                digits.push_back(vec_res[i]);
            }
            else
            {
                continue;
            }
        }
        isNegative = false;
    }

    BigInt(std::vector<unsigned long long> vec, bool _isNegative)
    {
        bool notnull = false;
        std::vector<unsigned long long> vec_res;
        module_digits(vec, vec_res);

        for (size_t i = 0; i < vec_res.size(); i++)
        {
            if (vec_res[i] == 0 && notnull)
            {
                digits.push_back(vec_res[i]);
            }
            else if (vec_res[i] != 0)
            {
                notnull = true;
                digits.push_back(vec_res[i]);
            }
            else
            {
                continue;
            }
        }
        isNegative = _isNegative;
    }

    static void fft(std::vector<std::complex<long double>> &a, const std::complex<long double> w)
    {
        int n = (int)a.size();
        if (n == 1)
        {
            return;
        }
        std::vector<std::complex<long double>> a0(n / 2), a1(n / 2);
        std::complex<long double> wn(1, 0);
        for (int i = n / 2 - 1; i >= 0; --i)
        {
            a0[i] = a[i + n / 2] + a[i];
            a1[i] = (a[i + n / 2] - a[i]) * wn;
            wn *= w;
        }
        fft(a0, w * w);
        fft(a1, w * w);

        for (int i = n / 2 - 1; i >= 0; --i)
        {
            a[2 * i] = a1[i];
            a[2 * i + 1] = a0[i];
        }

    }

    static void normalize(std::vector<unsigned long long> &_digits, unsigned long long _module = 1000000000)
    {
        std::vector<unsigned long long> _digits_new;
        module_digits(_digits, _digits_new, _module);
        _digits = _digits_new;
    }

    static void module_digits(const std::vector<unsigned long long> &_digits, std::vector<unsigned long long> &_res_digits, unsigned long long _module = 1000000000)
    {
        unsigned long long div = 0, mod = 0;
        for (int i = _digits.size() - 1; i >= 0; --i)
        {
            mod = ((_digits[i] + div) % _module);

            _res_digits.push_back(mod);

            div = (_digits[i] + div) / _module;
        }
        if (div != 0)
        {
            _res_digits.push_back(div);
        }
        std::reverse(_res_digits.begin(), _res_digits.end());
    }

    static void take_dozen_for(std::vector<unsigned long long> &_digits, const size_t &index, unsigned long long _module = 1000000000)
    {
        if (index == 0)
        {
            return;
        }
        int take = index - 1;
        for (int i = index - 1; i >= 0 && _digits[i] == 0; i--)
        {
            _digits[i] = _module - 1;
            take--;
        }
        --_digits[take];
    }

    static void take_zeroes_from_end_ans_reverse(std::vector<unsigned long long> &_digits)
    {
        int index = _digits.size() - 1;
        while (index > 0 && _digits[index--] == 0)
        {
            _digits.pop_back();
        }
        std::reverse(_digits.begin(), _digits.end());
    }

    static std::string to_string(const BigInt &element)
    {
        std::string bigIntNumber_str = std::to_string(element.digits[0]);

        for (size_t i = 1; i < element.digits.size(); i++)
        {
            if (element.digits[i] == 0)
            {
                for (unsigned long long j = 1; j < element.module; j *= 10)
                {
                    bigIntNumber_str += '0';
                }
                continue;
            }
            else
            {
                int j = NUMBER_LENGTH(element.digits[i]);
                int mod_length = NUMBER_LENGTH(element.module) - 1;
                while (j++ < mod_length)
                {
                    bigIntNumber_str += '0';
                }
                bigIntNumber_str += std::to_string(element.digits[i]);
            }
        }
        return bigIntNumber_str;
    }

    std::vector<unsigned long long> digits;
    bool isNegative;
    /*  const  */unsigned long long module = 1000000000;//1000000000; // 1000000000;
};
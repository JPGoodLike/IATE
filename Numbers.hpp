#pragma once
#include <iostream>
#include <sstream>
#include "arrays.hpp"


namespace mbgl {

    class Number;
    struct DivModResult;

    class Number {
        private:
            DArray<short> digits;

        public:
            Number() {}
            Number(std::string s_digits) {
                digits = DArray<short>(s_digits.length());
                for (int i = 0, j = digits.Length() - 1; i < digits.Length(); i++, j--) {
                    digits[i] = (short)(s_digits[j] - '0');
                }
                Normalize();           
            }
            Number(int num) {
                std::string s_digits = std::to_string(num);
                digits = DArray<short>(s_digits.length());
                for (int i = 0, j = digits.Length() - 1; i < digits.Length(); i++, j--) {
                    digits[i] = (short)(s_digits[j] - '0');
                }
                Normalize();
            }
            Number(DArray<short> digits)
                : digits(digits) {}

            ~Number() {
                digits.Clean();
            }

            int Length() const {
                return digits.Length();
            }

            bool CheckDigits() const {
                for (int i = 0; i < digits.Length(); i++) {
                    if (digits[i] > 9) 
                        return false;
                }
                return true;
            }

            Number& Normalize() {
                int zeroQ = 0;
                for (int i = digits.Length()-1; i > 0; i--) {
                    if (digits[i] == 0) {
                        zeroQ++;
                    } else
                        break;
                }
                digits.ShrinkTop(zeroQ);
                return *this;
            }

            std::string Print() const {
                std::stringstream ss("");
                for (int i = digits.Length() - 1; i >= 0; i--)
                    ss << digits[i] << " ";
                return ss.str();
            }

            //arithmetical operations
            static Number Add2(const Number& self, const Number& other) {
                int sumLen = std::max(self.Length(), other.Length());
                Number sum = DArray<short>(sumLen);

                short rem = 0;
                for (int i = 0; i < sumLen; i++) {
                    short digit = 0;
                    if (i < self.Length())
                        digit += self.digits[i];
                    if (i < other.Length())
                        digit += other.digits[i];
                    digit += rem;
                    rem = digit / 10;
                    digit %= 10;
                    sum.digits[i] = digit;
                }
                if (rem > 0)
                    sum.digits.Push(rem);

                return sum;
            }

            static Number Sub2(const Number& self, const Number& other) {
                if (other.Length() > self.Length()) {
                    return Number(0);
                }
                Number dif = DArray<short>(self.Length());

                short rem = 0;
                for (int i = 0; i < self.Length(); i++) {
                    short digit = self.digits[i];
                    if (i < other.Length())
                        digit -= other.digits[i];
                    digit -= rem;

                    if (digit < 0) {
                        digit += 10;
                        rem = 1;
                    } else 
                        rem = 0;

                    dif.digits[i] = digit;
                }
                if (rem > 0) {
                    return Number(0);
                }

                dif.Normalize();
                return dif;
            }

            static Number Mul2(const Number& self, const Number& other) {
                Number sum = 0;
                for (int i = 0; i < other.Length(); i++) {
                    Number cSum = 0;
                    for (int j = 0; j < other.digits[i]; j++) {
                        cSum += self;
                    }

                    if (i != 0) {
                        cSum.digits.Extend(i);
                        for (int j = cSum.digits.Length()-1, k = cSum.digits.Length()-1-i; k >= 0; j--, k--) {
                            cSum.digits[j] = cSum.digits[k];
                            cSum.digits[k] = 0;
                        }
                    }
                    sum += cSum;
                }

                return sum;
            }

            static Number Div2(const Number& self, const Number& other) {
                if (other.Length() > self.Length())
                    return Number(0);

                Number quot = DArray<short>(self.Length());
                Number cDivident;
                for (int i = quot.digits.Length()-1; i >= 0; i--) {
                    short digit = 0;                
                    cDivident.digits.Insert(self.digits[i], 0);
                    cDivident.Normalize();
                    while(cDivident >= other) {
                        cDivident -= other;
                        digit++;
                    }
                    quot.digits[i] = digit;
                }

                quot.Normalize();
                return quot;
            }

            static Number Div2(const Number& self, const Number& other, Number& outRem) {
                if (other.Length() > self.Length())
                    return Number(0);

                Number quot = DArray<short>(self.Length());
                Number cDivident;
                for (int i = quot.digits.Length()-1; i >= 0; i--) {
                    short digit = 0;                
                    cDivident.digits.Insert(self.digits[i], 0);
                    cDivident.Normalize();
                    while(cDivident >= other) {
                        cDivident -= other;
                        digit++;
                    }
                    quot.digits[i] = digit;
                }

                outRem = cDivident;
                quot.Normalize();
                return quot;
            }

            static Number Pow2(const Number& self, unsigned long long power) {
                unsigned long long cPow = 1;
                Number result = self;
                while(cPow*2 <= power) {
                    cPow *= 2;
                    result *= result;
                }
                for (int i = 0; i < power - cPow; i++) {
                    result *= self;
                }
                return result;
            }

            //logic operations
            static bool IsGreater(const Number& self, const Number& other) {
                if (self.Length() > other.Length()) {
                    return true;
                } else if (self.Length() < other.Length()) {
                    return false;
                } else {
                    for (int i = self.Length()-1; i >= 0; i--) {
                        if (self.digits[i] != other.digits[i])
                            return self.digits[i] > other.digits[i];
                    }
                    return false;
                }
            }

            static bool IsLess(const Number& self, const Number& other) {
                if (self.Length() < other.Length()) {
                    return true;
                } else if (self.Length() > other.Length()) {
                    return false;
                } else {
                    for (int i = self.Length()-1; i >= 0; i--) {
                        if (self.digits[i] != other.digits[i])
                            return self.digits[i] < other.digits[i];
                    }
                    return false;
                }
            }

            static bool IsEqual(const Number& self, const Number& other) {
                if (self.Length() != other.Length()) {
                    return false;
                } else {
                    for (int i = 0; i < self.Length(); i++) {
                        if (self.digits[i] != other.digits[i])
                            return false;
                    }
                    return true;
                }
            }
            //operators overloading

            //arithmetical operators
            Number operator+(const Number& other) {
                return Add2(*this, other);
            }
            Number& operator+=(const Number& other) {
                *this = Add2(*this, other);
                return *this;
            }
            Number& operator++() {
                *this = Add2(*this, 1);
                return *this;
            }
            Number& operator++(int) {
                *this = Add2(*this, 1);
                return *this;
            }
            Number operator-(const Number& other) {
                return Sub2(*this, other);
            }
            Number& operator-=(const Number& other) {
                *this = Sub2(*this, other);
                return *this;
            }
            Number& operator--() {
                *this = Sub2(*this, 1);
                return *this;
            }
            Number& operator--(int) {
                *this = Sub2(*this, 1);
                return *this;
            }
            Number operator*(const Number& other) {
                return Mul2(*this, other);
            }
            Number& operator*=(const Number& other) {
                *this = Mul2(*this, other);
                return *this;
            }
            Number operator/(const Number& other) const {
                return Div2(*this, other);
            }
            Number& operator/=(const Number& other) {
                *this = Div2(*this, other);
                return *this;
            }
            //logical operators
            bool operator>(const Number& other) const {
                return IsGreater(*this, other);
            }
            bool operator<(const Number& other) const {
                return IsLess(*this, other);
            }
            bool operator==(const Number& other) const {
                return IsEqual(*this, other);
            }
            bool operator<=(const Number& other) const {
                return !IsGreater(*this, other);
            }
            bool operator>=(const Number& other) const {
                return !IsLess(*this, other);
            }
            bool operator!=(const Number& other) const {
                return !IsEqual(*this, other);
            }

            //other operators
            short& operator[](int i) {
                return digits[i];
            }

            friend std::ostream& operator<<(std::ostream& stream, const Number& number) {
                for (int i = number.Length() - 1; i >= 0; i--)
                    stream << number.digits[i];
                return stream;
            }
    };

    // struct DivModResult {
    //     Number div;
    //     Number mod;
    //     DivModResult(const Number& div, const Number& mod)
    //     : div(div), mod(mod) {}
    // };

    // int main() {
    //     Number n1 = (std::string)"15";
    //     Number n2 = (std::string)"0";
        
    //     cout << "n1 " << n1 << " " << n1.CheckDigits() << endl;
    //     cout << "n2 " << n2 << " " << n2.CheckDigits() << endl;
    //     cout << "n1+n2 " << n1 + n2 << endl;
    //     cout << "n2-n1 " << (n2 - n1) << endl;
    //     cout << "n1*n2 " << n1 * n2 << endl;
    //     cout << "n2/n1 " << (n2 / n1) << endl;
    //     cout << "n1^5 " << Number::Pow2(n1, 5000) << endl; 
    //     Number rem;
    //     cout << "n2 (/,%) n1 " << Number::Div2(n2, n1, rem) << " " << rem << endl;
    // }

}
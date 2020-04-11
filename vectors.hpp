#pragma once
#include <iostream>
#include <math.h>

namespace mbgl {

    template<typename T>
    class Vector2 {
        public:
            T x, y;
            Vector2(T x = 0, T y = 0) {
                this->x = x;
                this->y = y;
            }

            T Magnitude() const {
                return sqrt(pow(this->x, 2) + pow(this->y, 2));
            }
            Vector2 Normalized() const {
                T magnitude = this->Magnitude();
                return Vector2(this->x / magnitude, this->y / magnitude);
            }

            Vector2 operator+(const Vector2& other) const {
                return Vector2(this->x + other.x, this->y + other.y);
            }
            Vector2 operator-(const Vector2& other) const {
                return Vector2(this->x - other.x, this->y - other.y);
            }
            Vector2 operator*(const T& other) const {
                return Vector2(this->x * other, this->y * other);
            }
            Vector2 operator/(const T& other) const {
                return Vector2(this->x / other, this->y / other);
            }
            friend std::ostream& operator<<(std::ostream& stream, const Vector2 other) {
                stream << other.x << " " << other.y;
                return stream;
            }
    };

}
#pragma once

// #include "Vector2.hpp"
// #include "Vector3.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <math.h>

namespace mbgl {
    template<typename T>
    class Vector2 {
    public:
        T x, y;
        Vector2(T x = 0, T y = 0)
        : x(x), y(y) {}
        template <typename S>
        Vector2(const sf::Vector2<S> v)
        : x((T)v.x), y((T)v.y) {}

        T Magnitude() const {
            return sqrt(pow(this->x, 2) + pow(this->y, 2));
        }
        Vector2 Normalized() const {
            T magnitude = this->Magnitude();
            return Vector2(this->x / magnitude, this->y / magnitude);
        }

        Vector2 Rotated(T angle) const {
            float andleInRad = angle * (M_PI / 180);
            return Vector2(x*cos(andleInRad) - y*sin(andleInRad), x*sin(andleInRad) + y*cos(andleInRad));
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
        Vector2& operator+=(const Vector2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }
        Vector2& operator-=(const Vector2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }
        Vector2& operator*=(const T& other) {
            x *= other;
            y *= other;
            return *this;
        }
        Vector2& operator/=(const T& other) {
            x /= other;
            y /= other;
            return *this;
        }
        friend std::ostream& operator<<(std::ostream& stream, const Vector2 other) {
            stream << other.x << " " << other.y;
            return stream;
        }

        template <typename S>
        operator Vector2<S>() const {
            return Vector2<S>((S)x, (S)y);
        }
        template <typename S>
        operator sf::Vector2<S>() const {
            return sf::Vector2<S>((S)x, (S)y);
        }

        static Vector2 Zero() {
            return Vector2(0, 0);
        }
        static Vector2 One() {
            return Vector2(1, 1);
        }
    };

    template<typename T>
    class Vector3 {
    public:
        T x, y, z;
        Vector3(T x = 0, T y = 0, T z = 0)
        : x(x), y(y), z(z) {}
        // Vector3(const sf::Vector3<T> v)
        // : x(v.x), y(v.y), z(v.z) {}
        Vector3(const Vector2<T> v)
        : x(v.x), y(v.y), z(0) {}
        // Vector3(const sf::Vector2<T> v)
        // : x(v.x), y(v.y), z(0) {}

        T Magnitude() const {
            return sqrt(x*x + y*y + z*z);
        }
        Vector3 Normalized() const {
            T magnitude = Magnitude();
            return Vector3(x / magnitude, y / magnitude, z / magnitude);
        }

        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        Vector3 operator*(const T& other) const {
            return Vector3(x * other, y * other, z * other);
        }
        Vector3 operator/(const T& other) const {
            return Vector3(x / other, y / other, z / other);
        }
        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }
        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }
        Vector3& operator*=(const T& other) {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        }
        Vector3& operator/=(const T& other) {
            x /= other;
            y /= other;
            z /= other;
            return *this;
        }
        friend std::ostream& operator<<(std::ostream& stream, const Vector3 other) {
            stream << other.x << " " << other.y << " " << other.z;
            return stream;
        }

        template <typename S>
        operator Vector2<S>() {
            return Vector2<S>((S)x, (S)y);
        }
        template <typename S>
        operator sf::Vector2<S>() {
            return sf::Vector2<S>((S)x, (S)y);
        }
        template <typename S>
        operator Vector3<S>() {
            return Vector3<S>((S)x, (S)y, (S)z);
        }
        template <typename S>
        operator sf::Vector3<S>() {
            return sf::Vector3<S>((S)x, (S)y, (S)z);
        }

        static Vector3 Zero() {
            return Vector3(0, 0, 0);
        }
        static Vector3 One() {
            return Vector3(1, 1, 1);
        }
    };

    using vec2 = Vector2<float>;
    using vec2i = Vector2<int>;
    using vec2u = Vector2<unsigned int>;    
    using vec2d = Vector2<double>;

    using vec3 = Vector3<float>;
    using vec3i = Vector3<int>;
    using vec3u = Vector3<unsigned int>;    
    using vec3d = Vector3<double>;
}
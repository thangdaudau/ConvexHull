#pragma once

namespace Core {
    template<typename T>
    struct point3D;


    // Dot product
    template<typename T>
    T dot(const point3D<T>& lhs, const point3D<T>& rhs);

    // Cross product
    template<typename T>
    point3D<T> cross(const point3D<T>& lhs, const point3D<T>& rhs);

    // Norm of a vector :V
    template<typename T>
    T norm(const point3D<T>& p);

    // Absolute of a vector :V
    template<typename T>
    double abs(const point3D<T>& p);

    // U == std::istream? But done this way because of fast input.
    template<typename T, typename U>
    U& operator<<(U& stream, const point3D<T>& p);

    // U == std::ostream? But done this way because of fast output.
    template<typename T, typename U>
    U& operator>>(U& stream, point3D<T>& p);





    template<typename T>
    struct point3D {
        T x, y, z;

        point3D(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {
        }

        point3D& operator+=(const point3D& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        point3D& operator-=(const point3D& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        point3D& operator*=(const T& a) {
            z *= a;
            x *= a;
            y *= a;
            return *this;
        }

        point3D& operator/=(const T& a) {
            z /= a;
            x /= a;
            y /= a;
            return *this;
        }

        friend point3D operator+(const point3D& lhs, const point3D& rhs) {
            return point3D(lhs) += rhs;
        }
        friend point3D operator-(const point3D& lhs, const point3D& rhs) {
            return point3D(lhs) -= rhs;
        }
        friend point3D operator*(const point3D& lhs, const T& rhs) {
            return point3D(lhs) *= rhs;
        }
        friend point3D operator*(const T& lhs, const point3D& rhs) {
            return point3D(rhs) *= lhs;
        }
        friend point3D operator/(const point3D& lhs, const T& rhs) {
            return point3D(lhs) /= rhs;
        }
        point3D operator-() {
            return -1 * *this;
        }

        friend bool operator==(const point3D& lhs, const point3D& rhs) {
            return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
        }
        friend bool operator!=(const point3D& lhs, const point3D& rhs) {
            return !(lhs == rhs);
        }
        friend bool operator<(const point3D& lhs, const point3D& rhs) {
            return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
        }
        friend bool operator>(const point3D& lhs, const point3D& rhs) {
            return std::tie(lhs.x, lhs.y, lhs.z) > std::tie(rhs.x, rhs.y, rhs.z);
        }
    };

    template<typename T>
    T dot(const point3D<T>& lhs, const point3D<T>& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    template<typename T>
    point3D<T> cross(const point3D<T>& lhs, const point3D<T>& rhs) {
        return point3D<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
    }

    template<typename T>
    T norm(const point3D<T>& p) {
        return dot(p, p);
    }

    template<typename T>
    double abs(const point3D<T>& p) {
        return sqrt(norm(p));
    }

    template<typename T, typename U>
    U& operator<<(U& stream, const point3D<T>& p) {
        return stream << '(' << p.x << ", " << p.y << ", " << p.z << ')';
    }

    template<typename T, typename U>
    U& operator>>(U& stream, point3D<T>& p) {
        return stream >> p.x >> p.y >> p.z;
    }

}
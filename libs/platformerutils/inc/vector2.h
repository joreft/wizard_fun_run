#pragma once

namespace jeagle
{

template<typename T>
struct Vector2
{
    T x;
    T y;
};

template <typename T>
inline constexpr Vector2<T> operator-(Vector2<T> const& rhs)
{
    return Vector2<T>{-rhs.x, -rhs.y};
}

template <typename T>
inline constexpr Vector2<T> operator-(Vector2<T> const& lhs, Vector2<T> const& rhs)
{
    return Vector2<T>{lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T>
inline constexpr Vector2<T> operator+(Vector2<T> const& lhs, Vector2<T> const& rhs)
{
    return Vector2<T>{lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T>
inline constexpr Vector2<T>& operator-=(Vector2<T>& lhs, Vector2<T> const& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

template <typename T>
inline constexpr Vector2<T>& operator+=(Vector2<T>& lhs, Vector2<T> const& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

template <typename T>
inline constexpr Vector2<T> operator*(T lhs, Vector2<T> const& rhs)
{
    return Vector2<T>{rhs.x * lhs, rhs.y * lhs};
}

template <typename T>
inline constexpr Vector2<T> operator*(Vector2<T> const& lhs, T rhs)
{
    return Vector2<T>{lhs.x * rhs, lhs.y * rhs};
}

template <typename T>
inline constexpr Vector2<T> operator/(T lhs, Vector2<T> const& rhs)
{
    return Vector2<T>{rhs.x / lhs, rhs.y / lhs};
}

template <typename T>
inline constexpr Vector2<T> operator/(Vector2<T> const& lhs, T rhs)
{
    return Vector2<T>{lhs.x / rhs, lhs.y / rhs};
}

template <typename T>
inline constexpr Vector2<T> operator/=(Vector2<T>& lhs, Vector2<T> const& rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

template <typename T>
inline constexpr Vector2<T> operator*=(Vector2<T>& lhs, Vector2<T> const& rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

template <typename T>
inline constexpr bool operator==(Vector2<T> const& lhs, Vector2<T> const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

template <typename T>
inline constexpr bool operator!=(Vector2<T> const& lhs, Vector2<T> const& rhs)
{
    return !(lhs == rhs);
}

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

} // namespace jeagle
#pragma once

#include <cassert>
#include <concepts>
#include <limits>

#include <nyx/core/math/angle.h>
#include <nyx/core/math/util.h>
#include <nyx/core/math/vec.h>

namespace nyx {

template <std::floating_point T> struct quat {
    T x{0}, y{0}, z{0}, w{0};

    [[nodiscard]] static constexpr quat from(vec3<T> start, vec3<T> end) noexcept;
    [[nodiscard]] static constexpr quat from(vec3<T> axis, Angle<T> angle) noexcept;
    [[nodiscard]] static constexpr quat from(Angle<T> pitch, Angle<T> yaw, Angle<T> roll) noexcept;

    [[nodiscard]] static constexpr T dot(const quat& q0, const quat& q1) noexcept;

    constexpr quat() noexcept = default;
    constexpr quat(T x, T y, T z, T w) noexcept;

    [[nodiscard]] constexpr quat operator+() const noexcept;
    [[nodiscard]] constexpr quat operator-() const noexcept;

    constexpr quat& operator+=(const quat& rhs) noexcept;
    constexpr quat& operator-=(const quat& rhs) noexcept;
    constexpr quat& operator*=(this quat& lhs, const quat& rhs) noexcept;
    constexpr quat& operator*=(T scalar) noexcept;
    constexpr quat& operator/=(T scalar) noexcept;

    [[nodiscard]] constexpr quat operator+(this quat lhs, const quat& rhs) noexcept;
    [[nodiscard]] constexpr quat operator-(this quat lhs, const quat& rhs) noexcept;
    [[nodiscard]] constexpr quat operator*(this quat lhs, const quat& rhs) noexcept;
    [[nodiscard]] constexpr vec3<T> operator*(const vec3<T>& v) const noexcept;
    [[nodiscard]] constexpr quat operator*(this quat q, T scalar) noexcept;
    [[nodiscard]] constexpr quat operator/(this quat q, T scalar) noexcept;

    [[nodiscard]] friend constexpr auto operator<=>(const quat&, const quat&) noexcept = default;

    [[nodiscard]] constexpr vec3<T> euler() const noexcept;
    [[nodiscard]] constexpr vec3<T> rotate(const vec3<T>& v) const noexcept;
    [[nodiscard]] constexpr quat conjugate() const noexcept;
    [[nodiscard]] constexpr T length_sq() const noexcept;
    [[nodiscard]] constexpr T length() const noexcept;
    [[nodiscard]] constexpr quat norm(this quat self) noexcept;
};

template <std::floating_point T> [[nodiscard]] constexpr vec3<T> operator*(const vec3<T>& v, const quat<T>& q) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr quat<T> operator*(T scalar, quat<T> q) noexcept;

// Type aliases
using quatf = quat<float>;
using quatd = quat<double>;
using quatld = quat<long double>;

template <std::floating_point T> constexpr quat<T> quat<T>::from(vec3<T> start, vec3<T> end) noexcept {
    start = start.norm();
    end = end.norm();

    const T dot{vec3<T>::dot(start, end)};

    if (dot < T{-1} + std::numeric_limits<T>::epsilon()) {
        auto axis{
            abs(start.x) > T{0.5}
              ? vec3<T>{-start.y, start.x, T{0}}
              : vec3<T>{T{0}, -start.z, start.y}
        };
        axis = axis.norm();
        return {axis.x, axis.y, axis.z, T{0}};
    }

    const auto cross{vec3<T>::cross(start, end)};
    const T m{sqrt(T{2} * (T{1} + dot))};
    const T m_inv{T{1} / m};

    return {cross.x * m_inv, cross.y * m_inv, cross.z * m_inv, m * T{0.5}};
}

template <std::floating_point T> constexpr quat<T> quat<T>::from(vec3<T> axis, Angle<T> angle) noexcept {
    axis = axis.norm();
    angle *= T{0.5};

    const T s{angle.sin()};
    return {axis.x * s, axis.y * s, axis.z * s, angle.cos()};
}

template <std::floating_point T> constexpr quat<T> quat<T>::from(Angle<T> pitch, Angle<T> yaw, Angle<T> roll) noexcept {
    pitch *= T{0.5};
    yaw *= T{0.5};
    roll *= T{0.5};

    const T sp{pitch.sin()}, cp{pitch.cos()}, sy{yaw.sin()}, cy{yaw.cos()}, sr{roll.sin()}, cr{roll.cos()};
    return {sp * cy * cr - cp * sy * sr, cp * sy * cr + sp * cy * sr, cp * cy * sr - sp * sy * cr,
            cp * cy * cr + sp * sy * sr};
}

template <std::floating_point T> constexpr T quat<T>::dot(const quat& q0, const quat& q1) noexcept {
    return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

template <std::floating_point T> constexpr quat<T>::quat(T x, T y, T z, T w) noexcept : x{x}, y{y}, z{z}, w{w} {}

template <std::floating_point T> constexpr quat<T> quat<T>::operator+() const noexcept { return *this; }

template <std::floating_point T> constexpr quat<T> quat<T>::operator-() const noexcept { return {-x, -y, -z, -w}; }

template <std::floating_point T> constexpr quat<T>& quat<T>::operator+=(const quat& rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

template <std::floating_point T> constexpr quat<T>& quat<T>::operator-=(const quat& rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

template <std::floating_point T> constexpr quat<T>& quat<T>::operator*=(this quat& lhs, const quat& rhs) noexcept {
    lhs = quat{lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
               lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
               lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
               lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z};
    return lhs;
}

template <std::floating_point T> constexpr quat<T>& quat<T>::operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

template <std::floating_point T> constexpr quat<T>& quat<T>::operator/=(T scalar) noexcept {
    assert(scalar != T{0});
    *this *= T{1} / scalar;
    return *this;
}

template <std::floating_point T> constexpr quat<T> quat<T>::operator+(this quat lhs, const quat& rhs) noexcept {
    lhs += rhs;
    return lhs;
}

template <std::floating_point T> constexpr quat<T> quat<T>::operator-(this quat lhs, const quat& rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

template <std::floating_point T> constexpr quat<T> quat<T>::operator*(this quat lhs, const quat& rhs) noexcept {
    lhs *= rhs;
    return lhs;
}

template <std::floating_point T> constexpr vec3<T> quat<T>::operator*(const vec3<T>& v) const noexcept {
    return rotate(v);
}

template <std::floating_point T> constexpr quat<T> quat<T>::operator*(this quat q, T scalar) noexcept {
    q *= scalar;
    return q;
}

template <std::floating_point T> constexpr quat<T> quat<T>::operator/(this quat q, T scalar) noexcept {
    assert(scalar != T{0} && "Division by zero");
    q /= scalar;
    return q;
}

template <std::floating_point T> constexpr vec3<T> quat<T>::euler() const noexcept {
    const vec2<T> pitch{T{2} * (w * x + y * z), T{1} - T{2} * (x * x + y * y)};
    return vec3<T>{(pitch.x == T{0} && pitch.y == T{0}) ? T{2} * atan(x / w) : atan(pitch.x / pitch.y),
                   asin(clamp(T{2} * (w * y - z * x), T{-1}, T{1})),
                   atan((T{2} * (w * z + x * y)) / (T{1} - T{2} * (y * y + z * z)))};
}

template <std::floating_point T> constexpr vec3<T> quat<T>::rotate(const vec3<T>& v) const noexcept {
    const vec3<T> axis{x, y, z};
    const vec3<T> uv{vec3<T>::cross(axis, v)};
    return v + T{2} * (uv * w + vec3<T>::cross(axis, uv));
}

template <std::floating_point T> constexpr quat<T> quat<T>::conjugate() const noexcept { return {-x, -y, -z, w}; }

template <std::floating_point T> constexpr T quat<T>::length_sq() const noexcept { return dot(*this, *this); }

template <std::floating_point T> constexpr T quat<T>::length() const noexcept { return sqrt(length_sq()); }

template <std::floating_point T> constexpr quat<T> quat<T>::norm(this quat self) noexcept {
    if (const T len{self.length()}; len > T{0}) {
        self *= T{1} / len;
    }
    return self;
}

template <std::floating_point T> constexpr vec3<T> operator*(const vec3<T>& v, const quat<T>& q) noexcept {
    return q.rotate(v);
}

template <std::floating_point T> constexpr quat<T> operator*(T scalar, quat<T> q) noexcept {
    q *= scalar;
    return q;
}

} // namespace nyx

// MIT License
//
// Copyright (c) 2026 Filippos Gleglakos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

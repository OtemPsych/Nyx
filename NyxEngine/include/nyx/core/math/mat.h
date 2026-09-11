#pragma once

#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>

#include <nyx/core/math/angle.h>
#include <nyx/core/math/quat.h>
#include <nyx/core/math/util.h>
#include <nyx/core/math/vec.h>

namespace nyx {

template <traits::Scalar T, std::size_t R, std::size_t C> struct mat {
    static_assert(R >= 2 && C >= 2, "Matrix dimensions must be at least 2x2");

    std::array<T, R * C> data{};

    [[nodiscard]] static constexpr mat identity() noexcept requires (R == C);
    [[nodiscard]] static constexpr mat orthographic(T left, T right, T bottom, T top, T near, T far) noexcept
        requires (R == 4 && C == 4);
    [[nodiscard]] static constexpr mat orthographic(T left, T right, T bottom, T top) noexcept
        requires (R == 4 && C == 4);
    template <std::floating_point U = T> requires (R == 4 && C == 4)
    [[nodiscard]] static constexpr mat<U, R, C> perspective(Angle<U> fov, T aspect_ratio, T near, T far) noexcept;
    [[nodiscard]] static constexpr mat lookat(const vec3<T>& eye, const vec3<T>& target, const vec3<T>& up) noexcept
        requires (std::floating_point<T> && R == 4 && C == 4);

    [[nodiscard]] static constexpr mat translate(const vec<T, R - 1>& translation) noexcept requires (R == C && R > 2);
    template <std::floating_point U = T> requires (R == 4 && C == 4)
    [[nodiscard]] static constexpr mat<U, R, C> rotate(const quat<U>& q) noexcept;
    [[nodiscard]] static constexpr mat scale(const vec<T, R - 1>& factors) noexcept requires (R == C && R > 2);

    [[nodiscard]] static constexpr std::size_t rows() noexcept;
    [[nodiscard]] static constexpr std::size_t cols() noexcept;

    constexpr mat() noexcept = default;
    constexpr explicit mat(std::array<T, R * C> data) noexcept : data{data} {}

    [[nodiscard]] constexpr mat operator+() const noexcept;
    [[nodiscard]] constexpr mat operator-() const noexcept;

    constexpr mat& operator+=(const mat& rhs) noexcept;
    constexpr mat& operator-=(const mat& rhs) noexcept;
    constexpr mat& operator*=(const mat<T, C, C>& rhs) noexcept;
    constexpr mat& operator*=(T scalar) noexcept;
    constexpr mat& operator/=(T scalar) noexcept;

    [[nodiscard]] constexpr mat operator+(this mat lhs, const mat& rhs) noexcept;
    [[nodiscard]] constexpr mat operator-(this mat lhs, const mat& rhs) noexcept;
    [[nodiscard]] constexpr mat operator*(this mat m, T scalar) noexcept;
    template <std::size_t C2>
    [[nodiscard]] constexpr mat<T, R, C2> operator*(this const mat& lhs, const mat<T, C, C2>& rhs) noexcept;
    [[nodiscard]] constexpr vec<T, R> operator*(const vec<T, C>& v) const noexcept;
    [[nodiscard]] constexpr mat operator/(this mat m, T scalar) noexcept;

    [[nodiscard]] constexpr auto&& operator[](this auto&& self, std::size_t i) noexcept;
    [[nodiscard]] constexpr auto&& operator[](this auto&& self, std::size_t r, std::size_t c) noexcept;
    [[nodiscard]] friend constexpr auto operator<=>(const mat&, const mat&) noexcept = default;

    [[nodiscard]] constexpr vec<T, C> row(this const mat& self, std::size_t index) noexcept;
    [[nodiscard]] constexpr vec<T, R> col(this const mat& self, std::size_t index) noexcept;
    [[nodiscard]] constexpr mat<T, R - 1, C - 1> submatrix(this const mat& self, std::size_t skip_row,
                                                           std::size_t skip_col) noexcept requires (R > 2 && C > 2);
    [[nodiscard]] constexpr T determinant(this const mat& self) noexcept requires (traits::SignedScalar<T> && R == C);
    [[nodiscard]] constexpr mat adjoint(this const mat& self) noexcept requires (traits::SignedScalar<T> && R == C);
    [[nodiscard]] constexpr mat invert() const noexcept requires (std::floating_point<T> && R == C);
    [[nodiscard]] constexpr mat<T, C, R> transpose(this const mat& self) noexcept;
};

template <traits::Scalar T, std::size_t R, std::size_t C>
[[nodiscard]] constexpr mat<T, R, C> operator*(T scalar, mat<T, R, C> m) noexcept;

// Type Aliases
template <typename T> using mat2 = mat<T, 2, 2>;
using mat2f = mat2<float>;
using mat2d = mat2<double>;
using mat2ld = mat2<long double>;
using mat2i = mat2<int>;
using mat2l = mat2<long>;
using mat2ll = mat2<long long>;
using mat2u = mat2<unsigned int>;
using mat2ul = mat2<unsigned long>;
using mat2ull = mat2<unsigned long long>;

using mat2i8 = mat2<std::int8_t>;
using mat2i16 = mat2<std::int16_t>;
using mat2i32 = mat2<std::int32_t>;
using mat2i64 = mat2<std::int64_t>;
using mat2ifast8 = mat2<std::int_fast8_t>;
using mat2ifast16 = mat2<std::int_fast16_t>;
using mat2ifast32 = mat2<std::int_fast32_t>;
using mat2ifast64 = mat2<std::int_fast64_t>;
using mat2ileast8 = mat2<std::int_least8_t>;
using mat2ileast16 = mat2<std::int_least16_t>;
using mat2ileast32 = mat2<std::int_least32_t>;
using mat2ileast64 = mat2<std::int_least64_t>;

using mat2u8 = mat2<std::uint8_t>;
using mat2u16 = mat2<std::uint16_t>;
using mat2u32 = mat2<std::uint32_t>;
using mat2u64 = mat2<std::uint64_t>;
using mat2ufast8 = mat2<std::uint_fast8_t>;
using mat2ufast16 = mat2<std::uint_fast16_t>;
using mat2ufast32 = mat2<std::uint_fast32_t>;
using mat2ufast64 = mat2<std::uint_fast64_t>;
using mat2uleast8 = mat2<std::uint_least8_t>;
using mat2uleast16 = mat2<std::uint_least16_t>;
using mat2uleast32 = mat2<std::uint_least32_t>;
using mat2uleast64 = mat2<std::uint_least64_t>;

template <typename T> using mat3 = mat<T, 3, 3>;
using mat3f = mat3<float>;
using mat3d = mat3<double>;
using mat3ld = mat3<long double>;
using mat3i = mat3<int>;
using mat3l = mat3<long>;
using mat3ll = mat3<long long>;
using mat3u = mat3<unsigned int>;
using mat3ul = mat3<unsigned long>;
using mat3ull = mat3<unsigned long long>;

using mat3i8 = mat3<std::int8_t>;
using mat3i16 = mat3<std::int16_t>;
using mat3i32 = mat3<std::int32_t>;
using mat3i64 = mat3<std::int64_t>;
using mat3ifast8 = mat3<std::int_fast8_t>;
using mat3ifast16 = mat3<std::int_fast16_t>;
using mat3ifast32 = mat3<std::int_fast32_t>;
using mat3ifast64 = mat3<std::int_fast64_t>;
using mat3ileast8 = mat3<std::int_least8_t>;
using mat3ileast16 = mat3<std::int_least16_t>;
using mat3ileast32 = mat3<std::int_least32_t>;
using mat3ileast64 = mat3<std::int_least64_t>;

using mat3u8 = mat3<std::uint8_t>;
using mat3u16 = mat3<std::uint16_t>;
using mat3u32 = mat3<std::uint32_t>;
using mat3u64 = mat3<std::uint64_t>;
using mat3ufast8 = mat3<std::uint_fast8_t>;
using mat3ufast16 = mat3<std::uint_fast16_t>;
using mat3ufast32 = mat3<std::uint_fast32_t>;
using mat3ufast64 = mat3<std::uint_fast64_t>;
using mat3uleast8 = mat3<std::uint_least8_t>;
using mat3uleast16 = mat3<std::uint_least16_t>;
using mat3uleast32 = mat3<std::uint_least32_t>;
using mat3uleast64 = mat3<std::uint_least64_t>;

template <typename T> using mat4 = mat<T, 4, 4>;
using mat4f = mat4<float>;
using mat4d = mat4<double>;
using mat4ld = mat4<long double>;
using mat4i = mat4<int>;
using mat4l = mat4<long>;
using mat4ll = mat4<long long>;
using mat4u = mat4<unsigned int>;
using mat4ul = mat4<unsigned long>;
using mat4ull = mat4<unsigned long long>;

using mat4i8 = mat4<std::int8_t>;
using mat4i16 = mat4<std::int16_t>;
using mat4i32 = mat4<std::int32_t>;
using mat4i64 = mat4<std::int64_t>;
using mat4ifast8 = mat4<std::int_fast8_t>;
using mat4ifast16 = mat4<std::int_fast16_t>;
using mat4ifast32 = mat4<std::int_fast32_t>;
using mat4ifast64 = mat4<std::int_fast64_t>;
using mat4ileast8 = mat4<std::int_least8_t>;
using mat4ileast16 = mat4<std::int_least16_t>;
using mat4ileast32 = mat4<std::int_least32_t>;
using mat4ileast64 = mat4<std::int_least64_t>;

using mat4u8 = mat4<std::uint8_t>;
using mat4u16 = mat4<std::uint16_t>;
using mat4u32 = mat4<std::uint32_t>;
using mat4u64 = mat4<std::uint64_t>;
using mat4ufast8 = mat4<std::uint_fast8_t>;
using mat4ufast16 = mat4<std::uint_fast16_t>;
using mat4ufast32 = mat4<std::uint_fast32_t>;
using mat4ufast64 = mat4<std::uint_fast64_t>;
using mat4uleast8 = mat4<std::uint_least8_t>;
using mat4uleast16 = mat4<std::uint_least16_t>;
using mat4uleast32 = mat4<std::uint_least32_t>;
using mat4uleast64 = mat4<std::uint_least64_t>;

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::identity() noexcept requires (R == C) {
    mat res{};
    for (std::size_t i{0}; i < C; ++i) {
        res[i, i] = T{1};
    }
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::orthographic(T left, T right, T bottom, T top, T near, T far) noexcept
    requires (R == 4 && C == 4) {
    const T x{right - left};
    const T y{top - bottom};
    const T z{far - near};

    mat res{mat::identity()};
    res[0, 0] = T{2} / x;
    res[1, 1] = T{2} / y;
    res[2, 2] = T{-2} / z;
    res[0, 3] = -((right + left) / x);
    res[1, 3] = -((top + bottom) / y);
    res[2, 3] = -((far + near) / z);
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::orthographic(T left, T right, T bottom, T top) noexcept requires (R == 4 && C == 4)
{
    const T x{right - left};
    const T y{top - bottom};

    mat res{mat::identity()};
    res[0, 0] = T{2} / x;
    res[1, 1] = T{2} / y;
    res[2, 2] = T{-1};
    res[0, 3] = -((right + left) / x);
    res[1, 3] = -((top + bottom) / y);
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
template <std::floating_point U> requires (R == 4 && C == 4)
constexpr mat<U, R, C> mat<T, R, C>::perspective(Angle<U> fov, T aspect_ratio, T near, T far) noexcept {
    const T term{(fov * T{0.5}).tan()};

    mat res{};
    res[0, 0] = T{1} / (aspect_ratio * term);
    res[1, 1] = T{1} / term;
    res[2, 2] = -((far + near) / (far - near));
    res[3, 2] = T{-1};
    res[2, 3] = -((T{2} * far * near) / (far - near));
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::lookat(const vec3<T>& eye, const vec3<T>& target, const vec3<T>& up) noexcept
    requires (std::floating_point<T> && R == 4 && C == 4) {
    const auto fwd = (target - eye).norm();
    const auto side = vec3<T>::cross(fwd, up).norm();
    const auto true_up = vec3<T>::cross(side, fwd);

    auto res = mat::identity();
    res[0, 0] = side.x;
    res[0, 1] = side.y;
    res[0, 2] = side.z;

    res[1, 0] = true_up.x;
    res[1, 1] = true_up.y;
    res[1, 2] = true_up.z;

    res[2, 0] = -fwd.x;
    res[2, 1] = -fwd.y;
    res[2, 2] = -fwd.z;

    res[0, 3] = -vec3<T>::dot(side, eye);
    res[1, 3] = -vec3<T>::dot(true_up, eye);
    res[2, 3] = vec3<T>::dot(fwd, eye);

    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::translate(const vec<T, R - 1>& translation) noexcept requires (R == C && R > 2) {
    mat result{mat::identity()};
    for (std::size_t i{0}; i < R - 1; ++i) {
        result[i, C - 1] = translation[i];
    }
    return result;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
template <std::floating_point U> requires (R == 4 && C == 4)
constexpr mat<U, R, C> mat<T, R, C>::rotate(const quat<U>& q) noexcept {
    const T xx{q.x * q.x};
    const T xy{q.x * q.y};
    const T xz{q.x * q.z};
    const T xw{q.x * q.w};

    const T yy{q.y * q.y};
    const T yz{q.y * q.z};
    const T yw{q.y * q.w};

    const T zz{q.z * q.z};
    const T zw{q.z * q.w};

    auto m = mat::identity();
    m[0, 0] = 1 - 2 * (yy + zz);
    m[0, 1] = 2 * (xy - zw);
    m[0, 2] = 2 * (xz + yw);

    m[1, 0] = 2 * (xy + zw);
    m[1, 1] = 1 - 2 * (xx + zz);
    m[1, 2] = 2 * (yz - xw);

    m[2, 0] = 2 * (xz - yw);
    m[2, 1] = 2 * (yz + xw);
    m[2, 2] = 1 - 2 * (xx + yy);

    return m;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::scale(const vec<T, R - 1>& factors) noexcept requires (R == C && R > 2) {
    auto m = mat::identity();
    for (std::size_t i{0}; i < R - 1; ++i) {
        m[i, i] = factors[i];
    }
    return m;
}

template <traits::Scalar T, std::size_t R, std::size_t C> constexpr std::size_t mat<T, R, C>::rows() noexcept {
    return R;
}

template <traits::Scalar T, std::size_t R, std::size_t C> constexpr std::size_t mat<T, R, C>::cols() noexcept {
    return C;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator+() const noexcept {
    return *this;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator-() const noexcept {
    mat res{};
    for (std::size_t i{0}; i < R * C; ++i) {
        res.data[i] = -data[i];
    }
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator+=(const mat& rhs) noexcept {
    for (std::size_t i{0}; i < R * C; ++i) {
        data[i] += rhs.data[i];
    }
    return *this;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator-=(const mat& rhs) noexcept {
    for (std::size_t i{0}; i < R * C; ++i) {
        data[i] -= rhs.data[i];
    }
    return *this;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator*=(const mat<T, C, C>& rhs) noexcept {
    *this = *this * rhs;
    return *this;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator*=(T scalar) noexcept {
    for (std::size_t i{0}; i < R * C; ++i) {
        data[i] *= scalar;
    }
    return *this;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator/=(T scalar) noexcept {
    assert(scalar != T{0} && "Division by zero");
    for (std::size_t i{0}; i < R * C; ++i) {
        data[i] /= scalar;
    }
    return *this;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator+(this mat lhs, const mat& rhs) noexcept {
    lhs += rhs;
    return lhs;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator-(this mat lhs, const mat& rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator*(this mat m, T scalar) noexcept {
    m *= scalar;
    return m;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
template <std::size_t C2>
constexpr mat<T, R, C2> mat<T, R, C>::operator*(this const mat& lhs, const mat<T, C, C2>& rhs) noexcept {
    mat<T, R, C2> res{};
    for (std::size_t j{0}; j < C2; ++j) {
        for (std::size_t k{0}; k < C; ++k) {
            const T rhs_val{rhs[k, j]};
            for (std::size_t i{0}; i < R; ++i) {
                res[i, j] += lhs[i, k] * rhs_val;
            }
        }
    }
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr vec<T, R> mat<T, R, C>::operator*(const vec<T, C>& v) const noexcept {
    vec<T, R> res{};
    for (std::size_t i{0}; i < R; ++i) {
        res[i] = vec<T, C>::dot(row(i), v);
    }
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator/(this mat m, T scalar) noexcept {
    assert(scalar != T{0} && "Division by zero");
    m /= scalar;
    return m;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr auto&& mat<T, R, C>::operator[](this auto&& self, std::size_t i) noexcept {
    assert(i < self.data.size());
    return self.data[i];
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr auto&& mat<T, R, C>::operator[](this auto&& self, std::size_t r, std::size_t c) noexcept {
    assert(r < R && c < C);
    return self.data[c * R + r];
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr vec<T, C> mat<T, R, C>::row(this const mat& self, std::size_t index) noexcept {
    assert(index < R);

    vec<T, C> result;
    for (std::size_t i{0}; i < C; ++i) {
        result[i] = self[index, i];
    }
    return result;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr vec<T, R> mat<T, R, C>::col(this const mat& self, std::size_t index) noexcept {
    assert(index < C);

    vec<T, R> result;
    for (std::size_t i{0}; i < R; ++i) {
        result[i] = self[i, index];
    }
    return result;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R - 1, C - 1> mat<T, R, C>::submatrix(this const mat& self, std::size_t skip_row,
                                                       std::size_t skip_col) noexcept requires (R > 2 && C > 2) {
    mat<T, R - 1, C - 1> res{};
    std::size_t current_col{0};
    for (std::size_t c{0}; c < C; ++c) {
        if (c == skip_col) {
            continue;
        }
        std::size_t current_row{0};
        for (std::size_t r{0}; r < R; ++r) {
            if (r == skip_row) {
                continue;
            }
            res[current_row++, current_col] = self[r, c];
        }
        ++current_col;
    }
    return res;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr T mat<T, R, C>::determinant(this const mat& self) noexcept requires (traits::SignedScalar<T> && R == C) {
    if constexpr (R == 2) {
        return self[0, 0] * self[1, 1] - self[1, 0] * self[0, 1];
    } else {
        T det{0};
        T sign{1};
        for (std::size_t i{0}; i < C; ++i) {
            det += sign * self[0, i] * self.submatrix(0, i).determinant();
            sign = -sign;
        }
        return det;
    }
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::adjoint(this const mat& self) noexcept requires (traits::SignedScalar<T> && R == C)
{
    mat adj{};
    if constexpr (R == 2) {
        adj[0, 0] = self[1, 1];
        adj[0, 1] = -self[0, 1];
        adj[1, 0] = -self[1, 0];
        adj[1, 1] = self[0, 0];
    } else {
        for (std::size_t j{0}; j < C; ++j) {
            for (std::size_t i{0}; i < R; ++i) {
                const T sign{((i + j) % 2 == 0) ? T{1} : T{-1}};
                adj[j, i] = sign * self.submatrix(i, j).determinant();
            }
        }
    }
    return adj;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::invert() const noexcept requires (std::floating_point<T> && R == C) {
    const T det{determinant()};
    assert(det != T{0});

    const T inv_det{T{1} / det};
    return adjoint() * inv_det;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, C, R> mat<T, R, C>::transpose(this const mat& self) noexcept {
    mat<T, C, R> result{};
    for (std::size_t j{0}; j < C; ++j) {
        for (std::size_t i{0}; i < R; ++i) {
            result[j, i] = self[i, j];
        }
    }
    return result;
}

template <traits::Scalar T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> operator*(T scalar, mat<T, R, C> m) noexcept {
    m *= scalar;
    return m;
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

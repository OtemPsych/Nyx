#pragma once

#include <array>
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <numbers>

#include <nyx/core/traits.h>

namespace nyx {

template <SignedScalar T> [[nodiscard]] constexpr T abs(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T fmod(T x, T y) noexcept;

template <SignedScalar T> [[nodiscard]] constexpr bool signbit(T x) noexcept;
template <SignedScalar T> [[nodiscard]] constexpr T copysign(T mag, T sgn) noexcept;

template <std::floating_point T> [[nodiscard]] constexpr bool isnan(T val) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr bool isinf(T val) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr bool isnormal(T val) noexcept;

template <Scalar T> [[nodiscard]] constexpr T min(T v0, T v1) noexcept;
template <Scalar T> [[nodiscard]] constexpr T max(T v0, T v1) noexcept;
template <Scalar T> [[nodiscard]] constexpr T clamp(T val, T min_val, T max_val) noexcept;
template <std::floating_point T>
[[nodiscard]] constexpr bool approx_equal(T a, T b, std::uint_fast32_t max_ulp_factor = 100) noexcept;

template <std::floating_point T> [[nodiscard]] constexpr T sqrt(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T rsqrt(T x) noexcept;

template <std::floating_point T> [[nodiscard]] constexpr T trunc(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T floor(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T ceil(T x) noexcept;

template <std::floating_point T> [[nodiscard]] constexpr T sin(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T cos(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T tan(T x) noexcept;

template <std::floating_point T> [[nodiscard]] constexpr T asin(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T acos(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T atan(T x) noexcept;

template <std::floating_point T> [[nodiscard]] constexpr T exp(T x) noexcept;
template <std::floating_point T = float> [[nodiscard]] constexpr T exp2(std::integral auto num) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T log(T x) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T pow(T x, T y) noexcept;

namespace detail {

template <std::floating_point T> [[nodiscard]] consteval T fmod_impl(T x, T y) noexcept;

template <SignedScalar T> [[nodiscard]] consteval bool signbit_impl(T x) noexcept;
template <SignedScalar T> [[nodiscard]] consteval T copysign_impl(T mag, T sgn) noexcept;
template <std::floating_point T> [[nodiscard]] consteval bool isinf_impl(T val) noexcept;

template <std::floating_point T> [[nodiscard]] consteval T sqrt_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T rsqrt_impl(T x) noexcept;

template <std::floating_point T> [[nodiscard]] consteval T trunc_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T floor_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T ceil_impl(T x) noexcept;

template <std::floating_point T> [[nodiscard]] consteval T sin_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T cos_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T tan_impl(T x) noexcept;

template <std::floating_point T> [[nodiscard]] consteval T asin_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T acos_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T atan_impl(T x) noexcept;

template <std::floating_point T> [[nodiscard]] consteval T exp_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T exp2_impl(std::integral auto num) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T log_impl(T x) noexcept;
template <std::floating_point T> [[nodiscard]] consteval T pow_impl(T x, T y) noexcept;

template <std::floating_point T> [[nodiscard]] consteval T reduce_pi(T x) noexcept;

} // namespace detail

template <SignedScalar T> constexpr T abs(T x) noexcept {
    if consteval {
        return x < T{0} ? -x : x;
    }
    return std::abs(x);
}

template <std::floating_point T> constexpr T fmod(T x, T y) noexcept {
    if consteval {
        return detail::fmod_impl(x, y);
    }
    return std::fmod(x, y);
}

template <SignedScalar T> constexpr bool signbit(T x) noexcept {
    if consteval {
        return detail::signbit_impl(x);
    }
    return std::signbit(x);
}

template <SignedScalar T> constexpr T copysign(T mag, T sgn) noexcept {
    if consteval {
        return detail::copysign_impl(mag, sgn);
    }
    return std::copysign(mag, sgn);
}

template <std::floating_point T> constexpr bool isnan(T val) noexcept {
    if consteval {
        return val != val;
    }
    return std::isnan(val);
}

template <std::floating_point T> constexpr bool isinf(T val) noexcept {
    if consteval {
        return detail::isinf_impl(val);
    }
    return std::isinf(val);
}

template <std::floating_point T> constexpr bool isnormal(T val) noexcept {
    if consteval {
        return !isnan(val) && !isinf(val) && val != T{0} && abs(val) >= std::numeric_limits<T>::min();
    }
    return std::isnormal(val);
}

template <Scalar T> constexpr T min(T v0, T v1) noexcept {
    if consteval {
        return v0 < v1 ? v0 : v1;
    }
    return std::min(v0, v1);
}

template <Scalar T> constexpr T max(T v0, T v1) noexcept {
    if consteval {
        return v0 > v1 ? v0 : v1;
    }
    return std::max(v0, v1);
}

template <Scalar T> constexpr T clamp(T val, T min_val, T max_val) noexcept {
    if consteval {
        return max(min(val, max_val), min_val);
    }
    return std::clamp(val, min_val, max_val);
}

template <std::floating_point T> constexpr bool approx_equal(T a, T b, std::uint_fast32_t max_ulp_factor) noexcept {
    if (a == b) {
        return true;
    }

    const T diff{abs(a - b)};
    const T norm{max(abs(a), abs(b))};
    return diff < (norm * std::numeric_limits<T>::epsilon() * max_ulp_factor);
}

template <std::floating_point T> constexpr T sqrt(T x) noexcept {
    if consteval {
        return detail::sqrt_impl(x);
    }
    return std::sqrt(x);
}

template <std::floating_point T> constexpr T rsqrt(T x) noexcept {
    if consteval {
        return detail::rsqrt_impl(x);
    }
    return T{1} / std::sqrt(x);
}

template <std::floating_point T> constexpr T trunc(T x) noexcept {
    if consteval {
        return detail::trunc_impl(x);
    }
    return std::trunc(x);
}

template <std::floating_point T> constexpr T floor(T x) noexcept {
    if consteval {
        return detail::floor_impl(x);
    }
    return std::floor(x);
}

template <std::floating_point T> constexpr T ceil(T x) noexcept {
    if consteval {
        return detail::ceil_impl(x);
    }
    return std::ceil(x);
}

template <std::floating_point T> constexpr T sin(T x) noexcept {
    if consteval {
        return detail::sin_impl(x);
    }
    return std::sin(x);
}

template <std::floating_point T> constexpr T cos(T x) noexcept {
    if consteval {
        return detail::cos_impl(x);
    }
    return std::cos(x);
}

template <std::floating_point T> constexpr T tan(T x) noexcept {
    if consteval {
        return detail::tan_impl(x);
    }
    return std::tan(x);
}

template <std::floating_point T> constexpr T asin(T x) noexcept {
    if consteval {
        return detail::asin_impl(x);
    }
    return std::asin(x);
}

template <std::floating_point T> constexpr T acos(T x) noexcept {
    if consteval {
        return detail::acos_impl(x);
    }
    return std::acos(x);
}

template <std::floating_point T> constexpr T atan(T x) noexcept {
    if consteval {
        return detail::atan_impl(x);
    }
    return std::atan(x);
}

template <std::floating_point T> constexpr T exp(T x) noexcept {
    if consteval {
        return detail::exp_impl(x);
    }
    return std::exp(x);
}

template <std::floating_point T> constexpr T exp2(std::integral auto num) noexcept {
    if consteval {
        return detail::exp2_impl<T>(num);
    }
    return std::exp2(num);
}

template <std::floating_point T> constexpr T log(T x) noexcept {
    if consteval {
        return detail::log_impl(x);
    }
    return std::log(x);
}

template <std::floating_point T> constexpr T pow(T x, T y) noexcept {
    if consteval {
        return detail::pow_impl(x, y);
    }
    return std::pow(x, y);
}

namespace detail {

template <std::floating_point T> consteval T fmod_impl(T x, T y) noexcept {
    // IEEE floating-point arithmetic constraints (IEC 60559)
    if (isnan(x) || isinf(x) || isnan(y) || y == T{0}) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    if (x == T{0} || isinf(y)) {
        return x;
    }

    const T abs_x{abs(x)};
    const T abs_y{abs(y)};
    if (abs_x < abs_y) {
        return x;
    }
    return copysign(abs_x - trunc(abs_x / abs_y) * abs_y, x);
}

template <SignedScalar T> consteval bool signbit_impl(T x) noexcept {
    if constexpr (std::integral<T>) {
        return x < T{0};
    }

    using Storage = std::array<std::uint8_t, sizeof(T)>;
    const Storage bytes{std::bit_cast<Storage>(x)};
    constexpr std::uint8_t sign_mask{1 << 7};

    if constexpr (std::endian::native == std::endian::little) {
        return (bytes.back() & sign_mask) != 0;
    }
    return (bytes.front() & sign_mask) != 0;
}

template <SignedScalar T> consteval T copysign_impl(T mag, T sgn) noexcept {
    if constexpr (std::integral<T>) {
        const T abs_mag{abs(mag)};
        return sgn < T{0} ? -abs_mag : abs_mag;
    }

    using Storage = std::array<std::uint8_t, sizeof(T)>;
    Storage mag_bytes{std::bit_cast<Storage>(mag)};
    const Storage sgn_bytes{std::bit_cast<Storage>(sgn)};
    constexpr std::uint8_t sign_mask{1 << 7};

    if constexpr (std::endian::native == std::endian::little) {
        mag_bytes.back() = (mag_bytes.back() & ~sign_mask) | (sgn_bytes.back() & sign_mask);
    } else {
        mag_bytes.front() = (mag_bytes.front() & ~sign_mask) | (sgn_bytes.front() & sign_mask);
    }
    return std::bit_cast<T>(mag_bytes);
}

template <std::floating_point T> consteval bool isinf_impl(T val) noexcept {
    if constexpr (std::numeric_limits<T>::has_infinity) {
        return val == std::numeric_limits<T>::infinity() || val == -std::numeric_limits<T>::infinity();
    }
    return false;
}

template <std::floating_point T> consteval T sqrt_impl(T x) noexcept {
    if (isnan(x) || isinf(x) || x == T{0} || x == T{1}) {
        return x;
    }
    if (x < T{0}) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    T current{x};
    T prev;
    do {
        prev = std::exchange(current, (current + x / current) / T{2});
    } while (current != prev);

    return current;
}

template <std::floating_point T> consteval T rsqrt_impl(T x) noexcept {
    if (x == T{0}) {
        return copysign(std::numeric_limits<T>::infinity(), x);
    }
    if (x < T{0}) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    return T{1} / sqrt(x);
}

template <std::floating_point T> consteval T trunc_impl(T x) noexcept {
    const T abs_x{abs(x)};

    if (abs_x < static_cast<T>(std::numeric_limits<std::int_fast64_t>::max())) {
        return static_cast<T>(static_cast<std::int_fast64_t>(x));
    }
    if (abs_x >= exp2<T>(std::numeric_limits<T>::digits)) {
        return x; // guaranteed to have no fractional bits
    }

    T integer_part{exp2<T>(std::numeric_limits<T>::digits - 1)};
    while (integer_part > abs_x) {
        integer_part /= T{2};
    }

    T accum{0};
    T rem{abs_x};
    while (integer_part >= T{1}) {
        if (rem >= integer_part) {
            accum += integer_part;
            rem -= integer_part;
        }
        integer_part /= T{2};
    }
    return copysign(accum, x);
}

template <std::floating_point T> consteval T floor_impl(T x) noexcept {
    if (isnan(x) ||
        isinf(x) ||
        x <= static_cast<T>(std::numeric_limits<std::int_fast64_t>::min()) ||
        x >= static_cast<T>(std::numeric_limits<std::int_fast64_t>::max())) {
        return x;
    }

    // For negative numbers, if truncation makes the number larger (closer to 0), we subtract 1.0 to find the
    // correct floor. (e.g., -3.5 truncated to -3.0 -> -4.0)
    T truncated{trunc(x)};
    return truncated > x ? truncated - T{1} : truncated;
}

template <std::floating_point T> consteval T ceil_impl(T x) noexcept {
    if (isnan(x) ||
        isinf(x) ||
        x <= static_cast<T>(std::numeric_limits<std::int_fast64_t>::min()) ||
        x >= static_cast<T>(std::numeric_limits<std::int_fast64_t>::max())) {
        return x;
    }

    // For positive numbers, if truncation makes the number smaller, we must add 1.0 to find the correct ceiling
    // (e.g., 3.5 truncated to 3.0 -> 4.0)
    T truncated{trunc(x)};
    return truncated < x ? truncated + T{1} : truncated;
}

template <std::floating_point T> consteval T sin_impl(T x) noexcept {
    if (isnan(x) || isinf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T reduced_x{detail::reduce_pi(x)};
    const T term{-reduced_x * reduced_x};

    T sum{reduced_x};
    T current{reduced_x};
    T n{3};

    while (abs(current) > std::numeric_limits<T>::epsilon()) {
        current *= term / (n * (n - T{1}));
        sum += current;

        n += T{2};
    }
    return sum;
}

template <std::floating_point T> consteval T cos_impl(T x) noexcept {
    if (isnan(x) || isinf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T reduced_x{detail::reduce_pi(x)};
    const T term{-reduced_x * reduced_x};

    T sum{1};
    T current{1};
    T n{2};

    while (abs(current) > std::numeric_limits<T>::epsilon()) {
        current *= term / (n * (n - T{1}));
        sum += current;

        n += T{2};
    }
    return sum;
}

template <std::floating_point T> consteval T tan_impl(T x) noexcept {
    const T c{cos(x)};
    if (c == T{0}) {
        return std::numeric_limits<T>::quiet_NaN(); // undefined at asymptotes
    }
    return sin(x) / c;
}

template <std::floating_point T> consteval T asin_impl(T x) noexcept {
    const T abs_x{abs(x)};
    if (abs_x > T{1}) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    if (abs_x == T{1}) {
        return copysign(std::numbers::pi_v<T> / T{2}, x);
    }
    return atan(x / sqrt(T{1} - x * x));
}

template <std::floating_point T> consteval T acos_impl(T x) noexcept {
    if (abs(x) > T{1}) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    return (std::numbers::pi_v<T> / T{2}) - asin(x);
}

template <std::floating_point T> consteval T atan_impl(T x) noexcept {
    if (isnan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    if (isinf(x)) {
        return copysign(std::numbers::pi_v<T> / T{2}, x);
    }
    if (x == T{0}) {
        return x;
    }
    if (abs(x) > T{1}) {
        return copysign(std::numbers::pi_v<T> / T{2} - atan(T{1} / x), x);
    }

    // Euler's series expansion for rapid convergence
    const T term{x / (T{1} + x * x)};
    const T transform{x * term};

    T sum{term};
    T current{term};
    T n{2};

    while (abs(current) > std::numeric_limits<T>::epsilon()) {
        current *= transform * (n / (n + T{1}));
        sum += current;

        n += T{2};
    }
    return sum;
}

template <std::floating_point T> consteval T exp_impl(T x) noexcept {
    if (isnan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    if (x == std::numeric_limits<T>::infinity()) {
        return x;
    }
    if (x == -std::numeric_limits<T>::infinity()) {
        return T{0};
    }

    T sum{1};
    T current{1};
    T n{1};
    while (current > std::numeric_limits<T>::epsilon()) {
        current *= x / n++;
        sum += current;
    }
    return sum;
}

template <std::floating_point T> consteval T exp2_impl(std::integral auto num) noexcept {
    if (num < 0) {
        return T{1} / exp2<T>(-num);
    }

    T base{2};
    T result{1};
    while (num > 0) {
        if (num & 1) {
            result *= base;
        }
        base *= base;
        num >>= 1;
    }
    return result;
}

template <std::floating_point T> consteval T log_impl(T x) noexcept {
    if (isinf(x)) {
        return x;
    }
    if (isnan(x) || x < T{0}) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    if (x == T{0}) {
        return -std::numeric_limits<T>::infinity();
    }

    // Initial raw range-reduction guess using Euler's constant (e)
    T y{0};
    T tmp{x};
    while (tmp > std::numbers::e_v<T>) {
        tmp /= std::numbers::e_v<T>;
        y += T{1};
    }

    // Halley's Method (super-convergent root finder)
    T dy;
    do {
        const T ey{exp(y)};
        dy = T{2} * (x - ey) / (x + ey);
        y += dy;
    } while (abs(dy) > std::numeric_limits<T>::epsilon());

    return y;
}

template <std::floating_point T> consteval T pow_impl(T x, T y) noexcept {
    if (isnan(x) || isnan(y)) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    if (y == T{0}) { // IEEE 754 standard: x^0 = 1 (even when x := 0)
        return T{1};
    }
    if (x == T{0}) {
        if (y < T{0}) {
            return std::numeric_limits<T>::infinity();
        }
        return T{0};
    }

    if (x > T{0}) {
        return exp(y * log(x));
    }

    if (trunc(y) != y) { // non-integer exponents on negative bases yield complex numbers
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T abs_y{abs(y)};
    T result{exp(y * log(-x))};
    if (abs_y >= exp2<T>(std::numeric_limits<T>::digits)) {
        return result;
    }

    const bool is_even{fmod(abs_y, T{2}) == T{0}};
    return is_even ? result : -result;
}

template <std::floating_point T> consteval T reduce_pi(T x) noexcept {
    if (isnan(x) || isinf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    constexpr T pi{std::numbers::pi_v<T>};
    constexpr T pi2{pi * 2};

    const T abs_x{abs(x)};
    if (abs_x <= pi) {
        return x;
    }

    T reduced{abs_x - trunc(abs_x / pi2) * pi2};
    if (reduced > pi) {
        reduced -= pi2;
    }
    return copysign(reduced, x);
}

} // namespace detail

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

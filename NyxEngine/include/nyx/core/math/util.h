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

template <SignedScalar T> [[nodiscard]] constexpr T abs(T x) noexcept {
    if consteval {
        return x < T{0} ? -x : x;
    } else {
        return std::abs(x);
    }
}

template <SignedScalar T> [[nodiscard]] constexpr bool signbit(T x) noexcept {
    if consteval {
        if constexpr (std::integral<T>) {
            return x < T{0};
        } else {
            using Storage = std::array<std::uint8_t, sizeof(T)>;
            const auto bytes{std::bit_cast<Storage>(x)};
            constexpr std::uint8_t sign_mask{1 << 7};

            if constexpr (std::endian::native == std::endian::little) {
                return (bytes.back() & sign_mask) != 0;
            } else {
                return (bytes.front() & sign_mask) != 0;
            }
        }
    } else {
        return std::signbit(x);
    }
}

template <SignedScalar T> [[nodiscard]] constexpr T copysign(T mag, T sgn) noexcept {
    if consteval {
        if constexpr (std::integral<T>) {
            const T abs_mag{abs(mag)};
            return sgn < T{0} ? -abs_mag : abs_mag;
        } else {
            using Storage = std::array<std::uint8_t, sizeof(T)>;
            auto mag_bytes{std::bit_cast<Storage>(mag)};
            const auto sgn_bytes{std::bit_cast<Storage>(sgn)};
            constexpr std::uint8_t sign_mask{1 << 7};

            if constexpr (std::endian::native == std::endian::little) {
                mag_bytes.back() = (mag_bytes.back() & ~sign_mask) | (sgn_bytes.back() & sign_mask);
            } else {
                mag_bytes.front() = (mag_bytes.front() & ~sign_mask) | (sgn_bytes.front() & sign_mask);
            }
            return std::bit_cast<T>(mag_bytes);
        }
    } else {
        return std::copysign(mag, sgn);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr bool isnan(T val) noexcept {
    if consteval {
        return val != val;
    } else {
        return std::isnan(val);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr bool isinf(T val) noexcept {
    if consteval {
        if constexpr (std::numeric_limits<T>::has_infinity) {
            return val == std::numeric_limits<T>::infinity() || val == -std::numeric_limits<T>::infinity();
        }
        return false;
    } else {
        return std::isinf(val);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr bool isnormal(T val) noexcept {
    if consteval {
        return !isnan(val) && !isinf(val) && val != T{0} && abs(val) >= std::numeric_limits<T>::min();
    } else {
        return std::isnormal(val);
    }
}

template <std::floating_point T>
[[nodiscard]] constexpr bool approx_equal(T a, T b, std::uint_fast32_t max_ulp_factor = 100) noexcept {
    if (a == b) return true;

    const T diff{abs(a - b)};
    const T norm{std::max(abs(a), abs(b))};
    return diff < (norm * std::numeric_limits<T>::epsilon() * max_ulp_factor);
}

template <std::floating_point T> [[nodiscard]] constexpr T radians(T degrees) noexcept {
    return degrees * (std::numbers::pi_v<T> / T{180});
}

template <std::floating_point T> [[nodiscard]] constexpr T degrees(T radians) noexcept {
    return radians * (T{180} / std::numbers::pi_v<T>);
}

template <std::floating_point T> [[nodiscard]] constexpr T sqrt(T x) noexcept {
    if consteval {
        if (x < T{0}) {
            return std::numeric_limits<T>::quiet_NaN();
        }
        if (isnan(x) || isinf(x) || x == T{0} || x == T{1}) {
            return x;
        }

        T current{x};
        T prev{0};
        while (current != prev) {
            prev = current;
            current = T{0.5} * (current + x / current);
        }
        return current;
    } else {
        return std::sqrt(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T rsqrt(T x) noexcept {
    if consteval {
        if (x == T{0}) {
            return copysign(std::numeric_limits<T>::infinity(), x);
        }
        if (x < T{0}) {
            return std::numeric_limits<T>::quiet_NaN();
        }
    }
    return T{1} / sqrt(x);
}

template <std::floating_point T> [[nodiscard]] constexpr T floor(T x) noexcept {
    if consteval {
        if (isnan(x) ||
            isinf(x) ||
            x <= static_cast<T>(std::numeric_limits<std::int_fast64_t>::min()) ||
            x >= static_cast<T>(std::numeric_limits<std::int_fast64_t>::max())) {
            return x;
        }

        // For negative numbers, if truncation makes the number larger (closer to 0), we subtract 1.0 to find the
        // correct floor. (e.g., -3.5 truncated to -3.0 -> -4.0)
        T truncated{static_cast<T>(static_cast<std::int_fast64_t>(x))};
        return truncated > x ? truncated - T{1} : truncated;
    } else {
        return std::floor(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T ceil(T x) noexcept {
    if consteval {
        if (isnan(x) ||
            isinf(x) ||
            x <= static_cast<T>(std::numeric_limits<std::int_fast64_t>::min()) ||
            x >= static_cast<T>(std::numeric_limits<std::int_fast64_t>::max())) {
            return x;
        }

        // For positive numbers, if truncation makes the number smaller, we must add 1.0 to find the correct ceiling
        // (e.g., 3.5 truncated to 3.0 -> 4.0)
        T truncated{static_cast<T>(static_cast<std::int_fast64_t>(x))};
        return truncated < x ? truncated + T{1} : truncated;
    } else {
        return std::ceil(x);
    }
}

namespace detail {

template <std::floating_point T> [[nodiscard]] consteval T fmod_pi2(T x) noexcept {
    if (isnan(x) || isinf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    constexpr T pi{std::numbers::pi_v<T>};
    constexpr T pi2{pi * T{2}};

    const T abs_x{abs(x)};
    if (abs_x <= pi) {
        return x;
    }
    if ((abs_x / pi2) >= static_cast<T>(std::numeric_limits<std::int_fast64_t>::max())) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const auto quotient{static_cast<std::int_fast64_t>(abs_x / pi2)};
    T reduced{abs_x - static_cast<T>(quotient) * pi2};
    if (reduced > pi) {
        reduced -= pi2;
    }
    return x < T{0} ? -reduced : reduced;
}

} // namespace detail

template <std::floating_point T> [[nodiscard]] constexpr T sin(T x) noexcept {
    if consteval {
        if (isnan(x) || isinf(x)) {
            return std::numeric_limits<T>::quiet_NaN();
        }

        const T norm_x{detail::fmod_pi2(x)};
        const T xx{norm_x * norm_x};
        T sum{norm_x};
        T term{norm_x};
        for (std::int_fast32_t i{3}; i < 24; i += 2) {
            term *= -xx / static_cast<T>(i * (i - 1));
            sum += term;
        }
        return sum;
    } else {
        return std::sin(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T cos(T x) noexcept {
    if consteval {
        if (isnan(x) || isinf(x)) {
            return std::numeric_limits<T>::quiet_NaN();
        }

        const T norm_x{detail::fmod_pi2(x)};
        const T xx{norm_x * norm_x};
        T sum{1};
        T term{1};
        for (std::int_fast32_t i{2}; i < 24; i += 2) {
            term *= -xx / static_cast<T>(i * (i - 1));
            sum += term;
        }
        return static_cast<T>(sum);
    } else {
        return std::cos(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T tan(T x) noexcept {
    if consteval {
        const T c{cos(x)};
        if (c == T{0}) {
            return std::numeric_limits<T>::quiet_NaN(); // undefined at asymptotes
        }
        return sin(x) / c;
    } else {
        return std::tan(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T atan(T x) noexcept {
    if consteval {
        if (x == T{0}) return x;
        if (isnan(x)) return std::numeric_limits<T>::quiet_NaN();
        if (isinf(x)) return copysign(std::numbers::pi_v<T> / T{2}, x);
        if (abs(x) > T{1}) return copysign(std::numbers::pi_v<T> / T{2} - atan(T{1} / x), x);

        // Euler's series expansion for rapid convergence
        const T term{x / (T{1} + x * x)};
        const T transform{x * term};

        T sum{term};
        T current{term};
        T num{2};
        T den{3};

        while (abs(current) > std::numeric_limits<T>::epsilon()) {
            current *= (num / den) * transform;
            sum += current;

            num += T{2};
            den += T{2};
        }
        return sum;
    } else {
        return std::atan(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T asin(T x) noexcept {
    if consteval {
        if (x < T{-1} || x > T{1}) {
            return std::numeric_limits<T>::quiet_NaN();
        }
        if (abs(x) == T{1}) {
            return copysign(std::numbers::pi_v<T> / 2, x);
        }
        return atan(x / sqrt(T{1} - x * x));
    } else {
        return std::asin(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T acos(T x) noexcept {
    if consteval {
        if (x < T{-1} || x > T{1}) {
            return std::numeric_limits<T>::quiet_NaN();
        }
        return (std::numbers::pi_v<T> / 2) - asin(x);
    } else {
        return std::acos(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T exp(T x) noexcept {
    if consteval {
        if (isnan(x)) return std::numeric_limits<T>::quiet_NaN();
        if (x == std::numeric_limits<T>::infinity()) return x;
        if (x == -std::numeric_limits<T>::infinity()) return T{0};

        T sum{1};
        T term{1};
        for (std::int_fast32_t i{1}; i < 35; ++i) {
            term *= x / static_cast<T>(i);
            sum += term;
        }
        return sum;
    } else {
        return std::exp(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T log(T x) noexcept {
    if consteval {
        if (isnan(x) || x <= T{0}) return std::numeric_limits<T>::quiet_NaN();
        if (x == T{0}) return -std::numeric_limits<T>::infinity();
        if (x == std::numeric_limits<T>::infinity()) return x;

        // Initial raw range-reduction guess using Euler's constant (e)
        T y{0};
        if (x > T{1}) {
            T temp{x};
            while (temp > std::numbers::e_v<T>) {
                temp /= std::numbers::e_v<T>;
                y += T{1};
            }
        }

        // Halley's Method (super-convergent root finder)
        for (std::int_fast32_t i{0}; i < 12; ++i) {
            const T ey{exp(y)};
            const T dy{static_cast<T>(2) * (x - ey) / (x + ey)};
            y += dy;
        }
        return y;
    } else {
        return std::log(x);
    }
}

template <std::floating_point T> [[nodiscard]] constexpr T pow(T x, T y) noexcept {
    if consteval {
        if (y == T{0}) return T{1};
        if (x == T{0}) return y > T{0} ? T{0} : std::numeric_limits<T>::infinity();

        if (x < T{0}) {
            // Negative bases can only be raised to integer powers using `x^2 = e^(yln(x))`
            const auto y_floor{static_cast<std::int_fast64_t>(y)};
            if (y != static_cast<T>(y_floor)) {
                return std::numeric_limits<T>::quiet_NaN();
            }
            T abs_pow{exp(y * log(-x))};
            return (y_floor % 2 == 0) ? abs_pow : -abs_pow;
        }
        return exp(y * log(x));
    } else {
        return std::pow(x, y);
    }
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

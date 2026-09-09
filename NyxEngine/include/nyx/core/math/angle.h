#pragma once

#include <cassert>
#include <concepts>
#include <format>
#include <numbers>

#include <nyx/core/math/util.h>

namespace nyx {

template <std::floating_point T = double> class Angle {
  public:
    [[nodiscard]] static constexpr Angle radians(T rad) noexcept;
    [[nodiscard]] static constexpr Angle degrees(T deg) noexcept;

    [[nodiscard]] static constexpr Angle asin(T rad) noexcept;
    [[nodiscard]] static constexpr Angle acos(T rad) noexcept;
    [[nodiscard]] static constexpr Angle atan(T rad) noexcept;
    [[nodiscard]] static constexpr Angle atan2(T y, T x) noexcept;

    [[nodiscard]] static constexpr Angle delta(Angle from, Angle to) noexcept;
    [[nodiscard]] static constexpr Angle distance(Angle a, Angle b) noexcept;
    [[nodiscard]] static constexpr Angle lerp(Angle a, Angle b, T t) noexcept;

    constexpr Angle() noexcept = default;

    [[nodiscard]] constexpr Angle operator+() const noexcept;
    [[nodiscard]] constexpr Angle operator-() const noexcept;

    constexpr Angle& operator+=(Angle rhs) noexcept;
    constexpr Angle& operator-=(Angle rhs) noexcept;
    constexpr Angle& operator*=(T factor) noexcept;
    constexpr Angle& operator/=(T factor) noexcept;
    constexpr Angle& operator%=(Angle rhs) noexcept;

    [[nodiscard]] friend constexpr auto operator<=>(Angle, Angle) noexcept = default;

    [[nodiscard]] constexpr T radians() const noexcept;
    [[nodiscard]] constexpr T degrees() const noexcept;

    [[nodiscard]] constexpr T sin() const noexcept;
    [[nodiscard]] constexpr T cos() const noexcept;
    [[nodiscard]] constexpr T tan() const noexcept;

    [[nodiscard]] constexpr Angle wrap_pi() const noexcept;
    [[nodiscard]] constexpr Angle wrap_tau() const noexcept;
  private:
    constexpr explicit Angle(T rad) noexcept;

  private:
    T rad_{};
};

template <std::floating_point T> [[nodiscard]] constexpr Angle<T> operator+(Angle<T> lhs, Angle<T> rhs) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr Angle<T> operator-(Angle<T> lhs, Angle<T> rhs) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr Angle<T> operator*(Angle<T> angle, T factor) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr Angle<T> operator*(T factor, Angle<T> angle) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr Angle<T> operator/(Angle<T> angle, T factor) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr T operator/(Angle<T> lhs, Angle<T> rhs) noexcept;
template <std::floating_point T> [[nodiscard]] constexpr Angle<T> operator%(Angle<T> lhs, Angle<T> rhs) noexcept;

namespace literals {

[[nodiscard]] constexpr Angle<> operator""_deg(long double deg) noexcept;
[[nodiscard]] constexpr Angle<> operator""_deg(unsigned long long deg) noexcept;
[[nodiscard]] constexpr Angle<> operator""_rad(long double rad) noexcept;
[[nodiscard]] constexpr Angle<> operator""_rad(unsigned long long rad) noexcept;

} // namespace literals

template <std::floating_point T> constexpr Angle<T> Angle<T>::radians(T rad) noexcept { return Angle{rad}; }

template <std::floating_point T> constexpr Angle<T> Angle<T>::degrees(T deg) noexcept {
    return Angle{deg * (std::numbers::pi_v<T> / T{180})};
}

template <std::floating_point T> constexpr Angle<T> Angle<T>::asin(T rad) noexcept { return Angle{nyx::asin(rad)}; }

template <std::floating_point T> constexpr Angle<T> Angle<T>::acos(T rad) noexcept { return Angle{nyx::acos(rad)}; }

template <std::floating_point T> constexpr Angle<T> Angle<T>::atan(T rad) noexcept { return Angle{nyx::atan(rad)}; }

template <std::floating_point T> constexpr Angle<T> Angle<T>::atan2(T y, T x) noexcept {
    return Angle{nyx::atan2(y, x)};
}

template <std::floating_point T> constexpr Angle<T> Angle<T>::delta(Angle from, Angle to) noexcept {
    return (to - from).wrap_pi();
}

template <std::floating_point T> constexpr Angle<T> Angle<T>::distance(Angle a, Angle b) noexcept {
    return Angle{abs(delta(a, b).rad_)};
}

template <std::floating_point T> constexpr Angle<T> Angle<T>::lerp(Angle a, Angle b, T t) noexcept {
    return a + delta(a, b) * t;
}

template <std::floating_point T> constexpr Angle<T> Angle<T>::operator+() const noexcept { return *this; }

template <std::floating_point T> constexpr Angle<T> Angle<T>::operator-() const noexcept { return Angle{-rad_}; }

template <std::floating_point T> constexpr Angle<T>& Angle<T>::operator+=(Angle rhs) noexcept {
    rad_ += rhs.rad_;
    return *this;
}

template <std::floating_point T> constexpr Angle<T>& Angle<T>::operator-=(Angle rhs) noexcept {
    rad_ -= rhs.rad_;
    return *this;
}

template <std::floating_point T> constexpr Angle<T>& Angle<T>::operator*=(T factor) noexcept {
    rad_ *= factor;
    return *this;
}

template <std::floating_point T> constexpr Angle<T>& Angle<T>::operator/=(T factor) noexcept {
    assert(factor != T{0} && "Division by zero");
    rad_ /= factor;
    return *this;
}

template <std::floating_point T> constexpr Angle<T>& Angle<T>::operator%=(Angle rhs) noexcept {
    assert(rhs.rad_ != T{0} && "Modulo by zero");
    rad_ = fmod(rad_, rhs.rad_);
    return *this;
}

template <std::floating_point T> constexpr T Angle<T>::radians() const noexcept { return rad_; }

template <std::floating_point T> constexpr T Angle<T>::degrees() const noexcept {
    return rad_ * (T{180} / std::numbers::pi_v<T>);
}

template <std::floating_point T> constexpr T Angle<T>::sin() const noexcept { return nyx::sin(rad_); }

template <std::floating_point T> constexpr T Angle<T>::cos() const noexcept { return nyx::cos(rad_); }

template <std::floating_point T> constexpr T Angle<T>::tan() const noexcept { return nyx::tan(rad_); }

template <std::floating_point T> constexpr Angle<T> Angle<T>::wrap_pi() const noexcept {
    constexpr T pi{std::numbers::pi_v<T>};
    constexpr T pi2{T{2} * pi};

    T rad{fmod(rad_ + pi, pi2)};
    if (rad < T{0}) {
        rad += pi2;
    }
    return Angle<T>{rad - pi};
}

template <std::floating_point T> constexpr Angle<T> Angle<T>::wrap_tau() const noexcept {
    constexpr T pi2{T{2} * std::numbers::pi_v<T>};

    T rad{fmod(rad_, pi2)};
    if (rad < T{0}) {
        rad += pi2;
    }
    return Angle{rad};
}

template <std::floating_point T> constexpr Angle<T>::Angle(T rad) noexcept : rad_{rad} {}

template <std::floating_point T> constexpr Angle<T> operator+(Angle<T> lhs, Angle<T> rhs) noexcept {
    lhs += rhs;
    return lhs;
}

template <std::floating_point T> constexpr Angle<T> operator-(Angle<T> lhs, Angle<T> rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

template <std::floating_point T> constexpr Angle<T> operator*(Angle<T> angle, T factor) noexcept {
    angle *= factor;
    return angle;
}

template <std::floating_point T> constexpr Angle<T> operator*(T factor, Angle<T> angle) noexcept {
    angle *= factor;
    return angle;
}

template <std::floating_point T> constexpr Angle<T> operator/(Angle<T> angle, T factor) noexcept {
    assert(factor != T{0} && "Division by zero");
    angle /= factor;
    return angle;
}

template <std::floating_point T> constexpr T operator/(Angle<T> lhs, Angle<T> rhs) noexcept {
    assert(rhs.radians() != T{0} && "Division by zero");
    return lhs.radians() / rhs.radians();
}

template <std::floating_point T> constexpr Angle<T> operator%(Angle<T> lhs, Angle<T> rhs) noexcept {
    assert(rhs.radians() != T{0} && "Modulo by zero");
    lhs %= rhs;
    return lhs;
}

namespace literals {

constexpr Angle<> operator""_deg(long double deg) noexcept { return Angle<>::degrees(static_cast<double>(deg)); }

constexpr Angle<> operator""_deg(unsigned long long deg) noexcept { return Angle<>::degrees(static_cast<double>(deg)); }

constexpr Angle<> operator""_rad(long double rad) noexcept { return Angle<>::radians(static_cast<double>(rad)); }

constexpr Angle<> operator""_rad(unsigned long long rad) noexcept { return Angle<>::radians(static_cast<double>(rad)); }

} // namespace literals

} // namespace nyx

template <std::floating_point T> struct std::formatter<nyx::Angle<T>> : std::formatter<T> {
    auto format(nyx::Angle<T> angle, std::format_context& ctx) const {
        ctx.advance_to(std::formatter<T>::format(angle.degrees(), ctx));
        return std::format_to(ctx.out(), "\xf8");
    }
};

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

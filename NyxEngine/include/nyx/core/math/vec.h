#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <utility>

#include <nyx/core/math/angle.h>
#include <nyx/core/math/util.h>

namespace nyx {

namespace traits {

template <typename T, std::size_t N> concept PlanarVec = SignedScalar<T> && N == 2;
template <typename T, std::size_t N> concept SpatialVec = SignedScalar<T> && N == 3;
template <typename T, std::size_t N> concept PhysicalVec = SignedScalar<T> && (N == 2 || N == 3);

} // namespace traits

namespace detail {

template <traits::Scalar T, std::size_t N> requires (N >= 2) struct vec_storage {
    std::array<T, N> coords{};

    constexpr vec_storage() noexcept = default;
    constexpr explicit vec_storage(T scalar) noexcept;
    template <std::same_as<T>... Args> requires (sizeof...(Args) == N) constexpr vec_storage(Args... args) noexcept;
    template <typename U, std::size_t M> constexpr vec_storage(const vec_storage<U, M>& v) noexcept;

    [[nodiscard]] friend constexpr auto operator<=>(const vec_storage&, const vec_storage&) noexcept = default;
    template <typename Self> [[nodiscard]] constexpr auto&& operator[](this Self&& self, std::size_t i) noexcept;
};

template <traits::Scalar T> struct vec_storage<T, 2> {
    T x{0}, y{0};

    constexpr vec_storage() noexcept = default;
    constexpr explicit vec_storage(T scalar) noexcept;
    constexpr vec_storage(T x, T y) noexcept;
    template <typename U, std::size_t M> constexpr vec_storage(const vec_storage<U, M>& v) noexcept;

    [[nodiscard]] friend constexpr auto operator<=>(const vec_storage&, const vec_storage&) noexcept = default;
    template <typename Self> [[nodiscard]] constexpr auto&& operator[](this Self&& self, std::size_t i) noexcept;
};

template <traits::Scalar T> struct vec_storage<T, 3> {
    T x{0}, y{0}, z{0};

    constexpr vec_storage() noexcept = default;
    constexpr explicit vec_storage(T scalar) noexcept;
    constexpr vec_storage(T x, T y, T z) noexcept;
    template <typename U, std::size_t M> constexpr vec_storage(const vec_storage<U, M>& v) noexcept;
    template <typename U> constexpr vec_storage(const vec_storage<U, 2>& v, T z) noexcept;

    [[nodiscard]] friend constexpr auto operator<=>(const vec_storage&, const vec_storage&) noexcept = default;
    template <typename Self> [[nodiscard]] constexpr auto&& operator[](this Self&& self, std::size_t i) noexcept;
};

template <traits::Scalar T> struct vec_storage<T, 4> {
    T x{0}, y{0}, z{0}, w{0};

    constexpr vec_storage() noexcept = default;
    constexpr explicit vec_storage(T scalar) noexcept;
    constexpr vec_storage(T x, T y, T z, T w) noexcept;
    template <typename U, std::size_t M> constexpr vec_storage(const vec_storage<U, M>& v) noexcept;
    template <typename U> constexpr vec_storage(const vec_storage<U, 2>& v, T z, T w) noexcept;
    template <typename U> constexpr vec_storage(const vec_storage<U, 3>& v, T w) noexcept;

    [[nodiscard]] friend constexpr auto operator<=>(const vec_storage&, const vec_storage&) noexcept = default;
    template <typename Self> [[nodiscard]] constexpr auto&& operator[](this Self&& self, std::size_t i) noexcept;
};

} // namespace detail

template <traits::Scalar T, std::size_t N> struct vec : public detail::vec_storage<T, N> {
    [[nodiscard]] static constexpr vec up() noexcept requires traits::PhysicalVec<T, N>;
    [[nodiscard]] static constexpr vec down() noexcept requires traits::PhysicalVec<T, N>;
    [[nodiscard]] static constexpr vec left() noexcept requires traits::PhysicalVec<T, N>;
    [[nodiscard]] static constexpr vec right() noexcept requires traits::PhysicalVec<T, N>;
    [[nodiscard]] static constexpr vec forward() noexcept requires traits::SpatialVec<T, N>;
    [[nodiscard]] static constexpr vec back() noexcept requires traits::SpatialVec<T, N>;

    [[nodiscard]] static constexpr vec x_axis() noexcept requires traits::PhysicalVec<T, N>;
    [[nodiscard]] static constexpr vec y_axis() noexcept requires traits::PhysicalVec<T, N>;
    [[nodiscard]] static constexpr vec z_axis() noexcept requires traits::SpatialVec<T, N>;

    [[nodiscard]] static constexpr vec normal(const vec& v0, const vec& v1) noexcept requires traits::PlanarVec<T, N>;
    [[nodiscard]] static constexpr vec cross(const vec& v0, const vec& v1) noexcept requires traits::SpatialVec<T, N>;
    [[nodiscard]] static constexpr T dot(const vec& v0, const vec& v1) noexcept;

    [[nodiscard]] static constexpr vec min(const vec& v0, const vec& v1) noexcept;
    [[nodiscard]] static constexpr vec max(const vec& v0, const vec& v1) noexcept;
    [[nodiscard]] static constexpr vec clamp(const vec& v, const vec& min_v, const vec& max_v) noexcept;
    [[nodiscard]] static constexpr vec clamp(const vec& v, T min_val, T max_val) noexcept;
    [[nodiscard]] static constexpr T distance(const vec& start, const vec& end) noexcept;
    [[nodiscard]] static constexpr auto angle(const vec& v0, const vec& v1) noexcept requires std::floating_point<T>;

    [[nodiscard]] static constexpr vec reflect(const vec& incident, const vec& normal) noexcept
        requires std::floating_point<T>;
    [[nodiscard]] static constexpr vec refract(const vec& incident, const vec& normal, T eta) noexcept
        requires std::floating_point<T>;

    [[nodiscard]] static constexpr vec lerp(const vec& v0, const vec& v1, T t) noexcept requires std::floating_point<T>;
    [[nodiscard]] static constexpr vec smoothstep(const vec& edge0, const vec& edge1, T t) noexcept
        requires std::floating_point<T>;

    using detail::vec_storage<T, N>::vec_storage;

    [[nodiscard]] constexpr vec operator+() const noexcept;
    [[nodiscard]] constexpr vec operator-(this vec self) noexcept requires traits::SignedScalar<T>;

    constexpr vec& operator+=(this vec& lhs, const vec& rhs) noexcept;
    constexpr vec& operator-=(this vec& lhs, const vec& rhs) noexcept;
    constexpr vec& operator*=(this vec& lhs, const vec& rhs) noexcept;
    constexpr vec& operator*=(this vec& v, T scalar) noexcept;
    constexpr vec& operator/=(this vec& lhs, const vec& rhs) noexcept;
    constexpr vec& operator/=(this vec& v, T scalar) noexcept;

    [[nodiscard]] constexpr vec operator+(this vec lhs, const vec& rhs) noexcept;
    [[nodiscard]] constexpr vec operator-(this vec lhs, const vec& rhs) noexcept;
    [[nodiscard]] constexpr vec operator*(this vec lhs, const vec& rhs) noexcept;
    [[nodiscard]] constexpr vec operator*(this vec v, T scalar) noexcept;
    [[nodiscard]] constexpr vec operator/(this vec lhs, const vec& rhs) noexcept;
    [[nodiscard]] constexpr vec operator/(this vec v, T scalar) noexcept;

    [[nodiscard]] constexpr T length_sq() const noexcept;
    [[nodiscard]] constexpr T length() const noexcept;
    [[nodiscard]] constexpr vec norm(this vec self) noexcept requires std::floating_point<T>;
};

template <typename T, std::size_t N> [[nodiscard]] constexpr vec<T, N> operator*(T scalar, vec<T, N> v) noexcept;

// Type Aliases (vec2, vec3, vec4)
template <typename T> using vec2 = vec<T, 2>;
using vec2f = vec2<float>;
using vec2d = vec2<double>;
using vec2ld = vec2<long double>;
using vec2i = vec2<int>;
using vec2l = vec2<long>;
using vec2ll = vec2<long long>;
using vec2u = vec2<unsigned int>;
using vec2ul = vec2<unsigned long>;
using vec2ull = vec2<unsigned long long>;

using vec2i8 = vec2<std::int8_t>;
using vec2i16 = vec2<std::int16_t>;
using vec2i32 = vec2<std::int32_t>;
using vec2i64 = vec2<std::int64_t>;
using vec2ifast8 = vec2<std::int_fast8_t>;
using vec2ifast16 = vec2<std::int_fast16_t>;
using vec2ifast32 = vec2<std::int_fast32_t>;
using vec2ifast64 = vec2<std::int_fast64_t>;
using vec2ileast8 = vec2<std::int_least8_t>;
using vec2ileast16 = vec2<std::int_least16_t>;
using vec2ileast32 = vec2<std::int_least32_t>;
using vec2ileast64 = vec2<std::int_least64_t>;

using vec2u8 = vec2<std::uint8_t>;
using vec2u16 = vec2<std::uint16_t>;
using vec2u32 = vec2<std::uint32_t>;
using vec2u64 = vec2<std::uint64_t>;
using vec2ufast8 = vec2<std::uint_fast8_t>;
using vec2ufast16 = vec2<std::uint_fast16_t>;
using vec2ufast32 = vec2<std::uint_fast32_t>;
using vec2ufast64 = vec2<std::uint_fast64_t>;
using vec2uleast8 = vec2<std::uint_least8_t>;
using vec2uleast16 = vec2<std::uint_least16_t>;
using vec2uleast32 = vec2<std::uint_least32_t>;
using vec2uleast64 = vec2<std::uint_least64_t>;

template <typename T> using vec3 = vec<T, 3>;
using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3ld = vec3<long double>;
using vec3i = vec3<int>;
using vec3l = vec3<long>;
using vec3ll = vec3<long long>;
using vec3u = vec3<unsigned int>;
using vec3ul = vec3<unsigned long>;
using vec3ull = vec3<unsigned long long>;

using vec3i8 = vec3<std::int8_t>;
using vec3i16 = vec3<std::int16_t>;
using vec3i32 = vec3<std::int32_t>;
using vec3i64 = vec3<std::int64_t>;
using vec3ifast8 = vec3<std::int_fast8_t>;
using vec3ifast16 = vec3<std::int_fast16_t>;
using vec3ifast32 = vec3<std::int_fast32_t>;
using vec3ifast64 = vec3<std::int_fast64_t>;
using vec3ileast8 = vec3<std::int_least8_t>;
using vec3ileast16 = vec3<std::int_least16_t>;
using vec3ileast32 = vec3<std::int_least32_t>;
using vec3ileast64 = vec3<std::int_least64_t>;

using vec3u8 = vec3<std::uint8_t>;
using vec3u16 = vec3<std::uint16_t>;
using vec3u32 = vec3<std::uint32_t>;
using vec3u64 = vec3<std::uint64_t>;
using vec3ufast8 = vec3<std::uint_fast8_t>;
using vec3ufast16 = vec3<std::uint_fast16_t>;
using vec3ufast32 = vec3<std::uint_fast32_t>;
using vec3ufast64 = vec3<std::uint_fast64_t>;
using vec3uleast8 = vec3<std::uint_least8_t>;
using vec3uleast16 = vec3<std::uint_least16_t>;
using vec3uleast32 = vec3<std::uint_least32_t>;
using vec3uleast64 = vec3<std::uint_least64_t>;

template <typename T> using vec4 = vec<T, 4>;
using vec4f = vec4<float>;
using vec4d = vec4<double>;
using vec4ld = vec4<long double>;
using vec4i = vec4<int>;
using vec4l = vec4<long>;
using vec4ll = vec4<long long>;
using vec4u = vec4<unsigned int>;
using vec4ul = vec4<unsigned long>;
using vec4ull = vec4<unsigned long long>;

using vec4i8 = vec4<std::int8_t>;
using vec4i16 = vec4<std::int16_t>;
using vec4i32 = vec4<std::int32_t>;
using vec4i64 = vec4<std::int64_t>;
using vec4ifast8 = vec4<std::int_fast8_t>;
using vec4ifast16 = vec4<std::int_fast16_t>;
using vec4ifast32 = vec4<std::int_fast32_t>;
using vec4ifast64 = vec4<std::int_fast64_t>;
using vec4ileast8 = vec4<std::int_least8_t>;
using vec4ileast16 = vec4<std::int_least16_t>;
using vec4ileast32 = vec4<std::int_least32_t>;
using vec4ileast64 = vec4<std::int_least64_t>;

using vec4u8 = vec4<std::uint8_t>;
using vec4u16 = vec4<std::uint16_t>;
using vec4u32 = vec4<std::uint32_t>;
using vec4u64 = vec4<std::uint64_t>;
using vec4ufast8 = vec4<std::uint_fast8_t>;
using vec4ufast16 = vec4<std::uint_fast16_t>;
using vec4ufast32 = vec4<std::uint_fast32_t>;
using vec4ufast64 = vec4<std::uint_fast64_t>;
using vec4uleast8 = vec4<std::uint_least8_t>;
using vec4uleast16 = vec4<std::uint_least16_t>;
using vec4uleast32 = vec4<std::uint_least32_t>;
using vec4uleast64 = vec4<std::uint_least64_t>;

namespace detail {

template <traits::Scalar T, std::size_t N> requires (N >= 2)
constexpr vec_storage<T, N>::vec_storage(T scalar) noexcept {
    coords.fill(scalar);
}

template <traits::Scalar T, std::size_t N> requires (N >= 2) template <std::same_as<T>... Args>
requires (sizeof...(Args) == N) constexpr vec_storage<T, N>::vec_storage(Args... args) noexcept : coords{args...} {}

template <traits::Scalar T, std::size_t N> requires (N >= 2) template <typename U, std::size_t M>
constexpr vec_storage<T, N>::vec_storage(const vec_storage<U, M>& v) noexcept {
    constexpr std::size_t min_count{min(N, M)};
    for (std::size_t i{0}; i < min_count; ++i) {
        coords[i] = static_cast<T>(v[i]);
    }
}

template <traits::Scalar T, std::size_t N> requires (N >= 2) template <typename Self>
constexpr auto&& vec_storage<T, N>::operator[](this Self&& self, std::size_t i) noexcept {
    assert(i < N && "Index out of bounds");
    return std::forward<Self>(self).coords[i];
}

template <traits::Scalar T> constexpr vec_storage<T, 2>::vec_storage(T scalar) noexcept : x{scalar}, y{scalar} {}

template <traits::Scalar T> constexpr vec_storage<T, 2>::vec_storage(T x, T y) noexcept : x{x}, y{y} {}

template <traits::Scalar T>
template <typename U, std::size_t M>
constexpr vec_storage<T, 2>::vec_storage(const vec_storage<U, M>& v) noexcept
    : x{static_cast<T>(v[0])}, y{static_cast<T>(v[1])} {}

template <traits::Scalar T>
template <typename Self>
constexpr auto&& vec_storage<T, 2>::operator[](this Self&& self, std::size_t i) noexcept {
    assert(i < 2 && "Index out of bounds");
    switch (i) {
        case 0:  return std::forward<Self>(self).x;
        case 1:  [[fallthrough]];
        default: return std::forward<Self>(self).y;
    }
}

template <traits::Scalar T>
constexpr vec_storage<T, 3>::vec_storage(T scalar) noexcept : x{scalar}, y{scalar}, z{scalar} {}

template <traits::Scalar T> constexpr vec_storage<T, 3>::vec_storage(T x, T y, T z) noexcept : x{x}, y{y}, z{z} {}

template <traits::Scalar T>
template <typename U, std::size_t M>
constexpr vec_storage<T, 3>::vec_storage(const vec_storage<U, M>& v) noexcept
    : x{static_cast<T>(v[0])}, y{static_cast<T>(v[1])}, z{M > 2 ? static_cast<T>(v[2]) : T{0}} {}

template <traits::Scalar T>
template <typename U>
constexpr vec_storage<T, 3>::vec_storage(const vec_storage<U, 2>& v, T z) noexcept
    : x{static_cast<T>(v.x)}, y{static_cast<T>(v.y)}, z{z} {}

template <traits::Scalar T>
template <typename Self>
constexpr auto&& vec_storage<T, 3>::operator[](this Self&& self, std::size_t i) noexcept {
    assert(i < 3 && "Index out of bounds");
    switch (i) {
        case 0:  return std::forward<Self>(self).x;
        case 1:  return std::forward<Self>(self).y;
        case 2:  [[fallthrough]];
        default: return std::forward<Self>(self).z;
    }
}

template <traits::Scalar T>
constexpr vec_storage<T, 4>::vec_storage(T scalar) noexcept : x{scalar}, y{scalar}, z{scalar}, w{scalar} {}

template <traits::Scalar T>
constexpr vec_storage<T, 4>::vec_storage(T x, T y, T z, T w) noexcept : x{x}, y{y}, z{z}, w{w} {}

template <traits::Scalar T>
template <typename U, std::size_t M>
constexpr vec_storage<T, 4>::vec_storage(const vec_storage<U, M>& v) noexcept
    : x{static_cast<T>(v[0])}
    , y{static_cast<T>(v[1])}
    , z{M > 2 ? static_cast<T>(v[2]) : T{0}}
    , w{M > 3 ? static_cast<T>(v[3]) : T{0}} {}

template <traits::Scalar T>
template <typename U>
constexpr vec_storage<T, 4>::vec_storage(const vec_storage<U, 2>& v, T z, T w) noexcept
    : x{static_cast<T>(v.x)}, y{static_cast<T>(v.y)}, z{z}, w{w} {}

template <traits::Scalar T>
template <typename U>
constexpr vec_storage<T, 4>::vec_storage(const vec_storage<U, 3>& v, T w) noexcept
    : x{static_cast<T>(v.x)}, y{static_cast<T>(v.y)}, z{static_cast<T>(v.z)}, w{w} {}

template <traits::Scalar T>
template <typename Self>
constexpr auto&& vec_storage<T, 4>::operator[](this Self&& self, std::size_t i) noexcept {
    assert(i < 4 && "Index out of bounds");
    switch (i) {
        case 0:  return std::forward<Self>(self).x;
        case 1:  return std::forward<Self>(self).y;
        case 2:  return std::forward<Self>(self).z;
        case 3:  [[fallthrough]];
        default: return std::forward<Self>(self).w;
    }
}

} // namespace detail

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::up() noexcept requires traits::PhysicalVec<T, N> {
    if constexpr (N == 2) {
        return {T{0}, T{-1}};
    } else {
        return {T{0}, T{1}, T{0}};
    }
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::down() noexcept requires traits::PhysicalVec<T, N> {
    if constexpr (N == 2) {
        return {T{0}, T{1}};
    } else {
        return {T{0}, T{-1}, T{0}};
    }
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::left() noexcept requires traits::PhysicalVec<T, N> {
    if constexpr (N == 2) {
        return {T{-1}, T{0}};
    } else {
        return {T{-1}, T{0}, T{0}};
    }
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::right() noexcept requires traits::PhysicalVec<T, N> {
    if constexpr (N == 2) {
        return {T{1}, T{0}};
    } else {
        return {T{1}, T{0}, T{0}};
    }
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::forward() noexcept requires traits::SpatialVec<T, N> {
    return {T{0}, T{0}, T{-1}};
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::back() noexcept requires traits::SpatialVec<T, N> {
    return {T{0}, T{0}, T{1}};
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::x_axis() noexcept requires traits::PhysicalVec<T, N> {
    if constexpr (N == 2) {
        return {T{1}, T{0}};
    } else {
        return {T{1}, T{0}, T{0}};
    }
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::y_axis() noexcept requires traits::PhysicalVec<T, N> {
    if constexpr (N == 2) {
        return {T{0}, T{1}};
    } else {
        return {T{0}, T{1}, T{0}};
    }
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::z_axis() noexcept requires traits::SpatialVec<T, N> {
    return {T{0}, T{0}, T{1}};
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::normal(const vec& v0, const vec& v1) noexcept requires traits::PlanarVec<T, N> {
    return vec{v0.y - v1.y, v1.x - v0.x}.norm();
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::cross(const vec& v0, const vec& v1) noexcept requires traits::SpatialVec<T, N> {
    return {v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x};
}

template <traits::Scalar T, std::size_t N> constexpr T vec<T, N>::dot(const vec& v0, const vec& v1) noexcept {
    T sum{0};
    for (std::size_t i{0}; i < N; ++i) {
        sum += v0[i] * v1[i];
    }
    return sum;
}

template <traits::Scalar T, std::size_t N> constexpr vec<T, N> vec<T, N>::min(const vec& v0, const vec& v1) noexcept {
    vec res{};
    for (std::size_t i{0}; i < N; ++i) {
        res[i] = min(v0[i], v1[i]);
    }
    return res;
}

template <traits::Scalar T, std::size_t N> constexpr vec<T, N> vec<T, N>::max(const vec& v0, const vec& v1) noexcept {
    vec res{};
    for (std::size_t i{0}; i < N; ++i) {
        res[i] = max(v0[i], v1[i]);
    }
    return res;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::clamp(const vec& v, const vec& min_v, const vec& max_v) noexcept {
    vec res{};
    for (std::size_t i{0}; i < N; ++i) {
        assert(min_v[i] <= max_v[i] && "min_v coordinate cannot be greater than max_v coordinate");
        res[i] = clamp(v[i], min_v[i], max_v[i]);
    }
    return res;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::clamp(const vec& v, T min_val, T max_val) noexcept {
    assert(min_val <= max_val && "min_val cannot be greater than max_val");

    vec res{};
    for (std::size_t i{0}; i < N; ++i) {
        res[i] = clamp(v[i], min_val, max_val);
    }
    return res;
}

template <traits::Scalar T, std::size_t N> constexpr T vec<T, N>::distance(const vec& start, const vec& end) noexcept {
    return (end - start).length();
}

template <traits::Scalar T, std::size_t N>
constexpr auto vec<T, N>::angle(const vec& v0, const vec& v1) noexcept requires std::floating_point<T> {
    return Angle<T>::acos(clamp(dot(v0.norm(), v1.norm()), T{-1}, T{1}));
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::reflect(const vec& incident, const vec& normal) noexcept requires std::floating_point<T>
{
    return incident - T{2} * dot(incident, normal) * normal;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::refract(const vec& incident, const vec& normal, T eta) noexcept
    requires std::floating_point<T> {
    const T d{dot(incident, normal)};
    const T k{T{1} - eta * eta * (T{1} - d * d)};
    if (k < T{0}) {
        return {};
    }
    return eta * incident - (eta * d + sqrt(k)) * normal;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::lerp(const vec& v0, const vec& v1, T t) noexcept requires std::floating_point<T> {
    assert(t >= T{0} && t <= T{1});
    return (T{1} - t) * v0 + t * v1;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::smoothstep(const vec& edge0, const vec& edge1, T t) noexcept
    requires std::floating_point<T> {
    assert(edge0 != edge1);

    const T t_lerp{clamp((t - edge0.length()) / (edge1.length() - edge0.length()), T{0}, T{1})};
    return lerp(edge0, edge1, t_lerp * t_lerp * (T{3} - T{2} * t_lerp));
}

template <traits::Scalar T, std::size_t N> constexpr vec<T, N> vec<T, N>::operator+() const noexcept { return *this; }

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::operator-(this vec self) noexcept requires traits::SignedScalar<T> {
    for (std::size_t i{0}; i < N; ++i) {
        self[i] = -self[i];
    }
    return self;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N>& vec<T, N>::operator+=(this vec& lhs, const vec& rhs) noexcept {
    for (std::size_t i{0}; i < N; ++i) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N>& vec<T, N>::operator-=(this vec& lhs, const vec& rhs) noexcept {
    for (std::size_t i{0}; i < N; ++i) {
        lhs[i] -= rhs[i];
    }
    return lhs;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N>& vec<T, N>::operator*=(this vec& lhs, const vec& rhs) noexcept {
    for (std::size_t i{0}; i < N; ++i) {
        lhs[i] *= rhs[i];
    }
    return lhs;
}

template <traits::Scalar T, std::size_t N> constexpr vec<T, N>& vec<T, N>::operator*=(this vec& v, T scalar) noexcept {
    for (std::size_t i{0}; i < N; ++i) {
        v[i] *= scalar;
    }
    return v;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N>& vec<T, N>::operator/=(this vec& lhs, const vec& rhs) noexcept {
    for (std::size_t i{0}; i < N; ++i) {
        assert(rhs[i] != T{0} && "Division by zero");
        lhs[i] /= rhs[i];
    }
    return lhs;
}

template <traits::Scalar T, std::size_t N> constexpr vec<T, N>& vec<T, N>::operator/=(this vec& v, T scalar) noexcept {
    assert(scalar != T{0} && "Division by zero");
    for (std::size_t i{0}; i < N; ++i) {
        v[i] /= scalar;
    }
    return v;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::operator+(this vec lhs, const vec& rhs) noexcept {
    lhs += rhs;
    return lhs;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::operator-(this vec lhs, const vec& rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::operator*(this vec lhs, const vec& rhs) noexcept {
    lhs *= rhs;
    return lhs;
}

template <traits::Scalar T, std::size_t N> constexpr vec<T, N> vec<T, N>::operator*(this vec v, T scalar) noexcept {
    v *= scalar;
    return v;
}

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::operator/(this vec lhs, const vec& rhs) noexcept {
    lhs /= rhs;
    return lhs;
}

template <traits::Scalar T, std::size_t N> constexpr vec<T, N> vec<T, N>::operator/(this vec v, T scalar) noexcept {
    assert(scalar != T{0} && "Division by zero");
    v /= scalar;
    return v;
}

template <traits::Scalar T, std::size_t N> constexpr T vec<T, N>::length_sq() const noexcept {
    return dot(*this, *this);
}

template <traits::Scalar T, std::size_t N> constexpr T vec<T, N>::length() const noexcept { return sqrt(length_sq()); }

template <traits::Scalar T, std::size_t N>
constexpr vec<T, N> vec<T, N>::norm(this vec self) noexcept requires std::floating_point<T> {
    if (const T len{self.length()}; len > T{0}) {
        self *= T{1} / len;
    }
    return self;
}

template <typename T, std::size_t N> constexpr vec<T, N> operator*(T scalar, vec<T, N> v) noexcept {
    return v * scalar;
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

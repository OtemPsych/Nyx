#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <print>
#include <type_traits>

#include <glm/ext.hpp>

#include <nyx/core/math/angle.h>
#include <nyx/core/math/mat.h>
#include <nyx/core/math/quat.h>
#include <nyx/core/math/util.h>
#include <nyx/core/math/vec.h>

void test_math_util() {
    static_assert(nyx::traits::Scalar<std::int32_t> &&
                  nyx::traits::Scalar<std::uint32_t> &&
                  nyx::traits::Scalar<float> &&
                  nyx::traits::Scalar<double> &&
                  !nyx::traits::Scalar<const char*>);
    static_assert(nyx::traits::SignedScalar<std::int32_t> &&
                  nyx::traits::SignedScalar<float> &&
                  nyx::traits::SignedScalar<double> &&
                  !nyx::traits::SignedScalar<std::uint32_t> &&
                  !nyx::traits::SignedScalar<std::size_t> &&
                  !nyx::traits::SignedScalar<const char*>);

    { // abs(T)
        static_assert(nyx::abs(-5) == 5 &&
                      nyx::abs(5) == 5 &&
                      nyx::abs(-0.0) == 0.0 &&
                      nyx::abs(-std::numeric_limits<double>::infinity()) == std::numeric_limits<double>::infinity());
    }
    { // fmod(T, T)
        constexpr float x{2.25f};
        constexpr float y{15.1f};
        constexpr float rem{nyx::fmod(x, y)};
        assert(rem == std::fmod(x, y));
    }

    { // signbit(T)
        static_assert(!nyx::signbit(0.0) && nyx::signbit(-0.0));
    }
    { // copysign(T, T)
        static_assert(nyx::copysign(2.0f, 0.0f) == 2.0f && nyx::copysign(2.0f, -0.0f) == -2.0f);
    }

    { // isnan(T)
        static_assert(nyx::isnan(std::numeric_limits<float>::quiet_NaN()) &&
                      !nyx::isnan(std::numeric_limits<float>::infinity()) &&
                      !nyx::isnan(0.0f));
    }
    { // isinf(T)
        static_assert(nyx::isinf(std::numeric_limits<float>::infinity()) &&
                      nyx::isinf(-std::numeric_limits<double>::infinity()) &&
                      !nyx::isinf(std::numeric_limits<float>::denorm_min()) &&
                      !nyx::isinf(0.0f));
    }
    { // isnormal(T)
        static_assert(nyx::isnormal(42.0f) &&
                      !nyx::isnormal(0.0) &&
                      !nyx::isnormal(std::numeric_limits<long double>::denorm_min()) &&
                      !nyx::isnormal(std::numeric_limits<double>::quiet_NaN()) &&
                      !nyx::isnormal(std::numeric_limits<float>::infinity()));
    }

    { // min(T, T)
        static_assert(nyx::min(3, 5) == 3 && nyx::min(-0.0001f, 0.0001f) == -0.0001f);
    }
    { // max(T, T)
        static_assert(nyx::max(-1, -5) == -1 && nyx::max(2.5f, 3.0f) == 3.0f);
    }
    { // clamp(T, T, T)
        static_assert(
            nyx::clamp(2, -3, 5) == 2 && nyx::clamp(-2.0f, 1.0f, 10.0f) == 1.0f && nyx::clamp(10.0, 1.0, 8.0) == 8.0);
    }
    { // approx_equal(T, T, std::uint_fast32_t)
        static_assert(nyx::approx_equal(1.0000001f, 1.000001f));
    }

    { // sqrt(T)
        static_assert(nyx::approx_equal(nyx::sqrt(4.0), 2.0) && nyx::approx_equal(nyx::sqrt(0.0), 0.0));
    }
    { // rsqrt(T)
        static_assert(nyx::approx_equal(nyx::rsqrt(4.0), 0.5) &&
                      nyx::rsqrt(std::numeric_limits<double>::infinity()) == 0.0 &&
                      nyx::isinf(nyx::rsqrt(0.0f)) &&
                      nyx::isnan(nyx::rsqrt(std::numeric_limits<float>::quiet_NaN())));
    }

    { // trunc(T)
        static_assert(nyx::trunc(-2.9f) == -2.0f && nyx::trunc(1.0f) == 1.0f && nyx::trunc(5.25f) == 5.0f);
    }
    { // floor(T)
        static_assert(nyx::floor(2.7) == 2.0 && nyx::floor(-2.7) == -3.0 && nyx::floor(5.9f) == 5.0f);
    }
    { // ceil(T)
        static_assert(nyx::ceil(2.3) == 3.0 && nyx::ceil(-2.3) == -2.0);
    }

    { // sin(T)
        static_assert(nyx::approx_equal(nyx::sin(0.0), 0.0) &&
                      nyx::approx_equal(nyx::sin(std::numbers::pi / 2.0), 1.0));

        constexpr double sin_res{nyx::sin(std::numbers::pi / 4.0)};
        assert(nyx::approx_equal(sin_res, std::sin(std::numbers::pi / 4.0)));
    }
    { // cos(T)
        static_assert(nyx::approx_equal(nyx::cos(0.0), 1.0) &&
                      nyx::approx_equal(nyx::cos(std::numbers::pi), -1.0, 1000));

        constexpr double cos_res{nyx::cos(std::numbers::pi / 4.0)};
        assert(nyx::approx_equal(cos_res, std::cos(std::numbers::pi / 4.0)));
    }
    { // tan(T)
        static_assert(nyx::approx_equal(nyx::tan(0.0), 0.0));
    }

    { // asin(T)
        static_assert(nyx::approx_equal(nyx::asin(0.0), 0.0) &&
                      nyx::approx_equal(nyx::asin(1.0), std::numbers::pi / 2.0));
    }
    { // acos(T)
        static_assert(nyx::approx_equal(nyx::acos(1.0), 0.0));
    }
    { // atan(T)
        static_assert(nyx::approx_equal(nyx::atan(0.0), 0.0) &&
                      nyx::approx_equal(nyx::atan(std::numeric_limits<double>::infinity()), std::numbers::pi / 2.0));
    }
    { // atan2(T, T)
        constexpr double pi{std::numbers::pi};
        constexpr double inf{std::numeric_limits<double>::infinity()};
        constexpr double nan{std::numeric_limits<double>::quiet_NaN()};

        static_assert(nyx::approx_equal(nyx::atan2(0.0, 1.0), 0.0) &&       // right (0)
                      nyx::approx_equal(nyx::atan2(1.0, 0.0), pi / 2.0) &&  // top (pi/2)
                      nyx::approx_equal(nyx::atan2(0.0, -1.0), pi) &&       // left (pi)
                      nyx::approx_equal(nyx::atan2(-1.0, 0.0), -pi / 2.0)); // bottom (-pi/2)

        static_assert(nyx::approx_equal(nyx::atan2(1.0, 1.0), pi / 4.0) &&          // Q1 (pi/4)
                      nyx::approx_equal(nyx::atan2(1.0, -1.0), 3.0 * pi / 4.0) &&   // Q2 (3pi/4)
                      nyx::approx_equal(nyx::atan2(-1.0, -1.0), -3.0 * pi / 4.0) && // Q3 (-3pi/4)
                      nyx::approx_equal(nyx::atan2(-1.0, 1.0), -pi / 4.0));         // Q4 (-pi/4)

        static_assert(nyx::approx_equal(nyx::atan2(+0.0, +1.0), +0.0) &&
                      nyx::approx_equal(nyx::atan2(-0.0, +1.0), -0.0) &&
                      nyx::approx_equal(nyx::atan2(+0.0, -1.0), +pi) &&
                      nyx::approx_equal(nyx::atan2(-0.0, -1.0), -pi));

        static_assert(nyx::approx_equal(nyx::atan2(inf, inf), pi / 4.0) &&
                      nyx::approx_equal(nyx::atan2(inf, -inf), 3.0 * pi / 4.0) &&
                      nyx::approx_equal(nyx::atan2(-inf, inf), -pi / 4.0) &&
                      nyx::approx_equal(nyx::atan2(-inf, -inf), -3.0 * pi / 4.0));

        static_assert(nyx::approx_equal(nyx::atan2(inf, 2.0), pi / 2.0) &&
                      nyx::approx_equal(nyx::atan2(-inf, 2.0), -pi / 2.0) &&
                      nyx::approx_equal(nyx::atan2(2.0, inf), 0.0) &&
                      nyx::approx_equal(nyx::atan2(2.0, -inf), pi));

        static_assert(
            nyx::isnan(nyx::atan2(nan, 1.0)) && nyx::isnan(nyx::atan2(1.0, nan)) && nyx::isnan(nyx::atan2(nan, nan)));
    }

    { // exp(T)
        static_assert(nyx::isnan(nyx::exp(std::numeric_limits<float>::quiet_NaN())) &&
                      nyx::exp(std::numeric_limits<double>::infinity()) == std::numeric_limits<double>::infinity() &&
                      nyx::exp(-std::numeric_limits<float>::infinity()) == 0.0f &&
                      nyx::approx_equal(nyx::exp(0.0f), 1.0f) &&
                      nyx::approx_equal(nyx::exp(1.0), std::numbers::e));
    }
    { // exp2(std::integral auto)
        static_assert(nyx::exp2(6) == 64.0f);
    }
    { // log(T)
        static_assert(nyx::approx_equal(nyx::log(1.0), 0.0) &&
                      nyx::approx_equal(nyx::log(std::numbers::e), 1.0) &&
                      nyx::isnan(nyx::log(-5.0)));
    }
    { // pow(T, T)
        static_assert(nyx::approx_equal(nyx::pow(2.0, 3.0), 8.0) &&
                      nyx::approx_equal(nyx::pow(5.0, 0.0), 1.0) &&
                      nyx::approx_equal(nyx::pow(-2.0, 3.0), -8.0) &&
                      nyx::approx_equal(nyx::pow(3.0f, 2.0f), 9.0f));
    }
}

void test_math_angle() {
    // Static methods
    { // radians(T)
        static_assert(nyx::Angled::radians(std::numbers::pi).radians() == std::numbers::pi);
    }
    { // degrees(T)
        static_assert(nyx::Anglef::degrees(180.0f).radians() == std::numbers::pi_v<float>);
    }

    { // asin(T)
        constexpr auto angle{nyx::Angleld::asin(1.0l)};
        static_assert(nyx::approx_equal(angle.degrees(), 90.0l));
    }
    { // acos(T)
        constexpr auto angle{nyx::Anglef::acos(1.0f)};
        static_assert(nyx::approx_equal(angle.degrees(), 0.0f));
    }
    { // atan(T)
        constexpr auto angle{nyx::Angled::atan(1.0)};
        static_assert(nyx::approx_equal(angle.degrees(), 45.0));
    }

    { // delta(Angle, Angle)
        constexpr auto from{nyx::Angle<>::degrees(10.0)};
        constexpr auto to{nyx::Angled::degrees(350.0)};
        constexpr auto delta{nyx::Angled::delta(from, to)};
        static_assert(nyx::approx_equal(delta.degrees(), -20.0));
    }
    { // distance(Angle, Angle)
        constexpr auto a{nyx::Angled::degrees(10.0)};
        constexpr auto b{nyx::Angled::degrees(350.0)};
        constexpr auto distance{nyx::Angled::distance(a, b)};
        static_assert(nyx::approx_equal(distance.degrees(), 20.0));
    }
    { // lerp(Angle, Angle, T)
        constexpr auto a{nyx::Anglef::degrees(0.0f)};
        constexpr auto b{nyx::Anglef::degrees(90.0f)};
        static_assert(nyx::approx_equal(nyx::Anglef::lerp(a, b, 0.0f).degrees(), 0.0f) &&
                      nyx::approx_equal(nyx::Anglef::lerp(a, b, 0.5f).degrees(), 45.0f) &&
                      nyx::approx_equal(nyx::Anglef::lerp(a, b, 1.0f).degrees(), 90.0f));
    }

    // Constructors
    { // Angle()
        static_assert(nyx::Angled{}.degrees() == 0.0);
    }

    // Operators
    { // operator+()
        constexpr auto angle{+nyx::Anglef::radians(std::numbers::pi_v<float>)};
        static_assert(angle.radians() == std::numbers::pi_v<float>);
    }
    { // operator-()
        constexpr auto angle{-nyx::Angled::degrees(45.5)};
        static_assert(angle.degrees() == -45.5);
    }

    { // operator+=(Angle)
        constexpr auto a{nyx::Angled::degrees(90.0)};
        auto b{nyx::Angle<>::degrees(45.0)};
        b += a;
        assert(nyx::approx_equal(b.degrees(), 135.0));
    }
    { // operator-=(Angle)
        constexpr auto a{nyx::Anglef::degrees(45.0f)};
        auto b{nyx::Anglef::degrees(135.0f)};
        b -= a;
        assert(nyx::approx_equal(b.degrees(), 90.0f));
    }
    { // operator*=(T)
        auto a{nyx::Angled::degrees(90.0)};
        a *= 2.0;
        assert(nyx::approx_equal(a.degrees(), 180.0));
    }
    { // operator/=(T)
        auto a{nyx::Anglef::degrees(180.0f)};
        a /= 2.0f;
        assert(nyx::approx_equal(a.degrees(), 90.0f));
    }
    { // operator%=(Angle)
        constexpr auto mod{nyx::Angled::degrees(360.0)};
        auto m{nyx::Angled::degrees(370.0)};
        m %= mod;
        assert(nyx::approx_equal(m.degrees(), 10.0));
    }

    { // operator+(Angle, Angle)
        constexpr auto a{nyx::Angle<>::degrees(60.0)};
        constexpr auto b{nyx::Angle<>::degrees(30.0)};
        static_assert(nyx::approx_equal((a + b).degrees(), 90.0));
    }
    { // operator-(Angle, Angle)
        constexpr auto a{nyx::Angle<>::degrees(30.0)};
        constexpr auto b{nyx::Angle<>::degrees(60.0)};
        static_assert(nyx::approx_equal((a - b).degrees(), -30.0));
    }
    { // operator*(Angle, T)
        constexpr auto a{nyx::Angle<float>::degrees(120.0f)};
        static_assert(nyx::approx_equal((a * 2.0f).degrees(), 240.0f));
    }
    { // operator*(T, Angle)
        constexpr auto a{nyx::Angle<>::degrees(-64.0)};
        static_assert(nyx::approx_equal((2.0 * a).degrees(), -128.0));
    }
    { // operator/(Angle, T)
        constexpr auto a{nyx::Angle<long double>::degrees(60.0l)};
        static_assert(nyx::approx_equal((a / 2.0l).degrees(), 30.0l));
    }
    { // operator/(Angle, Angle)
        constexpr auto a{nyx::Angle<>::degrees(90.0)};
        constexpr auto b{nyx::Angle<>::degrees(30.0)};
        static_assert(nyx::approx_equal(a / b, 3.0));
    }
    { // operator%(Angle, Angle)
        constexpr auto mod{nyx::Angle<>::degrees(360.0)};
        constexpr auto m{nyx::Angle<>::degrees(370.0)};
        static_assert(nyx::approx_equal((m % mod).degrees(), 10.0));
    }

    // Methods
    { // radians()
        constexpr auto a{nyx::Angle<float>::degrees(180.0f)};
        static_assert(nyx::approx_equal(a.radians(), std::numbers::pi_v<float>));
    }
    { // degrees()
        constexpr auto a{nyx::Angle<double>::radians(std::numbers::pi)};
        static_assert(nyx::approx_equal(a.degrees(), 180.0));
    }

    { // sin()
        constexpr auto a{nyx::Angle<>::degrees(90.0)};
        static_assert(nyx::approx_equal(a.sin(), 1.0));
    }
    { // cos()
        constexpr auto a{nyx::Angle<>::degrees(0.0)};
        static_assert(nyx::approx_equal(a.cos(), 1.0));
    }
    { // tan()
        constexpr auto a{nyx::Angle<>::degrees(45.0)};
        static_assert(nyx::approx_equal(a.tan(), 1.0));
    }

    { // wrap_pi()
        constexpr auto a{nyx::Angle<float>::degrees(270.0f).wrap_pi()};
        constexpr auto b{nyx::Angle<float>::degrees(-270.0f).wrap_pi()};
        constexpr auto c{nyx::Angle<float>::degrees(180.0f).wrap_pi()};
        static_assert(nyx::approx_equal(a.degrees(), -90.0f) &&
                      nyx::approx_equal(b.degrees(), 90.0f) &&
                      nyx::approx_equal(c.degrees(), -180.0f));
    }
    { // wrap_tau()
        constexpr auto a{nyx::Angle<>::degrees(450.0).wrap_tau()};
        constexpr auto b{nyx::Angle<>::degrees(-90.0).wrap_tau()};
        constexpr auto c{nyx::Angle<>::degrees(360.0).wrap_tau()};
        static_assert(nyx::approx_equal(a.degrees(), 90.0) &&
                      nyx::approx_equal(b.degrees(), 270.0) &&
                      nyx::approx_equal(c.degrees(), 0.0));
    }

    // Literals
    {
        using namespace nyx::literals;
        static_assert(45.0_deg == nyx::Angle<>::degrees(45.0) &&
                      50_deg == nyx::Angle<>::degrees(50.0) &&
                      1.2_rad == nyx::Angle<>::radians(1.2) &&
                      1_rad == nyx::Angle<>::radians(1.0));

        std::println("{:.2f}", 1_rad);
    }
}

void test_math_vec() {
    { // vec()
        constexpr nyx::vec<float, 5> v5{};
        static_assert(v5.coords[0] == 0.0f &&
                      v5.coords[1] == 0.0f &&
                      v5.coords[2] == 0.0f &&
                      v5.coords[3] == 0.0f &&
                      v5.coords[4] == 0.0f);

        constexpr nyx::vec2ifast32 v2{};
        static_assert(v2.x == 0 && v2.y == 0);

        constexpr nyx::vec3d v3{};
        static_assert(v3.x == 0.0 && v3.y == 0.0 && v3.z == 0.0f);

        constexpr nyx::vec4i v4{};
        static_assert(v4.x == 0 && v4.y == 0 && v4.z == 0 && v4.w == 0);
    }
    { // vec(T)
        constexpr nyx::vec<std::uint32_t, 5> v5{3};
        static_assert(
            v5.coords[0] == 3 && v5.coords[1] == 3 && v5.coords[2] == 3 && v5.coords[3] == 3 && v5.coords[4] == 3);

        constexpr nyx::vec2i8 v2{32};
        static_assert(v2.x == 32 && v2.y == 32);

        constexpr nyx::vec3f v3{2.5f};
        static_assert(v3.x == 2.5f && v3.y == 2.5f && v3.z == 2.5f);

        constexpr nyx::vec4u64 v4{30};
        static_assert(v4.x == 30 && v4.y == 30 && v4.z == 30 && v4.w == 30);
    }
    { // vec(T...)
        constexpr nyx::vec<int, 5> v5{0, 1, 2, 3, 4};
        static_assert(v5[0] == 0 && v5[1] == 1 && v5[2] == 2 && v5[3] == 3 && v5[4] == 4);

        constexpr nyx::vec2l v2{5, 3};
        static_assert(v2.x == 5 && v2.y == 3);

        constexpr nyx::vec3u v3{3, 1, 8};
        static_assert(v3.x == 3 && v3.y == 1 && v3.z == 8);

        constexpr nyx::vec4i16 v4{-2, 3, 0, 2};
        static_assert(v4.x == -2 && v4.y == 3 && v4.z == 0 && v4.w == 2);
    }
    { // vec(const vec&)
        constexpr nyx::vec<float, 6> v6{-2.0f, 3.0f, 0.0f, 5.25f, 1.02f, -75.0f};

        constexpr nyx::vec<int, 5> v5{v6};
        static_assert(v5[0] == -2 && v5[1] == 3 && v5[2] == 0 && v5[3] == 5 && v5[4] == 1);

        constexpr nyx::vec4d v4{v6};
        static_assert(v4.x == -2.0 && v4.y == 3.0 && v4.z == 0.0 && v4.w == 5.25);

        constexpr nyx::vec3ifast32 v3{v6};
        static_assert(v3.x == -2 && v3.y == 3 && v3.z == 0);

        constexpr nyx::vec2f v2{v6};
        static_assert(v2.x == -2.0f && v2.y == 3.0f);
    }
    { // vec(const vec&, T...)
        constexpr nyx::vec3u v3{
            nyx::vec2f{2.0f, 12.0f},
            5
        };
        static_assert(v3.x == 2 && v3.y == 12 && v3.z == 5);

        constexpr nyx::vec4f v4_0{
            nyx::vec2u{2, 5},
            1.0f, -10.0f
        };
        static_assert(v4_0.x == 2.0f && v4_0.y == 5.0f && v4_0.z == 1.0f && v4_0.w == -10.0f);

        constexpr nyx::vec4ileast64 v4_1{
            nyx::vec3ld{5.0, -12.0, 80.0},
            -15
        };
        static_assert(v4_1.x == 5 && v4_1.y == -12 && v4_1.z == 80 && v4_1.w == -15);
    }

    // Subscript variations
    {
        nyx::vec3i v{10, 20, 30};
        v[1] = 99;
        assert(v.y == 99);
    }
    {
        constexpr nyx::vec3i v{40, 50, 60};
        static_assert(v[0] == 40 && v[2] == 60);
        static_assert(std::is_const_v<std::remove_reference_t<decltype(v[0])>>);
    }
    static_assert(nyx::vec2i{5, 10}[0] == 5);

    { // In-place binary arithmetic operators
        nyx::vec2f a{2.0f, 4.0f};
        nyx::vec2f b{3.0f, 1.0f};

        // Compound assignment operators
        a += b;
        assert(a.x == 5.0f && a.y == 5.0f);

        a -= b;
        assert(a.x == 2.0f && a.y == 4.0f);

        a *= b;
        assert(a.x == 6.0f && a.y == 4.0f);

        a /= b;
        assert(a.x == 2.0f && a.y == 4.0f);

        // Scalar arithmetic operators
        a *= 2.0f;
        assert(a.x == 4.0f && a.y == 8.0f);

        a /= 4.0f;
        assert(a.x == 1.0f && a.y == 2.0f);

        a = {2.0f, 4.0f};
        nyx::vec2f res_add{a + b};
        assert(res_add.x == 5.0f && res_add.y == 5.0f);
        assert(a.x == 2.0f);

        nyx::vec2f res_scalar_left{3.0f * a};
        assert(res_scalar_left.x == 6.0f && res_scalar_left.y == 12.0f);

        constexpr nyx::vec2i signed_v{5, -10};
        constexpr nyx::vec2i negated{-signed_v};
        static_assert(negated.x == -5 && negated.y == 10);
    }

    // Static directional components and basis anchors
    static_assert(nyx::vec2i::up() == nyx::vec2i{0, -1});
    static_assert(nyx::vec2f::down() == nyx::vec2f{0.0f, 1.0f});
    static_assert(nyx::vec2d::left() == nyx::vec2d{-1.0, 0.0});
    static_assert(nyx::vec2i64::right() == nyx::vec2i64{1, 0});

    static_assert(nyx::vec3i::up() == nyx::vec3i{0, 1, 0});
    static_assert(nyx::vec3f::down() == nyx::vec3f{0.0f, -1.0f, 0.0f});
    static_assert(nyx::vec3d::left() == nyx::vec3d{-1.0, 0.0, 0.0});
    static_assert(nyx::vec3i::forward() == nyx::vec3i{0, 0, -1});
    static_assert(nyx::vec3i::back() == nyx::vec3i{0, 0, 1});

    // Geometric math operators
    { // Magnitude with Normalization
        constexpr nyx::vec2f v{3.0f, 4.0f};
        static_assert(v.length() == 5.0f);

        constexpr nyx::vec2f unit = v.norm();
        static_assert(nyx::approx_equal(unit.x, 0.6f) && nyx::approx_equal(unit.y, 0.8f));
        static_assert(nyx::approx_equal(unit.length(), 1.0f));

        constexpr nyx::vec2f zero_v{0.0f, 0.0f};
        static_assert(zero_v.norm() == zero_v);
    }
    static_assert(nyx::vec3f::dot(nyx::vec3f{1.0f, 3.0f, -5.0f}, nyx::vec3f{4.0f, -2.0f, -1.0f}) == 3.0f);
    static_assert(nyx::vec3i::cross(nyx::vec3i{1, 0, 0}, nyx::vec3i{0, 1, 0}) == nyx::vec3i{0, 0, 1});

    { // Reflection Refraction
        constexpr nyx::vec2f incident{1.0f, -1.0f};
        constexpr nyx::vec2f normal{0.0f, 1.0f};

        constexpr nyx::vec2f reflected{nyx::vec2f::reflect(incident.norm(), normal)};
        static_assert(nyx::approx_equal(reflected.y, 0.707107f));

        constexpr nyx::vec2f refracted{nyx::vec2f::refract(incident.norm(), normal, 2.5f)};
        static_assert(refracted == nyx::vec2f{});
    }
}

void test_math_quat() {
    using namespace nyx::literals;

    // Static methods
    { // from(vec3<T>, vec3<T>)
        {
            constexpr auto v = nyx::vec3d{1.0f, 2.0f, 3.0f}.norm();
            constexpr auto q{nyx::quatd::from(v, v)};
            static_assert(nyx::approx_equal(q.x, 0.0) &&
                          nyx::approx_equal(q.y, 0.0) &&
                          nyx::approx_equal(q.z, 0.0) &&
                          nyx::approx_equal(q.w, 1.0));
        }
        {
            constexpr nyx::vec3f start{1.0f, 0.0f, 0.0f};
            constexpr nyx::vec3f end{-1.0f, 0.0f, 0.0f};
            constexpr auto q{nyx::quatf::from(start, end)};
            constexpr auto rotated{q.rotate(start)};
            static_assert(nyx::approx_equal(rotated.x, -1.0f) &&
                          nyx::approx_equal(rotated.y, 0.0f) &&
                          nyx::approx_equal(rotated.z, 0.0f));
        }
    }
    { // from(vec3<T>, Angle<T>)
        constexpr nyx::vec3ld axis{0.0l, 0.0l, 1.0l};
        constexpr auto q{nyx::quatld::from(axis, 90_degld)};
        static_assert(nyx::approx_equal(q.x, 0.0l) &&
                      nyx::approx_equal(q.y, 0.0l) &&
                      nyx::approx_equal(q.z, std::numbers::sqrt2_v<long double> / 2.0l) &&
                      nyx::approx_equal(q.w, std::numbers::sqrt2_v<long double> / 2.0l));
    }
    { // from(Angle<T>, Angle<T>, Angle<T>)
        constexpr auto q{nyx::quatf::from(0_radf, 0_radf, 0_radf)};
        static_assert(nyx::approx_equal(q.x, 0.0f) &&
                      nyx::approx_equal(q.y, 0.0f) &&
                      nyx::approx_equal(q.z, 0.0f) &&
                      nyx::approx_equal(q.w, 1.0f));
    }
    { // dot(const quat&, const quat&)
        constexpr float d{nyx::quatf::dot(nyx::quatf{1.0f, 2.0f, 3.0f, 4.0f}, nyx::quatf{2.0f, 3.0f, 4.0f, 5.0f})};
        static_assert(d == 40.0f);
    }

    // Constructors
    { // quat()
        constexpr nyx::quatf q;
        static_assert(q.x == 0.0f && q.y == 0.0f && q.z == 0.0f && q.w == 0.0f);
    }
    { // quat(T, T, T, T)
        constexpr nyx::quatd q{1.0, 2.0, 3.0, 4.0};
        static_assert(q.x == 1.0 && q.y == 2.0 && q.z == 3.0 && q.w == 4.0);
    }

    // Operators
    { // operator+()
        constexpr auto q{
            +nyx::quatf{1.0f, -2.0f, 3.0f, -4.0f}
        };
        static_assert(q.x == 1.0f && q.y == -2.0f && q.z == 3.0f && q.w == -4.0f);
    }
    { // operator-()
        constexpr auto q{
            -nyx::quatd{1.0, -2.0, 3.0, 4.0}
        };
        static_assert(q.x == -1.0 && q.y == 2.0 && q.z == -3.0 && q.w == -4.0);
    }

    { // operator+=(const quat&)
        constexpr nyx::quatd q0{1.0, 2.0, 3.0, 4.0};
        constexpr nyx::quatd q1{5.0, 6.0, 7.0, 8.0};

        nyx::quatd add{q0};
        add += q1;
        assert(add.x == q0.x + q1.x && add.y == q0.y + q1.y && add.z == q0.z + q1.z && add.w == q0.w + q1.w);
    }
    { // operator-=(const quat&)
        constexpr nyx::quatd q0{1.0, 2.0, 3.0, 4.0};
        constexpr nyx::quatd q1{5.0, 6.0, 7.0, 8.0};

        nyx::quatd sub{q0};
        sub -= q1;
        assert(sub.x == q0.x - q1.x && sub.y == q0.y - q1.y && sub.z == q0.z - q1.z && sub.w == q0.w - q1.w);
    }
    { // operator*=(this quat lhs, const quat&)
        constexpr nyx::quatd i{1.0, 0.0, 0.0, 0.0};
        constexpr nyx::quatd j{0.0, 1.0, 0.0, 0.0};
        constexpr nyx::quatd k{0.0, 0.0, 1.0, 0.0};
        auto ij{i};
        ij *= j;
        assert(nyx::approx_equal(ij.x, k.x) &&
               nyx::approx_equal(ij.y, k.y) &&
               nyx::approx_equal(ij.z, k.z) &&
               nyx::approx_equal(ij.w, k.w));
    }
    { // operator*=(T)
        constexpr nyx::quatld q0{1.0l, 2.0l, 3.0l, 4.0l};
        nyx::quatld q1{q0};
        q1 *= 2.0l;
        assert(q1.x == q0.x * 2.0l && q1.y == q0.y * 2.0l && q1.z == q0.z * 2.0l && q1.w == q0.w * 2.0l);
    }
    { // operator/=(T)
        constexpr nyx::quatf q0{2.0f, 4.0f, 6.0f, 8.0f};
        nyx::quatf q1{q0};
        q1 /= 2.0f;
        assert(q1.x == q0.x / 2.0f && q1.y == q0.y / 2.0f && q1.z == q0.z / 2.0f && q1.w == q0.w / 2.0f);
    }

    { // operator+(this quat, const quat&)
        constexpr nyx::quatd q0{1.0, 2.0, 3.0, 4.0};
        constexpr nyx::quatd q1{5.0, 6.0, 7.0, 8.0};

        constexpr nyx::quatd add{q0 + q1};
        static_assert(add.x == q0.x + q1.x && add.y == q0.y + q1.y && add.z == q0.z + q1.z && add.w == q0.w + q1.w);
    }
    { // operator-(this quat, const quat&)
        constexpr nyx::quatd q0{1.0, 2.0, 3.0, 4.0};
        constexpr nyx::quatd q1{5.0, 6.0, 7.0, 8.0};

        constexpr nyx::quatd sub{q0 - q1};
        static_assert(sub.x == q0.x - q1.x && sub.y == q0.y - q1.y && sub.z == q0.z - q1.z && sub.w == q0.w - q1.w);
    }
    { // operator*(this quat, const quat&)
        constexpr nyx::quatd i{1.0, 0.0, 0.0, 0.0};
        constexpr nyx::quatd j{0.0, 1.0, 0.0, 0.0};
        constexpr nyx::quatd k{0.0, 0.0, 1.0, 0.0};
        constexpr auto ij{i * j};
        static_assert(nyx::approx_equal(ij.x, k.x) &&
                      nyx::approx_equal(ij.y, k.y) &&
                      nyx::approx_equal(ij.z, k.z) &&
                      nyx::approx_equal(ij.w, k.w));
    }
    { // operator*(const vec3<T>&)
        constexpr auto q = nyx::quatf::from(nyx::vec3f{1.0f, -2.0f, 0.0f}.norm(), 45_radf);
        constexpr nyx::vec3f v{25.0f, 12.0f, -2.0f};
        static_assert(q * v == q.rotate(v));
    }
    { // operator*(this quat, T)
        constexpr auto q = nyx::quatd{1.0, 2.0, 3.0, 4.0} * 2.0;
        static_assert(q.x == 2.0f && q.y == 4.0 && q.z == 6.0 && q.w == 8.0);
    }
    { // operator/(this quat, T)
        constexpr auto q = nyx::quatf{2.0f, 4.0f, 10.0f, 12.0f} / 2.0f;
        static_assert(q.x == 1.0f && q.y == 2.0f && q.z == 5.0f && q.w == 6.0f);
    }

    // Methods
    { // euler()
        constexpr auto pitch = 25_degd;
        constexpr auto yaw = 25_degd;
        constexpr auto roll = 25_degd;
        constexpr auto euler = nyx::quatd::from(pitch, yaw, roll).euler();
        static_assert(nyx::approx_equal(euler.x, pitch.radians()) &&
                      nyx::approx_equal(euler.y, yaw.radians()) &&
                      nyx::approx_equal(euler.z, roll.radians()));
    }
    { // rotate(const vec3<T>&)
        constexpr nyx::vec3f axis{0.0f, 0.0f, 1.0f};
        constexpr auto q = nyx::quatf::from(axis, 90_degf);

        constexpr auto rotated = q.rotate(nyx::vec3f{1.0f, 0.0f, 0.0f});
        static_assert(nyx::approx_equal(rotated.x, 0.0f) &&
                      nyx::approx_equal(rotated.y, 1.0f) &&
                      nyx::approx_equal(rotated.z, 0.0f));
    }
    { // conjugate()
        constexpr auto q = nyx::quatd{1.0, -2.0, 3.0, -4.0};
        constexpr auto conj = q.conjugate();
        static_assert(conj.x == -q.x && conj.y == -q.y && conj.z == -q.z && conj.w == q.w);
    }
    { // length_sq(), length()
        constexpr auto q = nyx::quatf{1.0f, 2.0f, 2.0f, 4.0f};
        static_assert(q.length_sq() == 25.0f && q.length() == 5.0f);
    }
    { // norm(this quat)
        constexpr auto q = nyx::quatd{2.0, 0.0, 0.0, 0.0};
        constexpr auto norm = q.norm();
        static_assert(nyx::approx_equal(norm.length(), 1.0) && nyx::approx_equal(norm.x, 1.0));
    }
}

void test_math_mat() {
    constexpr auto equal = []<typename T, std::size_t R, std::size_t C>(const nyx::mat<T, R, C>& m,
                                                                        const glm::mat<C, R, T>& glm_m) {
        for (std::size_t c{0}; c < C; ++c) {
            for (std::size_t r{0}; r < R; ++r) {
                if (!nyx::approx_equal(m[r, c], glm_m[c][r])) {
                    return false;
                }
            }
        }
        return true;
    };
    constexpr auto approx_equal = []<typename T, std::size_t R, std::size_t C>(const nyx::mat<T, R, C>& m0,
                                                                               const nyx::mat<T, R, C>& m1) {
        for (std::size_t i{0}; i < R * C; ++i) {
            if (!nyx::approx_equal(m0[i], m1[i])) {
                return false;
            }
        }
        return true;
    };

    // Static methods
    { // identity()
        constexpr auto identity = nyx::mat4f::identity();
        constexpr auto glm_identity = glm::identity<glm::mat4>();
        static_assert(equal(identity, glm_identity));
    }
    { // orthographic(T, T, T, T, T, T)
        constexpr auto ortho = nyx::mat4f::orthographic(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1000.0f);
        auto glm_ortho = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1000.0f);
        assert(equal(ortho, glm_ortho));
    }
    { // orthographic(T, T, T, T)
        constexpr auto ortho = nyx::mat4f::orthographic(0.0f, 640.0f, 480.0f, 0.0f);
        auto glm_ortho = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);
        assert(equal(ortho, glm_ortho));
    }
    { // perspective(Angle<T>, T, T, T)
        constexpr auto persp = nyx::mat4f::perspective(nyx::Anglef::degrees(90.0f), 1280.0f / 720.0f, -1.0f, 1000.0f);
        auto glm_persp = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, -1.0f, 1000.0f);
        assert(equal(persp, glm_persp));
    }
    { // lookat(const vec3<T>&, const vec3<T>&, const vec3<T>&)
        constexpr auto lookat =
            nyx::mat4f::lookat(nyx::vec3f{5.0f, -10.0f, 28.0f}, nyx::vec3f{0.0f, -1.0f, 1.0f}, nyx::vec3f::up());
        auto glm_lookat =
            glm::lookAt(glm::vec3{5.0f, -10.0f, 28.0f}, glm::vec3{0.0f, -1.0f, 1.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
        assert(equal(lookat, glm_lookat));
    }

    { // translate(const vec<T, R-1>&)
        constexpr auto translate = nyx::mat4f::translate(nyx::vec3f{0.25f, 0.1f, -2.5f});
        constexpr auto glm_translate = glm::translate(glm::mat4{1.0f}, glm::vec3{0.25f, 0.1f, -2.5f});
        static_assert(equal(translate, glm_translate));
    }
    { // rotate(const quat<T>&)
        constexpr auto q = nyx::quatf::from(nyx::vec3f{12.0f, -5.25f, 1.5f}.norm(), nyx::Anglef::degrees(45.0f));
        constexpr auto rotate = nyx::mat4f::rotate(q);

        auto glm_q = glm::angleAxis(glm::radians(45.0f), glm::normalize(glm::vec3{12.0f, -5.25f, 1.5f}));
        auto glm_rotate = glm::mat4_cast(glm_q);

        assert(equal(rotate, glm_rotate));
    }
    { // scale(const vec<T, R-1>&)
        constexpr auto scale = nyx::mat4f::scale(nyx::vec3f{0.5f, -0.25f, 2.0f});
        auto glm_scale = glm::scale(glm::mat4{1.0f}, glm::vec3{0.5f, -0.25f, 2.0f});
        assert(equal(scale, glm_scale));
    }

    { // rows()
        static_assert(nyx::mat<int, 3, 2>::rows() == 3);
    }
    { // cols()
        static_assert(nyx::mat<int, 3, 2>::cols() == 2);
    }

    // Operators
    { // operator+(), operator-()
        constexpr auto m = nyx::mat3i32::identity();
        static_assert(+m == m);

        constexpr auto neg_m = -m;
        static_assert(neg_m[0, 0] == -1 && neg_m[1, 1] == -1 && neg_m[2, 2] == -1 && neg_m[1, 2] == 0);
    }
    { // operator+=(const mat&), operator-=(const mat&)
        auto m0 = nyx::mat4d::identity();
        constexpr auto m1 = nyx::mat4d::identity();
        m0 += m1;
        assert((m0[0, 0] == 2.0 && m0[1, 1] == 2.0 && m0[2, 2] == 2.0 && m0[3, 3] == 2.0));

        m0 -= m1;
        assert(m0 == nyx::mat4d::identity());
    }
    { // operator*=(const mat<T, C, C>&), operator*(this const mat&, const mat<T, C, C>&)
        constexpr auto scale1 = nyx::mat4f::scale(nyx::vec3f{0.5f, 0.25f, 2.0f});
        auto scale2 = nyx::mat4f::scale(nyx::vec3f{0.8f, 1.25f, 0.2f});
        scale2 *= scale1;

        auto glmscale1 = glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 0.25f, 2.0f));
        auto glmscale2 = glm::scale(glm::mat4(1.f), glm::vec3(0.8f, 1.25f, 0.2f));
        glmscale2 *= glmscale1;

        assert(equal(scale2, glmscale2));

        scale2 = nyx::mat4f::scale(nyx::vec3f{0.8f, 1.25f, 0.2f}) * scale1;
        assert(equal(scale2, glmscale2));
    }
    { // operator*=(T), operator/=(T), operator*(this mat, T), operator/(this mat, T)
        auto m = nyx::mat3i32::identity();
        m *= 2;
        assert((m[0, 0] == 2 && m[1, 1] == 2 && m[2, 2] == 2));
        m /= 2;
        assert((m[0, 0] == 1 && m[1, 1] == 1 && m[2, 2] == 1));

        m = m * 2;
        assert((m[0, 0] == 2 && m[1, 1] == 2 && m[2, 2] == 2));
        m = m / 2;
        assert((m[0, 0] == 1 && m[1, 1] == 1 && m[2, 2] == 1));
    }

    // Methods
    { // Test determinants and inverses
        // 2x2 determinant & inversion
        constexpr nyx::mat2f m2{
            {3, 1, 2, 4}
        }; // det = 3*4 - 2*1 = 10
        static_assert(m2.determinant() == 10.0f);

        constexpr auto m2_inv = m2.invert();
        constexpr auto i2_test = m2 * m2_inv;
        static_assert(approx_equal(i2_test, nyx::mat2f::identity()));

        // 3x3 fundamental adjoint identity: A * adj(A) = det(A) * I
        constexpr nyx::mat3f m3{
            {2, -1, 0, 1, 3, -2, 0, 1, 1}
        };
        constexpr auto det3 = m3.determinant();
        constexpr auto adj3 = m3.adjoint();
        constexpr auto adj_prod3 = m3 * adj3;
        constexpr auto expected_adj_prod3 = nyx::mat3f::identity() * det3;
        static_assert(approx_equal(adj_prod3, expected_adj_prod3));

        constexpr auto m3_inv = m3.invert();
        constexpr auto i3_test = m3 * m3_inv;
        static_assert(approx_equal(i3_test, nyx::mat3f::identity()));

        // 4x4 determinant & inversion
        constexpr nyx::mat4f m4{
            {1, 0, 2, -1, 3, 0, 0, 5, 2, 1, 4, -3, 1, 0, 5, 0}
        };
        constexpr auto det4 = m4.determinant();
        static_assert(det4 != 0.0f);

        constexpr auto m4_inv = m4.invert();
        constexpr auto i4_test = m4 * m4_inv;
        static_assert(approx_equal(i4_test, nyx::mat4f::identity()));
    }
    { // Test integer scalars and edge cases
        // Integer matrix algebra
        constexpr auto a = nyx::mat3i{
            {1, 4, 7, 2, 5, 8, 3, 6, 9}
        };
        constexpr auto b = nyx::mat3i::identity();
        constexpr auto c = a * b;
        static_assert(c == a);

        constexpr auto det = a.determinant();
        static_assert(det == 0); // singular integer matrix (linearly dependent rows)

        constexpr auto non_singular = nyx::mat2i{
            {2, 1, 5, 3}
        };
        static_assert(non_singular.determinant() == 1);

        // Precision verification
        auto ad = nyx::mat3d::identity();
        ad[0, 2] = std::numbers::pi;
        auto ad_inv = ad.invert();
        assert(approx_equal(ad * ad_inv, nyx::mat3d::identity()));
    }
}

int main() {
    test_math_util();
    test_math_angle();
    test_math_vec();
    test_math_quat();
    test_math_mat();
}

//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2018 Ryo Suzuki
//	Copyright (c) 2016-2018 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <cmath>
# include <utility>

	/// <summary>
	/// 2 次元ベクトル
	/// </summary>
struct Vector2D
{
	double m_x, m_y;

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Vector2D() noexcept = default;

	constexpr Vector2D(const Vector2D&) noexcept = default;

	template <class X, class Y>
	constexpr Vector2D(X _x, Y _y) noexcept
		: m_x(static_cast<double>(_x))
		, m_y(static_cast<double>(_y)) {}

	constexpr Vector2D(double _x, double _y) noexcept
		: m_x(_x)
		, m_y(_y) {}

	constexpr double elem(size_t index) const noexcept
	{
		return index == 0 ? m_x
			: index == 1 ? m_y
			: 0;
	}

	constexpr Vector2D operator +() const noexcept
	{
		return *this;
	}

	constexpr Vector2D operator -() const noexcept
	{
		return{ -m_x, -m_y };
	}

	constexpr Vector2D operator +(const Vector2D& v) const noexcept
	{
		return{ m_x + v.m_x, m_y + v.m_y };
	}

	constexpr Vector2D operator -(const Vector2D& v) const noexcept
	{
		return{ m_x - v.m_x, m_y - v.m_y };
	}

	constexpr Vector2D operator *(double s) const noexcept
	{
		return{ m_x * s, m_y * s };
	}

	constexpr Vector2D operator *(const Vector2D& v) const noexcept
	{
		return{ m_x * v.m_x, m_y * v.m_y };
	}

	constexpr Vector2D operator /(double s) const noexcept
	{
		return *this * (static_cast<double>(1.0) / s);
	}

	constexpr Vector2D operator /(const Vector2D& v) const noexcept
	{
		return{ m_x / v.m_x, m_y / v.m_y };
	}

	constexpr Vector2D& operator +=(const Vector2D& v) noexcept
	{
		m_x += v.m_x; m_y += v.m_y;
		return *this;
	}

	constexpr Vector2D& operator -=(const Vector2D& v) noexcept
	{
		m_x -= v.m_x; m_y -= v.m_y;
		return *this;
	}

	constexpr Vector2D& operator *=(double s) noexcept
	{
		m_x *= s; m_y *= s;
		return *this;
	}

	constexpr Vector2D& operator *=(const Vector2D& v) noexcept
	{
		m_x *= v.m_x; m_y *= v.m_y;
		return *this;
	}

	constexpr Vector2D& operator /=(double s) noexcept
	{
		return *this *= (static_cast<double>(1.0) / s);
	}

	constexpr Vector2D& operator /=(const Vector2D& v) noexcept
	{
		m_x /= v.m_x; m_y /= v.m_y;
		return *this;
	}

	constexpr bool operator ==(const Vector2D& v) const noexcept
	{
		return v.m_x == m_x && v.m_y == m_y;
	}

	constexpr bool operator !=(const Vector2D& v) const noexcept
	{
		return v.m_x != m_x || v.m_y != m_y;
	}

	constexpr Vector2D& set(double _x, double _y) noexcept
	{
		m_x = _x; m_y = _y;
		return *this;
	}

	constexpr Vector2D& set(const Vector2D& v) noexcept
	{
		return *this = v;
	}

	[[nodiscard]] constexpr Vector2D movedBy(double _x, double _y) const noexcept
	{
		return{ m_x + _x, m_y + _y };
	}

	[[nodiscard]] constexpr Vector2D movedBy(const Vector2D& v) const noexcept
	{
		return{ m_x + v.m_x, m_y + v.m_y };
	}

	constexpr Vector2D& moveBy(double _x, double _y) noexcept
	{
		m_x += _x; m_y += _y;
		return *this;
	}

	constexpr Vector2D& moveBy(const Vector2D& v) noexcept
	{
		return *this += v;
	}

	constexpr bool isZero() const noexcept
	{
		return m_x == static_cast<double>(0.0)
			&& m_y == static_cast<double>(0.0);
	}

	bool hasNaN() const noexcept
	{
		return std::isnan(m_x)
			|| std::isnan(m_y);
	}

	constexpr double dot(const Vector2D& v) const noexcept
	{
		return m_x * v.m_x + m_y * v.m_y;
	}

	constexpr double cross(const Vector2D& v) const noexcept
	{
		return m_x * v.m_y - m_y * v.m_x;
	}

	double length() const noexcept
	{
		return std::sqrt(lengthSq());
	}

	constexpr double lengthSq() const noexcept
	{
		return dot(*this);
	}

	double lengthInv() const noexcept
	{
		return static_cast<double>(1.0) / length();
	}

	Vector2D& setLength(double _length) noexcept
	{
		const double len = length();

		if (len == 0.0)
		{
			return *this;
		}

		return *this *= (_length / len);
	}

	double distanceFrom(double _x, double _y) const noexcept
	{
		return distanceFrom({ _x, _y });
	}

	double distanceFrom(const Vector2D& v) const noexcept
	{
		return (*this - v).length();
	}

	constexpr double distanceFromSq(double _x, double _y) const noexcept
	{
		return distanceFromSq({ _x, _y });
	}

	constexpr double distanceFromSq(const Vector2D& v) const noexcept
	{
		return (*this - v).lengthSq();
	}

	[[nodiscard]] Vector2D normalized() const noexcept
	{
		return *this * lengthInv();
	}

	Vector2D& normalize() noexcept
	{
		return *this *= lengthInv();
	}

	[[nodiscard]] Vector2D rotated(double angle) const noexcept
	{
		const double s = std::sin(angle);
		const double c = std::cos(angle);
		return{ m_x * c - m_y * s, m_x * s + m_y * c };
	}

	Vector2D& rotate(double angle) noexcept
	{
		return *this = rotated(angle);
	}

	[[nodiscard]] double getAngle(const Vector2D& v) const
	{
		// if (isZero() || v.isZero()) throw new exception;

		return std::atan2(cross(v), dot(v));
	}

	[[nodiscard]] constexpr Vector2D projection(const Vector2D& onto) const noexcept
	{
		return onto.lengthSq() ? onto * dot(onto) / onto.lengthSq() : Zero();
	}

	[[nodiscard]] constexpr Vector2D lerp(const Vector2D& other, double f) const noexcept
	{
		return Vector2D(m_x + (other.m_x - m_x) * f, m_y + (other.m_y - m_y) * f);
	}

	template <class ShapeModel2DType>
	bool intersects(const ShapeModel2DType& shape) const
	{
		return Geometry2D::Intersect(*this, shape);
	}

	/// <summary>
	/// Vector2D{ m_x, m_x }
	/// </summary>
	constexpr Vector2D xx() const noexcept
	{
		return{ m_x, m_x };
	}

	/// <summary>
	/// Vector2D{ m_x, m_y }
	/// </summary>
	constexpr Vector2D xy() const noexcept
	{
		return{ m_x, m_y };
	}

	/// <summary>
	/// Vector2D{ m_y, m_x }
	/// </summary>
	constexpr Vector2D yx() const noexcept
	{
		return{ m_y, m_x };
	}

	/// <summary>
	/// Vector2D{ m_y, m_y }
	/// </summary>
	constexpr Vector2D yy() const noexcept
	{
		return{ m_y, m_y };
	}

	/// <summary>
	/// Vector2D{ 0, 0 }
	/// </summary>
	static constexpr Vector2D Zero() noexcept
	{
		return{ 0, 0 };
	}

	/// <summary>
	/// Vector2D{ 1, 1 }
	/// </summary>
	static constexpr Vector2D One() noexcept
	{
		return{ 1, 1 };
	}

	/// <summary>
	/// Vector2D{ value, value }
	/// </summary>
	static constexpr Vector2D All(double value = 1) noexcept
	{
		return{ value, value };
	}

	/// <summary>
	/// Vector2D{ 1, 0 }
	/// </summary>
	static constexpr Vector2D UnitX() noexcept
	{
		return{ 1, 0 };
	}

	/// <summary>
	/// Vector2D{ 0, 1 }
	/// </summary>
	static constexpr Vector2D UnitY() noexcept
	{
		return{ 0, 1 };
	}

	/// <summary>
	/// Vector2D{ -length, 0 }
	/// </summary>
	static constexpr Vector2D Left(double length = 1) noexcept
	{
		return{ -length, 0 };
	}

	/// <summary>
	/// Vector2D{ length, 0 }
	/// </summary>
	static constexpr Vector2D Right(double length = 1) noexcept
	{
		return{ length, 0 };
	}

	/// <summary>
	/// Vector2D{ 0, -length }
	/// </summary>
	static constexpr Vector2D Up(double length = 1) noexcept
	{
		return{ 0, -length };
	}

	/// <summary>
	/// Vector2D{ 0, length }
	/// </summary>
	static constexpr Vector2D Down(double length = 1) noexcept
	{
		return{ 0, length };
	}
};

inline constexpr Vector2D operator *(double s, const Vector2D& v) noexcept
{
	return v * s;
}
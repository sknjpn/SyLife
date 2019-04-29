#include<cmath>

struct Vector2D
{
	double m_x;
	double m_y;

	Vector2D() = default;

	constexpr Vector2D(double x, double y)
		: m_x(x)
		, m_y(y) {}

	double length() const
	{
		return std::sqrt(lengthSquare());
	}

	constexpr double lengthSquare() const
	{
		return dot(*this);
	}

	constexpr double dot(const Vector2D& v) const noexcept
	{
		return m_x * v.m_x + m_y * v.m_y;
	}

	constexpr double cross(const Vector2D& v) const noexcept
	{
		return m_x * v.m_y - m_y * v.m_x;
	}

	double distanceFrom(const Vector2D& other) const
	{
		return (other - *this).length();
	}

	Vector2D normalized() const
	{
		return *this / length();
	}

	Vector2D rotated(double theta) const
	{
		return Vector2D(m_x * cos(theta) - m_y * sin(theta), m_y * cos(theta) + m_x * sin(theta));
	}

	constexpr bool isZero() const
	{
		return m_x == 0.0 && m_y == 0.0;
	}

	constexpr Vector2D operator +() const
	{
		return *this;
	}

	constexpr Vector2D operator -() const
	{
		return{ -m_x, -m_y };
	}

	constexpr Vector2D operator +(const Vector2D& other) const
	{
		return{ m_x + other.m_x, m_y + other.m_y };
	}

	constexpr Vector2D operator -(const Vector2D& other) const
	{
		return{ m_x - other.m_x, m_y - other.m_y };
	}

	constexpr Vector2D operator *(double s) const
	{
		return{ m_x * s, m_y * s };
	}

	constexpr Vector2D operator /(double s) const
	{
		return{ m_x / s, m_y / s };
	}

	Vector2D& operator +=(const Vector2D& other)
	{
		m_x += other.m_x;
		m_y += other.m_y;
		return *this;
	}

	Vector2D& operator -=(const Vector2D& other)
	{
		m_x -= other.m_x;
		m_y -= other.m_y;
		return *this;
	}

	Vector2D& operator *=(double s)
	{
		m_x *= s;
		m_y *= s;
		return *this;
	}

	Vector2D& operator /=(double s)
	{
		m_x /= s;
		m_y /= s;
		return *this;
	}
};

inline constexpr Vector2D operator *(double s, const Vector2D& v)
{
	return{ s*v.m_x, s*v.m_y };
}

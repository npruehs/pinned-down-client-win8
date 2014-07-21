#pragma once

#include <string>

namespace PinnedDownClient
{
	namespace Math
	{
		struct Vector2F
		{
		public:
			// Fields.
			float x;
			float y;

			// Constructors.
			Vector2F()
				: Vector2F(0.0f, 0.0f)
			{
			}

			Vector2F(const float x, const float y)
			{
				this->x = x;
				this->y = y;
			}

			// Unary operators.
			inline Vector2F operator-() const
			{
				return Vector2F(-this->x, -this->y);
			}

			// Binary arithmetic operators.
			inline Vector2F operator+(const Vector2F &v) const
			{
				return Vector2F(this->x + v.x, this->y + v.y);
			}

			inline Vector2F& operator+=(const Vector2F &v) {
				this->x += v.x;
				this->y += v.y;

				return *this;
			}

			inline Vector2F operator-(const Vector2F &v) const
			{
				return Vector2F(this->x - v.x, this->y - v.y);
			}

			inline Vector2F& operator-=(const Vector2F &v) {
				this->x -= v.x;
				this->y -= v.y;

				return *this;
			}

			inline Vector2F operator*(const float f) const
			{
				return Vector2F(f * this->x, f * this->y);
			}

			inline Vector2F& operator*=(const float f) {
				this->x *= f;
				this->y *= f;

				return *this;
			}

			inline float operator*(const Vector2F &v) const
			{
				return (this->x * v.x + this->y * v.y);
			}

			inline Vector2F operator/(const float f) const
			{
				return Vector2F(this->x / f, this->y / f);
			}

			inline Vector2F& operator/=(const float f) {
				this->x /= f;
				this->y /= f;

				return *this;
			}

			// Binary comparison operators.
			inline bool operator==(const Vector2F& v) const
			{
				return (this->x == v.x) && (this->y == v.y);
			}

			inline bool operator!=(const Vector2F& v) const
			{
				return (this->x != v.x) || (this->y != v.y);
			}

			// ToString.
			std::wstring ToString()
			{
				return L"(" + std::to_wstring(this->x) + L" / " + std::to_wstring(this->y) + L")";
			}
		};
	}
}
#pragma once

#include <string>
#include "Vector2F.h"
#include "MathUtils.h"

namespace PinnedDownClient
{
	namespace Math
	{
		struct RectF
		{
		public:
			// Fields.
			Vector2F position;
			Vector2F size;

			// Constructors.
			RectF()
				: RectF(0.0f, 0.0f, 0.0f, 0.0f)
			{
			}

			RectF(float x, float y, float width, float height) : RectF(Vector2F(x, y), Vector2F(width, height))
			{
			}

			RectF(Vector2F position, Vector2F size)
			{
				this->position = position;
				this->size = size;
			}

			// Getters.
			inline float GetWidth() const 
			{
				return this->size.x;
			}

			inline float GetHeight() const 
			{
				return this->size.y;
			}

			inline float GetArea() const
			{
				return this->GetWidth() * this->GetHeight();
			}

			inline Vector2F GetCenter() const
			{
				return (this->position + this->size) / 2;
			}

			// Sides.
			inline float GetLeft() const
			{
				return this->position.x;
			}

			inline float GetRight() const
			{
				return this->position.x + this->size.x;
			}

			inline float GetTop() const
			{
				return this->position.y;
			}

			inline float GetBottom() const
			{
				return this->position.y + this->size.y;
			}

			// Corners.
			inline Vector2F GetTopLeft() const
			{
				return this->position;
			}

			inline Vector2F GetTopRight() const
			{
				return this->position + Vector2F(this->size.x, 0.0f);
			}

			inline Vector2F GetBottomLeft() const
			{
				return this->position + Vector2F(0.0f, this->size.y);
			}

			inline Vector2F GetBottomRight() const
			{
				return this->position + this->size;
			}

			// Comparsion.
			inline bool operator==(const RectF& other) const
			{
				return (this->position == other.position) && (this->size == other.size);
			}

			inline bool operator!=(const RectF& other) const
			{
				return (this->position != other.position) || (this->size == other.size);
			}

			bool Contains(const RectF& other) const
			{
				return (this->GetLeft() <= other.GetLeft() && this->GetRight() >= other.GetRight())
					&& (this->GetTop() <= other.GetTop() && this->GetBottom() >= other.GetBottom());
			}

			bool Contains(const Vector2F& point) const
			{
				return point.x >= this->GetLeft() && point.x < this->GetRight() && point.y >= this->GetTop() && point.y < this->GetBottom();
			}

			bool Intersects(const RectF& other) const
			{
				return (this->GetRight() > other.GetLeft() && this->GetLeft() < other.GetRight())
					&& (this->GetBottom() > other.GetTop() && this->GetTop() < other.GetBottom());
			}

			RectF Union(RectF& other) const
			{
				float left = Min(this->GetLeft(), other.GetLeft());
				float top = Min(this->GetTop(), other.GetTop());

				float right = Max(this->GetRight(), other.GetRight());
				float bottom = Max(this->GetBottom(), other.GetBottom());

				return RectF(Vector2F(top, left), Vector2F(right - left, bottom - top));
			}

			// ToString.
			std::string ToString()
			{
				return "Position: " + this->position.ToString() + " Size: " + this->size.ToString();
			}
		};
	}
}
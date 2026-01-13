#include "Header files/Vector2D.h"
#include <iostream> // for printing


// Default Constructor (0, 0).
Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

// Parameterized Constructor
Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}


// Adds the X and Y components of 'vec' to the current vector.
Vector2D& Vector2D::Addition(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this; // Returns the updated vector itself.
}

// Subtracts the X and Y components of 'vec' from the current vector.
Vector2D& Vector2D::Subtraction(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

// Multiplies the X and Y components of the current vector by 'vec' components.
Vector2D& Vector2D::Multiplication(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

// Divides the X and Y components of the current vector by 'vec' components.
Vector2D& Vector2D::Division(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

// Sets the vector back to zero position (0, 0).
Vector2D& Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;

	return *this;
}


// Operator Overload .
std::ostream& operator<< (std::ostream& stream, const Vector2D& vec)
{
	// Prints the format: (X,Y)
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}

// These allow syntax like 'v1 = v1 + v2;'.

Vector2D& operator+ (Vector2D& v1, const Vector2D& v2)
{
	return v1.Addition(v2);
}


Vector2D& operator- (Vector2D& v1, const Vector2D& v2)
{
	return v1.Subtraction(v2);
}


Vector2D& operator* (Vector2D& v1, const Vector2D& v2)
{
	return v1.Multiplication(v2);
}


Vector2D& operator/ (Vector2D& v1, const Vector2D& v2)
{
	return v1.Division(v2);
}

// These allow faster, in-place syntax like 'v1 += v2;'.

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Addition(vec);
}


Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Subtraction(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Multiplication(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Division(vec);
}


// Calculates v1 * i and returns a NEW vector (used for movement).
Vector2D Vector2D::operator*(const int& i) const
{
	// Returns a new vector without changing the original one.
	return Vector2D(x * i, y * i);
}

// Calculates v1 *= i and updates the current vector in place.
Vector2D& Vector2D::operator*=(const int& i)
{
	this->x *= i;
	this->y *= i;
	return *this;
}
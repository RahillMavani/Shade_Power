#pragma once
#include <iostream>

class Vector2D
{
public:
	// Declaring x and y coordinates here (Data fields)
	float x = 0.0f; //Initialized for safety (Fixes C26495 warnings)
	float y = 0.0f; //Initialized for safety (Fixes C26495 warnings)

	Vector2D();
	Vector2D(float x, float y);

	// Standard mathematical functions (member functions)
	Vector2D& Addition(const Vector2D& vector);
	Vector2D& Subtraction(const Vector2D& vector);
	Vector2D& Multiplication(const Vector2D& vector);
	Vector2D& Division(const Vector2D& vector);

	// Operator Overloads
	friend Vector2D& operator+ (Vector2D& v1, const Vector2D& v2); // Enables v1 = v1 + v2;
	friend Vector2D& operator- (Vector2D& v1, const Vector2D& v2); // Enables v1 = v1 - v2;
	friend Vector2D& operator* (Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/ (Vector2D& v1, const Vector2D& v2);

	// Compound Assignment Operator Overloads (e.g., v1 += v2;)
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	// Scalar Multiplication Overloads (e.g., v1 * 5)
	Vector2D operator*(const int& i) const;
	Vector2D& operator*=(const int& i);

	Vector2D& operator*(const int& i);
	Vector2D& Zero(); //Sets both x and y to 0.

	//Allows printing the vector directly to the console (e.g., cout << myVec).
	friend std::ostream& operator<< (std::ostream& stream, const Vector2D& vec);

};
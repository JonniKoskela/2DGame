#pragma once
#include <iostream>
#include <string>
#include "glm.hpp"
#include "math.h"
#include <chrono>
#include <fstream>
#include <sstream>


constexpr long double MPI = 3.14159265358979323851;

/*float toRadian(float degrees) {
	return degrees * MPI / 180.0;
}*/

class Matrix2f
{
public:
	float data[2][2];

	// Constructor to initialize the matrix to identity
	Matrix2f()
	{
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				data[i][j] = (i == j) ? 1.0f : 0.0f;
	}

	// Function to set the value at a specific row and column
	void set(int row, int col, float value)
	{
		data[row][col] = value;
	}

	// Function to rotate the matrix by a given angle (in radians)
	void rotate(float angle)
	{
		float cosTheta = cos(angle);
		float sinTheta = sin(angle);

		Matrix2f rotationMatrix;
		rotationMatrix.set(0, 0, cosTheta);
		rotationMatrix.set(0, 1, -sinTheta);
		rotationMatrix.set(1, 0, sinTheta);
		rotationMatrix.set(1, 1, cosTheta);

		// Multiply the current matrix by the rotation matrix
		*this *= rotationMatrix;
	}

	// Function to scale the matrix by a given factor in both dimensions
	void scale(float scaleX, float scaleY)
	{
		Matrix2f scaleMatrix;
		scaleMatrix.set(2, 0, scaleX);
		scaleMatrix.set(2, 1, scaleY);

		// Multiply the current matrix by the scale matrix
		*this *= scaleMatrix;
	}

	void translate(float translateX, float translateY)
	{
		Matrix2f translationMatrix;
		translationMatrix.set(0, 0, translateX);
		translationMatrix.set(1, 1, translateY);

		// Multiply the current matrix by the translation matrix
		*this *= translationMatrix;
	}

	// Overload the * operator for matrix multiplication
	Matrix2f operator*(const Matrix2f& other) const
	{
		Matrix2f result;
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				result.data[i][j] = data[i][0] * other.data[0][j] + data[i][1] * other.data[1][j];
		return result;
	}

	// Overload the *= operator for in-place matrix multiplication
	Matrix2f& operator*=(const Matrix2f& other)
	{
		*this = *this * other;
		return *this;
	}
};
class Matrix3f
{
public:
	float data[3][3]{};

	// Constructor to initialize the matrix to identity
	Matrix3f()
	{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				data[i][j] = (i == j) ? 1.0f : 0.0f;
	}

	void reset()
	{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				this->data[i][j] = (i == j) ? 1.0f : 0.0f;
	}
	// Function to set the value at a specific row and column
	void set(int row, int col, float value)
	{
		data[row][col] = value;
	}
	// Function to rotate the matrix around the Z-axis by a given angle (in radians)
	void rotate(float angle)
	{
		float cosTheta = cos(angle);
		float sinTheta = sin(angle);

		Matrix3f rotationMatrix;
		rotationMatrix.set(0, 0, cosTheta);
		rotationMatrix.set(0, 1, sinTheta);
		rotationMatrix.set(1, 0, -sinTheta);
		rotationMatrix.set(1, 1, cosTheta);

		// Multiply the current matrix by the rotation matrix
		*this *= rotationMatrix;
	}

	// Function to translate the matrix by given offsets in x and y
	void translate(float translateX, float translateY)
	{
		Matrix3f matrix{};
		matrix.data[2][0] = translateX;
		matrix.data[2][1] = translateY;
		
		*this *= matrix;
	}
	//void translate(Vec2 vec)
	//{
	//	Matrix3f matrix{};
	//	matrix.data[2][0] = vec.x;
	//	matrix.data[2][1] = vec.y;

	//	*this *= matrix;
	//}

	// Overload the * operator for matrix multiplication
	Matrix3f operator*(const Matrix3f& other) const
	{
		Matrix3f result;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				result.data[i][j] = data[i][0] * other.data[0][j] + data[i][1] * other.data[1][j] + data[i][2] * other.data[2][j];
		return result;
	}

	// Overload the *= operator for in-place matrix multiplication
	Matrix3f& operator*=(const Matrix3f& other)
	{
		*this = *this * other;
		return *this;
	}
};

//------------------------------------VECTORS
struct Vec4 
{
	float x{};
	float y{};
	float z{};
	float w{};
};
struct Vec3 
{
	float x{};
	float y{};
	float z{};
};
struct Vec2
{
	float x{};
	float y{};

	Vec2 operator/ (float scalar)
	{
		return{ x / scalar, y / scalar };
	}
	Vec2 operator- (Vec2 other)
	{
		return { x - other.x, y - other.y };
	}
	Vec2 operator* (float scalar)
	{
		return{ x * scalar, y * scalar };
	}
	Vec2 operator+ (Vec2 other)
	{
		return { x + other.x, y + other.y };
	}
	Vec2 operator*(const Matrix2f& matrix) const
	{
		Vec2 result;
		result.x = x * matrix.data[0][0] + y * matrix.data[1][0];
		result.y = x * matrix.data[0][1] + y * matrix.data[1][1];
		return result;
	}
	Vec2 operator*(const Matrix3f& matrix) const
	{
		Vec2 result;
		result.x = x * matrix.data[0][0] + y * matrix.data[1][0] + matrix.data[2][0];
		result.y = x * matrix.data[0][1] + y * matrix.data[1][1] + matrix.data[2][1];
		return result;
	}
};
std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
	os << "x: " << vec.x << " " << " y: " << vec.y << "\n";
	return os;
}
struct iVec2 
{
	int x{};
	int y{};

	iVec2 operator- (iVec2 other)
	{
		return { x - other.x, y - other.y };
	}
	iVec2 operator+ (iVec2 other)
	{
		return { x + other.x, y + other.y };
	}
	iVec2 operator* (int scalar)
	{
		return{ x * scalar, y * scalar };
	}
};
std::ostream& operator<<(std::ostream& os, const iVec2& vec)
{
	os << "x: " << vec.x << " " << " y: " << vec.y << "\n";
	return os;
}
Vec2 vec_2(iVec2 vec)
{
	return Vec2{ (float)vec.x, (float)vec.y };
}
iVec2 ivec_2(Vec2 vec)
{
	return iVec2{ (int)vec.x, (int)vec.y };
}


Vec2 transform_rotate(Vec2 vec, float angle)
{
	Vec2 rotated;
	return rotated;
}

//Vec2 transform_translate(Vec2& vec, int x, int y)
//{
//
//}





struct Matrix4f
{
	float data[4][4]{};

	Matrix4f operator*(const Matrix4f& other) const
	{
		Matrix4f result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.data[i][j] = 0.0f;
				for (int k = 0; k < 4; ++k)
				{
					result.data[i][j] += data[i][k] * other.data[k][j];
				}
			}
		}
		return result;
	}
	operator glm::mat4() const
	{
		glm::mat4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result[i][j] = data[i][j];
			}
		}
		return result;
	}
};

class Quaternionf
{
private:
float w, x, y, z;

public:
Quaternionf(float qw, float qx, float qy, float qz) : w(qw), x(qx), y(qy), z(qz) {}

Matrix4f toMatrix() const
{
	Matrix4f rotationMatrix;

	rotationMatrix.data[0][0] = 1 - 2 * (y * y + z * z);
	rotationMatrix.data[0][1] = 2 * (x * y - w * z);
	rotationMatrix.data[0][2] = 2 * (x * z + w * y);
	rotationMatrix.data[0][3] = 0.0f;

	rotationMatrix.data[1][0] = 2 * (x * y + w * z);
	rotationMatrix.data[1][1] = 1 - 2 * (x * x + z * z);
	rotationMatrix.data[1][2] = 2 * (y * z - w * x);
	rotationMatrix.data[1][3] = 0.0f;

	rotationMatrix.data[2][0] = 2 * (x * z - w * y);
	rotationMatrix.data[2][1] = 2 * (y * z + w * x);
	rotationMatrix.data[2][2] = 1 - 2 * (x * x + y * y);
	rotationMatrix.data[2][3] = 0.0f;

	rotationMatrix.data[3][0] = 0.0f;
	rotationMatrix.data[3][1] = 0.0f;
	rotationMatrix.data[3][2] = 0.0f;
	rotationMatrix.data[3][3] = 1.0f;

	return rotationMatrix;
}

	Quaternionf operator*(const Quaternionf& other) const
	{
		float newW = w * other.w - x * other.x - y * other.y - z * other.z;
		float newX = w * other.x + x * other.w + y * other.z - z * other.y;
		float newY = w * other.y - x * other.z + y * other.w + z * other.x;
		float newZ = w * other.z + x * other.y - y * other.x + z * other.w;

		return Quaternionf(newW, newX, newY, newZ);
	}

		/*void rotate(float angle, float axisX, float axisY, float axisZ)
		{
			float halfAngle = angle * 0.5f;
			float sinHalfAngle = sinf(halfAngle);

			float newW = cosf(halfAngle);
			float newX = axisX * sinHalfAngle;
			float newY = axisY * sinHalfAngle;
			float newZ = axisZ * sinHalfAngle;


			Quaternionf rotation(newW, newX, newY, newZ);
			*this = rotation * (*this);
		}*/
};


Matrix4f orthographicProjection(float left, float right, float top, float bottom)
{
	Matrix4f resultMat;
	resultMat.data[3][0] = -(right + left) / (right - left);
	resultMat.data[3][1] = -(top + bottom) / (top - bottom);
	resultMat.data[3][2] = -1.0f;
	resultMat.data[0][0] = 2.0f / (right - left);
	resultMat.data[1][1] = 2.0f / (top - bottom);
	resultMat.data[2][2] = -2.0f / (1.0f - 0.0f);
	resultMat.data[3][3] = 1.0f;

	return resultMat;
}
Matrix4f makeRotationMatrix(float angle)
{
	Matrix4f resultMat;

	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	resultMat.data[0][0] = cosTheta;
	resultMat.data[0][1] = -sinTheta;
	resultMat.data[1][0] = sinTheta;
	resultMat.data[1][1] = cosTheta;

	resultMat.data[2][2] = 1.0f;
	resultMat.data[3][3] = 1.0f;

	return resultMat;
}

class sprite2D
{
	iVec2 size{};
	iVec2 position{};
};


struct iRect
{
	iVec2 pos;
	iVec2 size;

	iRect(Vec2 a, Vec2 b)
	{
		pos.x = a.x;
		pos.y = a.y;
		size.x = b.x;
		size.y = b.y;
	}
	iRect(Vec2 a, iVec2 b)
	{
		pos.x = a.x;
		pos.y = a.y;
		size.x = b.x;
		size.y = b.y;
	}
	iRect(iVec2 a, iVec2 b)
	{
		pos.x = a.x;
		pos.y = a.y;
		size.x = b.x;
		size.y = b.y;
	}
	iRect(iVec2 a, Vec2 b)
	{
		pos.x = a.x;
		pos.y = a.y;
		size.x = b.x;
		size.y = b.y;
	}
};
struct Rect
{
	Vec2 pos;
	Vec2 size;

};


bool rectCollision(iRect a, iRect b)
{
	return
		a.pos.x < b.pos.x + b.size.x &&
		a.pos.x + a.size.x > b.pos.x &&
		a.pos.y < b.pos.y + b.size.y &&
		a.pos.y + a.size.y > b.pos.y;
}
bool rectCollision(Rect a, Rect b)
{
	return
		a.pos.x < b.pos.x + b.size.x &&
		a.pos.x + a.size.x > b.pos.x &&
		a.pos.y < b.pos.y + b.size.y &&
		a.pos.y + a.size.y > b.pos.y;
}
bool rectCollision(Rect a, iRect b)
{
	return
		a.pos.x < b.pos.x + b.size.x &&
		a.pos.x + a.size.x > b.pos.x &&
		a.pos.y < b.pos.y + b.size.y &&
		a.pos.y + a.size.y > b.pos.y;
}


bool pointInRect(Vec2 point, Rect rect)
{
	return (point.x >= rect.pos.x &&
		point.x <= rect.pos.x + rect.size.x &&
		point.y >= rect.pos.y &&
		point.y <= rect.pos.y + rect.size.y);
}
bool pointInRect(Vec2 point, iRect rect)
{
	return (
		point.x >= rect.pos.x &&
		point.x <= rect.pos.x + rect.size.x &&
		point.y >= rect.pos.y &&
		point.y <= rect.pos.y + rect.size.y
		);
}

//----------------------------------------------------UTILITY FUNCTIONS
Vec2 normalize(const Vec2& vec)
{
	Vec2 result{};
	float div = sqrtf((vec.x * vec.x) + (vec.y * vec.y));
	result.x = vec.x / div;
	result.y = vec.y / div;
	return result;
}

Vec2 normalizeTo(Vec2& playerPos, Vec2& mousePos)
{
	Vec2 result = playerPos - mousePos;
	return normalize(result);
}

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

float approach(float current, float target, float increase)
{
	if (current < target)
	{
		return std::min(current + increase, target);

	}
	return std::max(current - increase, target);
}



float toRadians()
{
	return 0.0f;
}

float distanceBetween(Vec2 posA, Vec2 posB)
{
	float distanceX = abs(posA.x - posB.x);
	float distanceY = abs(posA.y - posB.y);
	return sqrt(distanceX * distanceX + distanceY * distanceY);
}

void getFPS(float interval)
{
	using Duration = std::chrono::duration<float>;
	using Clock = std::chrono::steady_clock;

	static auto previousFPSTime = Clock::now();
	static auto currentFPSTime = Clock::now();
	static double acc;
	static int ticks;
	
	currentFPSTime = Clock::now();
	Duration dt = std::chrono::duration_cast<std::chrono::microseconds>(currentFPSTime - previousFPSTime);

	previousFPSTime = currentFPSTime;

	++ticks;
	acc += dt.count();
	
	if (acc > interval)
	{
		std::cout << ticks << "\n";

		acc = 0;
		ticks = 0;
	}
}

//accepts std::string or any arithmetic type
template<typename T>
std::vector<T> filepath_vec(const char* filePath)
{
	std::ifstream ifs(filePath);
	std::string str;
	std::vector<T> vec;
	std::string::size_type st;
	T val;
	while (std::getline(ifs, str, ','))
	{
		val = std::stoi(str, &st);
		vec.push_back(val);
	}
	ifs.close();
	return vec;
}

template<>
std::vector<std::string> filepath_vec<std::string>(const char* filePath)
{
	std::ifstream ifs(filePath);
	std::string str;
	std::vector<std::string> vec;
	while (std::getline(ifs, str, ','))
	{
		vec.push_back(str);
	}
	ifs.close();
	return vec;
}

template<typename T>
std::vector<T> filepath_vec(std::string& filePath)
{
	char* p = &filePath;
	return filepath_vec<T>(filePath);
}

//float getMilliseconds()
//{
//		auto currentTime = Clock::now();
//		std::chrono::duration<float, std::milli> elapsed ;
//		return elapsed.count();
//}


//void rotateTransform(Transform transform, float angle)
//{
//	float left 
//	float top
//	float right
//	float bottom = transform.size.y
//}
#pragma once


constexpr long double MPI = 3.14159265358979323851;

/*float toRadian(float degrees) {
	return degrees * MPI / 180.0;
}*/

struct Vec4 {
	float x{};
	float y{};
	float z{};
	float w{};
};

struct Vec3 {
	float x{};
	float y{};
	float z{};
};
struct Vec2 {
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
};


struct iVec2 {
	int x{};
	int y{};

	iVec2 operator- (iVec2 other)
	{
		return { x - other.x, y - other.y };
	}
};

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
	};

	class Quaternionf {
	private:
		float w, x, y, z;

	public:
		// Constructor
		Quaternionf(float qw, float qx, float qy, float qz) : w(qw), x(qx), y(qy), z(qz) {}

		// Method to convert the quaternion to a rotation matrix
		Matrix4f toMatrix() const {
			Matrix4f rotationMatrix;

			// Assuming Matrix4f is a 4x4 matrix type
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

		Quaternionf operator*(const Quaternionf& other) const {
			float newW = w * other.w - x * other.x - y * other.y - z * other.z;
			float newX = w * other.x + x * other.w + y * other.z - z * other.y;
			float newY = w * other.y - x * other.z + y * other.w + z * other.x;
			float newZ = w * other.z + x * other.y - y * other.x + z * other.w;

			return Quaternionf(newW, newX, newY, newZ);
		}

		/*void rotate(float angle, float axisX, float axisY, float axisZ) {
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
		resultMat.data[3][1] = (top + bottom) / (top - bottom);
		resultMat.data[3][2] = 0.0f;
		resultMat.data[0][0] = 2.0f / (right - left);
		resultMat.data[1][1] = 2.0f / (top - bottom);
		resultMat.data[2][2] = 1.0f / (1.0f - 0.0f);
		resultMat.data[3][3] = 1.0f;

		return resultMat;
	}



	class sprite2D {
		iVec2 size{};
		iVec2 position{};
	};
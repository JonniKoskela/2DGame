#pragma once
#include <vector>

#define GLSL_bool alignas(4) uint32_t
#define GLSL_int alignas(4) int32_t
#define GLSL_uint alignas(4) uint32_t
#define GLSL_float alignas(4) float

#define GLSL_vec2 alignas(8) Vec2
#define GLSL_ivec2 alignas(8) iVec2


struct attackRenderData4xVec2
{
	GLSL_int attackFlag{};
	GLSL_vec2 vertices[4]{};
	GLSL_ivec2 textureAtlasPosition{};
	GLSL_ivec2 textureAtlasOffset{};

	attackRenderData4xVec2& operator*(const Matrix3f& transform)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			this->vertices[i] = this->vertices[i] * transform;
		}

		return *this;
	}

	Vec2 normalizeVertices()
	{
		float fx = this->vertices[0].x;
		float fy = this->vertices[0].y;
		for (int i = 0;i<4;++i)
		{
			this->vertices[i].x -= fx;
			this->vertices[i].y -= fy;
		}
		return { fx,fy };
	}
	void rotateAroundHandle(float angle)
	{
		Matrix3f xform;
		xform.rotate(angle);
		Vec2 offset = this->normalizeVertices();
		float lx = lerp(this->vertices[0].x, this->vertices[1].x, 0.5f);
		float ly = lerp(this->vertices[0].y, this->vertices[1].y, 0.5f);
		for (auto& vertex : this->vertices)
		{
			vertex = vertex * xform;
			vertex = vertex + offset;
		}
	}
};


static std::vector<attackRenderData4xVec2> attackTransforms{};
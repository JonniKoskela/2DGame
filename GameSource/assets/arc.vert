#version 430 core
//layout (location = 0) in vec2 aPos;
uniform mat4 orthoProjection;
out int renderDataAttackFlag;

layout (location = 0) uniform sampler2D textureAtlas;

layout (location = 0) out vec2 textureCoordsOut;


struct attackRenderData4xVec2
{
	int attackFlag;
	vec2 vertices[4];
};
layout (std430, binding = 0) buffer attackTransformSBO
{
  attackRenderData4xVec2 attackTransforms[];
}; 

void main() {
	attackRenderData4xVec2 attackTransform = attackTransforms[gl_InstanceID];
	
	vec2 triangleVertices[6] = 
	{
	attackTransform.vertices[0],
	attackTransform.vertices[1],
	attackTransform.vertices[3],
	attackTransform.vertices[3],
	attackTransform.vertices[1],
	attackTransform.vertices[2],
	};
	vec2 vertexPos = triangleVertices[gl_VertexID];
    gl_Position = orthoProjection * vec4(vertexPos, 0.0, 1.0);
	renderDataAttackFlag = attackTransform.attackFlag;
}
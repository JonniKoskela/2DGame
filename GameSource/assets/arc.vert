#version 430 core
//layout (location = 0) in vec2 aPos;
uniform mat4 orthoProjection;
out flat int renderDataAttackFlag;




layout (location = 0) out vec2 attackTextureCoordsOut;


struct attackRenderData4xVec2
{
	int attackFlag;
	vec2 vertices[4];
	ivec2 atlasPosition;
	ivec2 spriteSize;
};
layout (std430, binding = 0) buffer attackTransformSBO
{
  attackRenderData4xVec2 attackTransforms[];
}; 

void main() {
	attackRenderData4xVec2 attackTransform = attackTransforms[gl_InstanceID];
	renderDataAttackFlag = attackTransform.attackFlag;
	vec2 triangleVertices[6] = 
	{
	attackTransform.vertices[0],
	attackTransform.vertices[1],
	attackTransform.vertices[3],
	attackTransform.vertices[3],
	attackTransform.vertices[1],
	attackTransform.vertices[2],
	};


if(attackTransform.attackFlag == 1){
  float left = attackTransform.atlasPosition.x;
  float top = attackTransform.atlasPosition.y;
  float right = attackTransform.atlasPosition.x + attackTransform.spriteSize.x;
  float bottom = attackTransform.atlasPosition.y + attackTransform.spriteSize.y;
vec2 textureCoords[6] = 
{
  vec2(left, bottom),
  vec2(left, top),
  vec2(right, bottom),
  vec2(right, bottom),
  vec2(left, top),
  vec2(right, top),

};
attackTextureCoordsOut = textureCoords[gl_VertexID];
}  


	vec2 vertexPos = triangleVertices[gl_VertexID];
    gl_Position = orthoProjection * vec4(vertexPos, 0.0, 1.0);

}
#version 430 core


struct Transform 
{ 
  vec2 size; 
  vec2 pos;
  ivec2 atlasOffset;
  ivec2 spriteSize;
};


layout (std430, binding = 0) buffer TransformSBO
{
  Transform transforms[];
}; 
uniform mat4 orthoProjection;
uniform vec2 screenSize;
uniform mat4 rotationMatrix;

layout (location = 0) out vec2 textureCoordsOut;


void main()
{
  Transform transform = transforms[gl_InstanceID];

  vec2 vertices[6] =
  {
    transform.pos,                                        // Top Left
    vec2(transform.pos + vec2(0.0, transform.size.y)),    // Bottom Left
    vec2(transform.pos + vec2(transform.size.x, 0.0)),    // Top Right
    vec2(transform.pos + vec2(transform.size.x, 0.0)),    // Top Right
    vec2(transform.pos + vec2(0.0, transform.size.y)),    // Bottom Left
    transform.pos + transform.size                        // Bottom Right
  };

  float left = transform.atlasOffset.x;
  float top = transform.atlasOffset.y;
  float right = transform.atlasOffset.x + transform.spriteSize.x;
  float bottom = transform.atlasOffset.y + transform.spriteSize.y;

vec2 textureCoords[6] = 
{
  vec2(left, bottom),
  vec2(left, top),
  vec2(right, bottom),
  vec2(right, bottom),
  vec2(left, top),
  vec2(right, top),
};
   {
    vec2 vertexPos = vertices[gl_VertexID];
    gl_Position = orthoProjection * vec4(vertexPos,0.0,1.0); 
   }

  textureCoordsOut = textureCoords[gl_VertexID];
}
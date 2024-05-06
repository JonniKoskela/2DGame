#version 430 core

uniform mat4 orthoProjection;
uniform vec2 screenSize;

uniform uint mapHeight;
uniform uint mapWidth;

layout(std430, binding = 0) buffer MapBuffer
{
    uint tileLayer[];
};

layout (location = 0) out vec2 textureCoordsOut;


void main()
{
    uint currentTile = tileLayer[gl_InstanceID];
    uint idx = gl_InstanceID;
    float posX = (gl_InstanceID % mapWidth) * 32.0;
    float posY = floor((gl_InstanceID / mapWidth)) * 32.0;   
    vec2 posVec = {posX,posY};


  vec2 vertices[6] = 
    {   
    posVec,                            // Top Left
    vec2(posVec + vec2(0.0, 32.0)),    // Bottom Left
    vec2(posVec + vec2(32.0, 0.0)),    // Top Right
    vec2(posVec + vec2(32.0, 0.0)),    // Top Right
    vec2(posVec + vec2(0.0, 32.0)),    // Bottom Left
    posVec + vec2(32.0,32.0)                  // Bottom Right
    };

    float tileAtlasPosX = (currentTile % 10) * 32.0;
    float tileAtlasPosY = floor(currentTile / 10) * 32.0;
    vec2 tileAtlasVec = {tileAtlasPosX,tileAtlasPosY};
    float left = tileAtlasPosX;
    float top = tileAtlasPosY;
    float right = tileAtlasPosX + 32.0;
    float bottom = tileAtlasPosY + 32.0;

vec2 textureCoords[6] = 
{
  vec2(left, bottom),
  vec2(left, top),
  vec2(right, bottom),
  vec2(right, bottom),
  vec2(left, top),
  vec2(right, top),
};

vec2 vertexPos = vertices[gl_VertexID];
gl_Position = orthoProjection * vec4(vertexPos,0.0,1.0);

textureCoordsOut = textureCoords[gl_VertexID];
}
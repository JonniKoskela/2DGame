#version 430 core


in flat int renderDataAttackFlag;
layout (location = 0) in vec2 textureCoordsIn;
layout (location = 0) out vec4 FragColor;
layout (location = 0) uniform sampler2D textureAtlas;
//how far from original position

// attackflag = :
//      0. primitive
//      1. animated with texture



void main()
{
    switch (renderDataAttackFlag)
    {
        case 0:
        FragColor = vec4(1.0,1.0,1.0,1.0);
        break;
    
        case 1:
        vec4 textureColor = texelFetch(textureAtlas, ivec2(textureCoordsIn), 0);
	
	    if(textureColor.a == 0)
	    {
		    discard; 
	    }

	    FragColor = textureColor;
        break;

/*        default:
        FragColor = vec4(0.0,0.0,0.0,1.0);
        break;*/
    }
}

#ifndef PI
#define PI 3.141592653589793
#endif

float circularInOut(float t);
float bounceOut(float t);

float circularInOut(float t) {
  return t < 0.5
    ? 0.5 * (1.0 - sqrt(1.0 - 4.0 * t * t))
    : 0.5 * (sqrt((3.0 - 2.0 * t) * (2.0 * t - 1.0)) + 1.0);
}
float bounceOut(float t) {
  const float a = 4.0 / 11.0;
  const float b = 8.0 / 11.0;
  const float c = 9.0 / 10.0;

  const float ca = 4356.0 / 361.0;
  const float cb = 35442.0 / 1805.0;
  const float cc = 16061.0 / 1805.0;

  float t2 = t * t;

  return t < a
    ? 7.5625 * t2
    : t < b
      ? 9.075 * t2 - 9.9 * t + 3.4
      : t < c
        ? ca * t2 - cb * t + cc
        : 10.8 * t * t - 20.52 * t + 10.72;
}
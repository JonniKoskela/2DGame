#version 430 core
out vec4 FragColor;
uniform vec3 arcColor;
uniform float fadeDuration;
uniform float currentTime;
uniform int attackFlag;

#ifndef PI
#define PI 3.141592653589793
#endif
float circularInOut(float t);

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

void main()
{
    switch (attackFlag)
    {
        case 0:
        float alpha = (1-(currentTime / fadeDuration));
        FragColor = vec4(arcColor, alpha);
        break;
    
    case 1:
        float alphaSlam =(currentTime / fadeDuration);
        FragColor = vec4(arcColor, alphaSlam);
        break;
    }
}

float circularInOut(float t) {
  return t < 0.5
    ? 0.5 * (1.0 - sqrt(1.0 - 4.0 * t * t))
    : 0.5 * (sqrt((3.0 - 2.0 * t) * (2.0 * t - 1.0)) + 1.0);
}
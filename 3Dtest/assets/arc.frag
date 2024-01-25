#version 430 core
out vec4 FragColor;
uniform vec3 arcColor;
uniform float fadeDuration;
uniform float currentTime;

void main() {
    float alpha = 1-(currentTime / fadeDuration);
    FragColor = vec4(arcColor, alpha);
}
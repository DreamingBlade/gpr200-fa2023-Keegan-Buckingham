#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;

uniform float iTime;

void main(){
	UV = vUV;
	vec3 position = vPos;
	position.y = position.y + 1- abs(sin(iTime));

	position.x = position.x +sin(iTime) * .5;

	float size = 10 - abs(sin(iTime))*6;
	gl_Position = vec4(position,size);
}
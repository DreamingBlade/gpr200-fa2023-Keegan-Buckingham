#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;

uniform float iTime;

////KEEGAN////
void main(){
	UV = vUV;
	vec3 position = vPos;

	float size = 1;
	gl_Position = vec4(position,size);
}
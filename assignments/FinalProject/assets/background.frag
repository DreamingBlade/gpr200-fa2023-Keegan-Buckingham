#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _GrassTexture;
uniform sampler2D _NoiseTexture;

uniform float iTime;

void main(){

	float noise = texture(_NoiseTexture,UV).r;
	vec2 uv = UV + noise * sin(iTime)*.5;
	uv.x = uv.x + iTime*.1;

	FragColor = texture(_GrassTexture,uv);
}

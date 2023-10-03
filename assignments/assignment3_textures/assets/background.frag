#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _BrickTexture;
uniform sampler2D _NoiseTexture;
uniform sampler2D _SadTexture;

uniform float iTime;

void main(){

	float noise = texture(_NoiseTexture,UV).r;
	vec2 uv = UV + noise + 0.1f * cos(iTime);

	FragColor = texture(_BrickTexture,uv);
	/*
	float noise = texture(_NoiseTexture,UV).r;
	vec2 uv = UV + noise * 0.1f;
	vec4 colorA = texture(_BrickTexture,uv);
	vec4 colorB = texture(_SadTexture,uv);

	vec3 color = mix(colorA.rgb, colorB.rgb, colorB.a * 0.5);

	FragColor = vec4(color,UV);
	*/
}

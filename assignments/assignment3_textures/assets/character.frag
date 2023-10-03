#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _CharacterTexture;

uniform float iTime;

void main(){

	FragColor = texture(_CharacterTexture,UV);
}
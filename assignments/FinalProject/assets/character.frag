#version 450
out vec4 FragColor;
in vec2 UV;

uniform vec2 _CharacterPosition; //the screen UV center of the character
uniform sampler2D _CharacterTexture;
uniform vec2 _SpriteUVCoordinates; //The sprite's UV coordinates on the sheet
uniform vec2 _SpriteUVSize; //The sprites UV size on the sheet


uniform float _Scale;
uniform float _Opacity;

uniform float iTime;

void main(){

	vec2 UV_sample = UV; //this variable is which part of the texture it actually uses

	//Maps the screen UV to the texture UV
	UV_sample -= _CharacterPosition;
	UV_sample /= _Scale;
	UV_sample += vec2(0.5,0.5);
	//loads in the texture
	FragColor = texture(_CharacterTexture,UV_sample);
	
	//determines the distance between the current UV & the sprite's center
	float xDist = distance(UV.x,_SpriteUVCoordinates.x);
	float yDist = distance(UV.y,_SpriteUVCoordinates.y);
	float xStep = step(_SpriteUVSize.x,xDist);
	float yStep = step(_SpriteUVSize.y,yDist);
	float d = max(xStep,yStep);
	//If that distance is larger than the sprite size, pixels are all transparent
	//d = step(_SpriteUVSize.y,d);
	FragColor.a *= _Opacity-d;

	/*
	//determines the distance between the current UV & the character's center
	float xd = distance(UV.x,_CharacterPosition.x);
	float yd = distance(UV.y,_CharacterPosition.y);
	float d = max(xd,yd);
	//If that distance is larger than the character size, pixels are all transparent
	d = step(_Scale/2,d);
	FragColor.a *= _Opacity-d;
	*/

	/*
	vec2 uv = UV;

	FragColor = texture(_CharacterTexture,uv);
	*/
}

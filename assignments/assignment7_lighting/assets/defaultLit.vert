#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

//This entire block will be passed to our fragment shader.
out Surface{
	vec2 UV;

	//TODO: Add vec3 WorldPosition
	vec3 WorldPosition;

	//TODO: Add vec3 WorldNormal
	vec3 WorldNormal;

}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	vs_out.UV = vUV;
	
	//TODO: Calculate vs_out.WorldPosition
	vec3 WorldPosition = vPos;

	//TODO: Calculate vs_out.WorldNormal (see Normal Matrix in slides)
	//converts vertex position to world space;
	vec4 worldVertexPosition = _Model * vec4(vPos,1.0);

	//Converts vertex normal to world space
	vec4 worldVertexNormal = _Model * vec4(vNormal, 0.0);

	//Top left 3x3 of _Model times a vec3
	vec3 modelNormal = mat3(_Model) * vNormal;
	
	vec3 WorldNormal = transpose(inverse(mat3(_Model)))*vNormal;

	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}

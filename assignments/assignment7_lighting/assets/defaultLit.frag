#version 450
out vec4 FragColor;

//defaultLit.frag
struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;
void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	//TODO: Lighting calculations using corrected normal

	float I = max(dot(normalize(normal), normalize(_Lights[0].position)),0);

	vec3 rNormal = reflect(-normalize(_Lights[0].position), normal);


}

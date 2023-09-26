#version 450
	out vec4 fragColor;
	in vec2 UV;
	void main(){
	vec3 color = mix(vec3(1.0,1.0,0.0),vec3(0.9,0.0,0.5),UV.y);
    
//Get 0-1 T value for hill shape
float hills = 1.0 - step(sin(UV.x*6.0) * 0.2 + 0.3,UV.y);
    
//Blend dark grey hills
color = mix(color,vec3(0.2),hills);
		fragColor = vec4(color,1.0);
	}
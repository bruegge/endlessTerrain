#version 430 core

layout(quads) in;

in TCS_OUT 
{
    float height;
} tes_in[];

out TES_OUT 
{
    float height;
} tes_out;

vec4 Interpolate4D(vec4 p1, vec4 p2, vec4 p3, vec4 p4)
{
	vec4 pm1 = mix(p1,p2, gl_TessCoord.x);
	vec4 pm2 = mix(p3,p4, gl_TessCoord.x);
	return mix(pm1, pm2, gl_TessCoord.y);
}

vec3 Interpolate3D(vec3 p1, vec3 p2, vec3 p3, vec3 p4)
{
	vec3 pm1 = mix(p1,p2, gl_TessCoord.x);
	vec3 pm2 = mix(p3,p4, gl_TessCoord.x);
	return mix(pm1, pm2, gl_TessCoord.y);
}

vec2 Interpolate2D(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
	vec2 pm1 = mix(p1,p2, gl_TessCoord.x);
	vec2 pm2 = mix(p3,p4, gl_TessCoord.x);
	return mix(pm1, pm2, gl_TessCoord.y);
}

float Interpolate1D(float p1, float p2, float p3, float p4)
{
	float pm1 = mix(p1,p2, gl_TessCoord.x);
	float pm2 = mix(p3,p4, gl_TessCoord.x);
	return mix(pm1, pm2, gl_TessCoord.y);
}

void main()
{
	tes_out.height = Interpolate1D(tes_in[0].height, tes_in[1].height, tes_in[2].height, tes_in[3].height);
}	

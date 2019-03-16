#version 430 core

layout (vertices = 4) out;

uniform vec3 cameraPosition = vec3(10,0,0);

in VS_OUT 
{
    float height;
} tcs_in[];

out TCS_OUT 
{
    float height;
} tcs_out[];

void main(void)
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelOuter[0] = 1;
		gl_TessLevelOuter[1] = 1;
		gl_TessLevelOuter[2] = 1;
		gl_TessLevelOuter[3] = 1;
		gl_TessLevelInner[0] = 1;
		gl_TessLevelInner[1] = 1;
	}
	
	tcs_out[gl_InvocationID].height = tcs_in[gl_InvocationID].height;
}
